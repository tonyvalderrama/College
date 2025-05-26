/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
						 INGENIERIA EN SISTEMAS COMPUTACIONALES
										Sistemas Operativos
M¢dulo     : Funciones gr ficas.
Archivo    : graf-ms3.c
Objetivo   : Funciones para la interfaz gr fica del Simulador de un Sistema
				 Operativo.
Dise¤o     : Jorge Eduardo Aparicio Mayorga  -  90120511-3,
				 Jos‚ Antonio Valderrama Ramos  -  90120509-9.
Implemento : Los mismos.
Versi¢n    : Noviembre 1993.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include "graf-ms3.h"
#include "mouse.h"
#include "dsls.h"
#include "so01.h"

#define VEL (10-vel)
#define BxP 4

void ini_graf(),draw_toolbox(int,int),error(char *),draw_interface();
void erase_window(),dialog_box(),door(int),write_instruc(char *);
void move_proc(int,int),anim_lae(int),anim_eal(int),anim_lasl(int);
void anim_lab(int),anim_bal(int),anim_slal(int),anim_easl(int);
void anim_slasb(int),anim_sbasl(int),anim_eab(int),anim_sbab(int);
void anim_basb(int),draw_bullet(int,int,int), aviso(char *);
void move_bullet(int,int,int,int,int,int,void **,int),move_proc(int,int);
void paint_recursos(int,int),paint_registers(),paint_flags();

unsigned int _x=10,_y=10;
int r1=107,r2=107,r3=107,r4=107,r5=107;    /* Posiciones de los recursos. */
extern int vel;
extern int regs[];
extern unsigned char SF,ZF;

int __x, __y;
char ratonazo=0;


/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : ini_graf.
Objetivo   : Inicializa el modo gr fico del simulador en modo VGA high.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void ini_graf()
{
	int driver=VGA,mode=VGAHI;
	initgraph(&driver,&mode,"");
	if (graphresult())
		{
		printf("\nNo puedo inicializar modo gr fico, perdone.\n");
		exit(0);
		}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : draw_toolbox.
Objetivo   : Dibujar la caja de herramientas en la posici¢n por default.
Par metros : int x,y = Coordenadas de esquina superior derecha del toolbox.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void draw_toolbox(int x,int y)
{
	if (x<1)
		_x = x = 1;
	if (y<1)
		_y = y = 1;
	setcolor(WHITE);
	rectangle(x,y,x+190,y+60);
	line(x+10,y+30,x+190,y+30);
	line(x+10,y,x+10,y+60);
	line(x+40,y,x+40,y+60);
	line(x+70,y,x+70,y+60);
	line(x+100,y,x+100,y+60);
	line(x+130,y,x+130,y+60);
	line(x+160,y,x+160,y+60);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	rectangle(x+13,y+8,x+27,y+22);                   /* Dibujo de "Cargar".*/
	rectangle(x+17,y+17,x+23,y+22);
	circle(x+20,y+13,1);
	setcolor(LIGHTRED);
	putpixel(x+32,y+12,LIGHTRED);
	line(x+32,y+13,x+34,y+13);
	line(x+29,y+14,x+35,y+14);
	line(x+29,y+15,x+36,y+15);
	line(x+29,y+16,x+35,y+16);
	line(x+32,y+17,x+34,y+17);
	putpixel(x+32,y+18,LIGHTRED);
	setcolor(WHITE);                                  /* Icono de "Salir". */
	line(x+165,y+35,x+175,y+35);
	line(x+165,y+35,x+165,y+50);
	line(x+175,y+35,x+175,y+50);
	line(x+165,y+35,x+170,y+40);
	line(x+170,y+40,x+170,y+55);
	line(x+170,y+55,x+165,y+50);
	line(x+170,y+50,x+175,y+50);
	setcolor(LIGHTMAGENTA);
	putpixel(x+181,y+40,LIGHTMAGENTA);
	line(x+180,y+41,x+181,y+41);
	line(x+179,y+42,x+186,y+42);
	line(x+178,y+43,x+186,y+43);
	line(x+179,y+44,x+186,y+44);
	line(x+180,y+45,x+181,y+45);
	putpixel(x+181,y+46,LIGHTMAGENTA);
	setcolor(WHITE);                                 /* Icono de Memoria   */
	line(x+135,y+35, x+135, y+55);
	line(x+146,y+35, x+146, y+55);
	setcolor(DARKGRAY);
	rectangle(x+136,y+37, x+145, y+40);
	rectangle(x+136,y+38, x+145, y+39);
	setcolor(LIGHTRED);
	rectangle(x+136, y+42, x+145, y+47);
	setfillstyle(SOLID_FILL,LIGHTRED);
	floodfill(x+141,y+45,LIGHTRED);
	setcolor(LIGHTBLUE);
	rectangle(x+136, y+49, x+145, y+53);
	setfillstyle(SOLID_FILL,LIGHTBLUE);
	floodfill(x+141,y+51,LIGHTBLUE);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	outtextxy(x+151,y+38,"M");
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,RED);                 /* Icono de "Suspender". */
	line(x+80,y+5,x+90,y+5);
	line(x+80,y+26,x+90,y+26);
	line(x+80,y+5,x+73,y+12);
	line(x+90,y+5,x+97,y+12);
	line(x+73,y+12,x+73,y+19);
	line(x+97,y+12,x+97,y+19);
	line(x+73,y+19,x+80,y+26);
	line(x+97,y+19,x+90,y+26);
	floodfill(x+85,y+10,WHITE);
	settextstyle(SMALL_FONT,HORIZ_DIR,3);
	outtextxy(x+78,y+11,"STOP");
	setfillstyle(SOLID_FILL,GREEN);                /* Icono de "Reanudar". */
	rectangle(x+105,y+5,x+125,y+25);
	floodfill(x+120,y+10,WHITE);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	outtextxy(x+110,y+9,"GO");
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,1);           /* Icono de "Ayuda". */
	outtextxy(x+142,y+3,"?");
	setcolor(LIGHTRED);                               /* Icono de "Matar". */
	circle(x+55,y+15,12);
	circle(x+55,y+15,11);
	circle(x+55,y+15,10);
	line(x+49,y+22,x+61,y+8);
	line(x+48,y+21,x+60,y+7);
	line(x+50,y+23,x+62,y+9);
	setcolor(DARKGRAY);                   /* Icono de "Directorio" o "ls". */
	setfillstyle(SOLID_FILL,YELLOW);
	rectangle(x+75,y+42,x+95,y+55);
	rectangle(x+75,y+39,x+85,y+42);
	floodfill(x+80,y+45,DARKGRAY);
	floodfill(x+80,y+40,DARKGRAY);
	setcolor(LIGHTGREEN);         /* Icono de Cambio de Prioridad o "Pri". */
	arc(x+50,y+45,90,270,7);
	line(x+50,y+38,x+50,y+52);
	setfillstyle(SOLID_FILL,LIGHTGREEN);
	floodfill(x+48,y+45,LIGHTGREEN);
	setcolor(WHITE);
	rectangle(x+50,y+38,x+57,y+52);
	setfillstyle(SOLID_FILL,WHITE);
	floodfill(x+53,y+45,WHITE);
	setcolor(LIGHTRED);
	arc(x+57,y+45,270,90,7);
	line(x+57,y+38,x+57,y+52);
	setfillstyle(SOLID_FILL,LIGHTRED);
	floodfill(x+59,y+45,LIGHTRED);
	setcolor(BLACK);
	settextstyle(SMALL_FONT,HORIZ_DIR,3);
	outtextxy(x+45,y+42,"P");
	outtextxy(x+52,y+39,"R");
	outtextxy(x+60,y+42,"I");
	setcolor(WHITE);                                       /* Icono de ps. */
	line(x+105,y+52,x+125,y+52);
	line(x+108,y+55,x+108,y+35);
	setcolor(LIGHTGREEN);
	rectangle(x+110,y+51,x+112,y+40);
	line(x+111,y+51,x+111,y+40);
	setcolor(LIGHTBLUE);
	rectangle(x+114,y+51,x+116,y+35);
	line(x+115,y+51,x+115,y+35);
	setcolor(LIGHTRED);
	rectangle(x+118,y+51,x+120,y+45);
	line(x+119,y+51,x+119,y+45);
	setcolor(LIGHTMAGENTA);
	rectangle(x+122,y+51,x+124,y+44);
	line(x+123,y+51,x+123,y+44);
	setcolor(WHITE);                    /* Icono de Velocidad del Sistema. */
	circle(x+175,y+15,10);
	line(x+175,y+5,x+175,y+8);
	line(x+165,y+15,x+168,y+15);
	line(x+185,y+15,x+182,y+15);
	line(x+170,y+11,x+167,y+8);
	line(x+180,y+11,x+182,y+8);
	line(x+180,y+19,x+182,y+20);
	line(x+170,y+19,x+167,y+20);
	setcolor(LIGHTRED);
	setlinestyle(SOLID_LINE,1,3);
	line(x+175,y+15,x+170,y+11);
	setlinestyle(SOLID_LINE,1,1);
	setcolor(LIGHTGREEN);               /* Icono de Prioridad del Sistema. */
	arc(x+20,y+45,90,270,7);
	line(x+20,y+38,x+20,y+52);
	setfillstyle(SOLID_FILL,LIGHTGREEN);
	floodfill(x+18,y+45,LIGHTGREEN);
	setcolor(WHITE);
	rectangle(x+20,y+38,x+27,y+52);
	setfillstyle(SOLID_FILL,WHITE);
	floodfill(x+23,y+45,WHITE);
	setcolor(LIGHTRED);
	arc(x+27,y+45,270,90,7);
	line(x+27,y+38,x+27,y+52);
	setfillstyle(SOLID_FILL,LIGHTRED);
	floodfill(x+29,y+45,LIGHTRED);
	setcolor(BLACK);
	settextstyle(SMALL_FONT,HORIZ_DIR,3);
	outtextxy(x+15,y+42,"P");
	outtextxy(x+22,y+39,"R");
	outtextxy(x+30,y+42,"I");
	setcolor(DARKGRAY);
	setlinestyle(SOLID_LINE,1,3);
	line(x+13,y+33,x+37,y+57);
	line(x+13,y+57,x+37,y+33);
	setlinestyle(SOLID_LINE,1,1);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : aviso.
Objetivo   : Mostrar un mensaje en la pantalla.
Par metros : char *str = Mensaje a presentar.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void aviso(char *str)
{  _show_hide_cur(HIDE);
	setcolor(LIGHTRED);
	setfillstyle(SOLID_FILL,LIGHTRED);
	rectangle(200,80,440,270);
	floodfill(320,175,LIGHTRED);
	setcolor(YELLOW);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	settextstyle(/*DEFAULT_FONT*/SIMPLEX_FONT,HORIZ_DIR,1);
	outtextxy(320,150,str);
	_show_hide_cur(SHOW);
	return;
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : error.
Objetivo   : Mostrar un mensaje de error en la pantalla.
Par metros : char *str = Mensaje de error a presentar.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void error(char *str)
{
	int x,y;
	aviso(str);
	boom();
	_def_cur_graph(_SMILY);
	_show_hide_cur(SHOW);
	while (!_check_button(&x,&y,0) && !kbhit());
	_show_hide_cur(HIDE);
	erase_window();
	_def_cur_graph(_FINGER);
	_show_hide_cur(SHOW);
	return;
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : draw_interface.
Objetivo   : Dibuja la pantalla del programa
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void draw_interface()
{
	int x,y,j,i;
	clearviewport();                           /* Borro pantalla gr fica. */
	setcolor(LIGHTGRAY);              /* Cajones de Registros y Banderas. */
	rectangle(205,10,505,70);
	rectangle(510,10,639,70);
	line(205,25,505,25);
	line(510,25,639,25);
	for (j=0,x=265;j<4;j++,x+=60)
		line(x,25,x,70);
	line(574,25,574,70);
	line(510,45,639,45);
	setfillstyle(SOLID_FILL,LIGHTGRAY);
	floodfill(355,15,LIGHTGRAY);
	floodfill(574,15,LIGHTGRAY);
	rectangle(206,25,504,45);
	setfillstyle(SOLID_FILL,LIGHTRED);
	for (j=0,x=205;j<7;j++,x+=60)
		floodfill(x+30,35,LIGHTGRAY);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	setcolor(LIGHTBLUE);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy(355,10,"Registros");
	outtextxy(574,10,"Banderas");
	setcolor(YELLOW);
	outtextxy(355,30,"AX        BX        CX        DX        EX");
	outtextxy(574,30,"ZF        SF");
	setcolor(LIGHTGRAY);             /* Dibujo de indicadores de recursos. */
	setfillstyle(SOLID_FILL,LIGHTGRAY);
	rectangle(10,75,190,230);
	line(10,90,190,90);
	floodfill(100,85,LIGHTGRAY);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,DARKGRAY);
	for(j=0,x=30;j<5;j++,x+=30)
		{
		arc(x+20,215,117,63,8);
		line(x+23,207,x+23,107);
		line(x+17,207,x+17,107);
		arc(x+20,107,0,180,3);
		line(x+17,107,x+23,107);
		floodfill(x+20,215,WHITE);
		}
	setcolor(LIGHTBLUE);
	outtextxy(100,75,"Recursos");
	setcolor(YELLOW);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	outtextxy(25,107,"100%");
	outtextxy(25,157," 50%");
	outtextxy(25,207,"  0%");
	setlinestyle(DOTTED_LINE,1,1);
	for (i=0,x=55;i<4;i++,x+=30)
		for (j=0,y=107;j<3;j++,y+=50)
			line(x,y,x+20,y);
	setlinestyle(SOLID_LINE,1,1);
	setcolor(LIGHTGRAY);                 /* Dibujo de ventana del usuario. */
	rectangle(195,75,445,275);
	rectangle(197,77,443,273);
	outtextxy(195, 320, "0b");                        /* Escala de la memoria */
	outtextxy(245, 320, "200");
	outtextxy(295, 320, "400");
	outtextxy(345, 320, "600");
	outtextxy(395, 320, "800");
	outtextxy(445, 320, "1000");
	line(195,315,195,310);
	line(245,315,245,310);
	line(295,315,295,310);
	line(345,315,345,310);
	line(395,315,395,310);
	line(445,315,445,310);
	arc(100,400,90,270,10); 	                       /* Dibujo del Ca¤¢n. */
	arc(112,410,0,180,12);
	line(100,390,130,390);
	line(124,410,130,410);
	line(130,390,130,410);
	line(130,390,133,386);
	line(133,386,133,414);
	line(133,414,130,410);
	setfillstyle(SOLID_FILL,LIGHTGRAY);
	floodfill(100,400,LIGHTGRAY);
	setcolor(BROWN);
	for (i=10;i>6;i--)
		circle(112,410,i);
	for (x=111;x<114;x++)
		line(x,404,x,418);
	for (y=409;y<412;y++)
		line(104,y,120,y);
	setcolor(YELLOW);
	circle(112,410,3);
	setfillstyle(SOLID_FILL,YELLOW);
	floodfill(112,410,YELLOW);
	setcolor(DARKGRAY);                     /* Dibujo de pila de procesos. */
	setfillstyle(SOLID_FILL,DARKGRAY);
	for (i=0,x=50,y=410;i<5;i++,x+=12)
		{
		if (i==3)
			{
			x = 56;
			y -= 10;
			}
		circle(x,y,5);
		floodfill(x,y,DARKGRAY);
		}
	y -= 10;
	circle(62,y,5);
	floodfill(62,y,DARKGRAY);
	setcolor(LIGHTRED);                             /* Dibujo del resorte. */
	setlinestyle(SOLID_LINE,1,3);
	line(475,430,495,430);
	line(475,440,495,440);
	line(478,430,493,440);
	line(478,440,493,430);
	setlinestyle(SOLID_LINE,1,1);
	setcolor(YELLOW);              	         /* Letrero del Edificio CPU. */
	setfillstyle(SOLID_FILL,YELLOW);
	for (i=0,y=80;i<2;i++,y+=30)
		for (x=520;x<631;x+=6)
			{
			circle(x,y,2);
			floodfill(x,y,YELLOW);
			}
	for (i=0,x=520;i<2;i++,x+=114)
		for (y=80;y<=110;y+=6)
			{
			circle(x,y,2);
			floodfill(x,y,YELLOW);
			}
	setcolor(LIGHTGREEN);
	settextstyle(SMALL_FONT,HORIZ_DIR,5);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy(578,85,"Edificio CPU");
	setcolor(DARKGRAY);
	setfillstyle(SOLID_FILL,DARKGRAY);
	rectangle(573,y,577,y+10);
	floodfill(575,y+5,DARKGRAY);
	setcolor(LIGHTGRAY);                 /* Cajas con Leyendas de estados. */
	rectangle(20,250,40,270);
	rectangle(20,280,40,300);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	outtextxy(90,255,"Estados Activos");
	outtextxy(104,285,"Estados Suspendidos");
	rectangle(519,y+11,635,430);           /* Estructura del Edificio CPU. */
	rectangle(567,430,587,390);                                 /* Puerta. */
	door(CLOSE);
	setcolor(LIGHTGRAY);
	rectangle(519,295,537,320);                /* Ventana de "Bloqueados". */
	rectangle(519,210,537,235);            /* Ventana "Suspendido Listos". */
	rectangle(519,155,537,180);        /* Ventana "Suspendido Bloqueados". */
	line(519,280,635,280);
	setfillstyle(SOLID_FILL,LIGHTGRAY);
	floodfill(577,355,LIGHTGRAY);
	floodfill(30,260,LIGHTGRAY);
	setfillstyle(SOLID_FILL,DARKGRAY);
	floodfill(577,250,LIGHTGRAY);
	floodfill(30,290,LIGHTGRAY);
	write_instruc("Edificio CPU");
	setcolor(RED);			                 	    /* Letreros de Listas, etc. */
	outtextxy(577,370,"Ejecucion");
	line(595,373,596,371);
	outtextxy(586,300,"Bloqueados");
	setcolor(LIGHTMAGENTA);
	outtextxy(586,210,"Suspendido");
	outtextxy(586,220,"Listos");
	outtextxy(586,155,"Suspendido");
	outtextxy(586,165,"Bloqueados");
	setcolor(WHITE);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	outtextxy(55,360,"Lista de Listos");
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : erase_window.
Objetivo   : Borrar la ventana est ndar.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void erase_window()
{
	_show_hide_cur(HIDE);
	setviewport(198,78,442,272,1);
	clearviewport();
	setviewport(0,0,getmaxx(),getmaxy(),1);
	_show_hide_cur(SHOW);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : dialog_box.
Objetivo   : Pone la cadena y los attributos listos para pedir datos.
Par metros : char *str = Cadena de t¡tulo de la caja,
				 char *str2 = Cadena de petici¢n (si es '\0' no imprimo
								  mensaje).
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void dialog_box(char *str,char *str2)
{
	setcolor(LIGHTRED);
	settextstyle(/*DEFAULT_FONT*/BOLD_FONT,HORIZ_DIR,1);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy(320,77,str);
	setcolor(YELLOW);                                 /* Flecha indicadora */
	line(260,195,285,195);
	line(260,205,285,205);
	line(260,195,260,205);
	line(285,195,285,190);
	line(285,190,305,200);
	line(305,200,285,210);
	line(285,210,285,205);
	setfillstyle(LTSLASH_FILL,YELLOW);
	floodfill(280,200,YELLOW);
	if (*str2 != '\0')
		{
		settextstyle(/*DEFAULT_FONT*/TRIPLEX_SCR_FONT,HORIZ_DIR,1);
		settextjustify(LEFT_TEXT,TOP_TEXT);
		setcolor(GREEN);
		outtextxy(210,150,str2);
		}
	gotoxy(40,13);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : door.
Objetivo   : Abre o cierra la puerta del Edificio CPU.
Par metros : int i = 1 si se abre, 0 si se cierra.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void door(int i)
{
	_show_hide_cur(HIDE);
	setcolor(LIGHTGRAY);
	rectangle(567,430,587,390);
	if (i)                                              /* Abre la puerta. */
		{
		setfillstyle(SOLID_FILL,BLACK);
		floodfill(577,410,LIGHTGRAY);
		}
	else                                              /* Cierra la puerta. */
		{
		setfillstyle(SOLID_FILL,BROWN);
		floodfill(577,410,LIGHTGRAY);
		setcolor(YELLOW);
		circle(583,410,1);
		setfillstyle(SOLID_FILL,YELLOW);
		floodfill(583,410,YELLOW);
		}
	_show_hide_cur(SHOW);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : write_instruc.
Objetivo   : Escribe la instrucci¢n que se est  ejecutando en el Edificio
				 CPU.
Par metros : char *str = Instrucci¢n a imprimir.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void write_instruc(char *str)
{
	_show_hide_cur(HIDE);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	setfillstyle(SOLID_FILL,BLUE);
	setcolor(WHITE);
	setviewport(520,345,634,365,1);
	clearviewport();
	floodfill(10,10,YELLOW);
	outtextxy(57,3,str);
	setviewport(0,0,getmaxx(),getmaxy(),1);
	_show_hide_cur(SHOW);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : move_proc.
Objetivo   : Anima el proceso a donde vaya (seg£n el par metro que se pase).
Par metros : int destiny = Par metro de rumbo y direcci¢n.
				 int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void move_proc(int destiny,int proc)
{
	switch(destiny)
		{
		case LISTO_2_EXEC:
			anim_lae(proc);
			break;
		case EXEC_2_LISTO:
			anim_eal(proc);
			break;
		case LISTO_2_SLISTO:
			anim_lasl(proc);
			break;
		case SLISTO_2_LISTO:
			anim_slal(proc);
			break;
		case EXEC_2_SLISTO:
			anim_easl(proc);
			break;
		case LISTO_2_BLOQ:
			anim_lab(proc);
			break;
		case BLOQ_2_LISTO:
			anim_bal(proc);
			break;
		case SLISTO_2_SBLOQ:
			anim_slasb(proc);
			break;
		case SBLOQ_2_SLISTO:
			anim_sbasl(proc);
			break;
		case EXEC_2_BLOQ:
			anim_eab(proc);
			break;
		case BLOQ_2_SBLOQ:
			anim_basb(proc);
			break;
		case SBLOQ_2_BLOQ:
			anim_sbab(proc);
			break;
		}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_sbasl.
Objetivo   : Anima el proceso de suspendidos bloqueados a suspendidos
				 listos.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_sbasl(int proc)
{
	void *imagine;
	draw_bullet(528,170,proc);
	imagine = (void *) malloc(imagesize(521,163,535,177));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(521,163,535,177,imagine);
	move_bullet(521,163,500,163,-1,0,&imagine,0);
	setcolor(LIGHTGRAY);
	line(519,155,519,185);
	move_bullet(500,163,478,183,-1,1,&imagine,0);
	move_bullet(478,183,478,413,0,1,&imagine,0);
	move_bullet(478,413,478,235,0,-1,&imagine,0);
	move_bullet(478,235,521,215,1,-1,&imagine,1);
	line(519,210,519,235);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_slasb.
Objetivo   : Anima el proceso de suspendidos listos a suspendidos
				 bloqueados.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_slasb(int proc)
{
	void *imagine;
	draw_bullet(528,222,proc);
	imagine = (void *) malloc(imagesize(521,215,535,229));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(521,215,535,229,imagine);
	move_bullet(521,215,478,215,-1,0,&imagine,0);
	setcolor(LIGHTGRAY);
	line(519,210,519,235);
	move_bullet(478,215,478,413,0,1,&imagine,0);
	move_bullet(478,413,478,190,0,-1,&imagine,0);
	move_bullet(478,190,500,160,1,-1,&imagine,0);
	move_bullet(500,160,521,160,1,0,&imagine,1);
	line(519,155,519,185);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_lab.
Objetivo   : Anima el proceso de listos a bloqueados.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_lab(int proc)
{
	void *imagine;
	boom();
	draw_bullet(145,400,proc);
	imagine = (void *) malloc(imagesize(138,393,152,407));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(138,393,152,407,imagine);
	move_bullet(138,393,450,393,2,0,&imagine,0);
	move_bullet(450,393,478,413,2,1,&imagine,0);
	move_bullet(478,413,478,325,0,-1,&imagine,0);
	move_bullet(478,325,500,300,1,-1,&imagine,0);
	move_bullet(500,300,521,300,1,0,&imagine,1);
	setcolor(LIGHTGRAY);
	line(519,290,519,335);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_bal.
Objetivo   : Anima el proceso de listos a bloqueados.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_bal(int proc)
{
	void *imagine;
	draw_bullet(528,305,proc);
	imagine = (void *) malloc(imagesize(521,298,535,312));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(521,298,535,312,imagine);
	move_bullet(521,298,498,298,-1,0,&imagine,0);
	setcolor(LIGHTGRAY);
	line(519,290,519,335);
	move_bullet(498,298,478,318,-1,1,&imagine,0);
	move_bullet(478,318,478,413,0,1,&imagine,0);
	move_bullet(478,413,430,400,-2,-1,&imagine,0);
	move_bullet(430,400,420,400,-2,0,&imagine,0);
	move_bullet(420,400,340,443,-2,1,&imagine,0);
	move_bullet(340,443,54,443,-2,0,&imagine,0);
	move_bullet(54,443,54,420,0,-1,&imagine,1);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_lae.
Objetivo   : Anima el proceso de listos a ejecuci¢n.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_lae(int proc)
{
	void *imagine;
	boom();
	draw_bullet(145,400,proc);
	imagine = (void *) malloc(imagesize(138,393,152,407));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(138,393,152,407,imagine);
	move_bullet(138,393,506,393,2,0,&imagine,1);
	door(OPEN);
	putimage(570,393,imagine,COPY_PUT);
	delay(150);
	door(CLOSE);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_eab.
Objetivo   : Anima el proceso de ejecuci¢n a bloqueados.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_eab(int proc)
{
	void *imagine;
	door(OPEN);
	draw_bullet(577,450,proc);
	delay(150);
	imagine = (void *) malloc(imagesize(570,443,584,457));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(570,443,584,457,imagine);
	door(CLOSE);
	move_bullet(570,443,430,443,-2,0,&imagine,0);
	move_bullet(430,443,450,400,1,-1,&imagine,0);
	move_bullet(450,400,478,413,2,1,&imagine,0);
	move_bullet(478,413,478,325,0,-1,&imagine,0);
	move_bullet(478,325,500,300,1,-1,&imagine,0);
	move_bullet(500,300,521,300,1,0,&imagine,1);
	setcolor(LIGHTGRAY);
	line(519,290,519,335);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_eal.
Objetivo   : Anima el proceso de ejecuci¢n a listos.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_eal(int proc)
{
	void *imagine;
	door(OPEN);
	draw_bullet(577,450,proc);
	delay(150);
	imagine = (void *) malloc(imagesize(570,443,584,457));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(570,443,584,457,imagine);
	door(CLOSE);
	move_bullet(570,443,54,443,-2,0,&imagine,0);
	move_bullet(54,443,54,420,0,-1,&imagine,1);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_slal.
Objetivo   : Anima el proceso de suspendidos listos a listos.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_slal(int proc)
{
	void *imagine;
	draw_bullet(528,222,proc);
	imagine = (void *) malloc(imagesize(521,215,535,229));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(521,215,535,229,imagine);
	move_bullet(521,215,478,215,-1,0,&imagine,0);
	setcolor(LIGHTGRAY);
	line(519,210,519,235);
	move_bullet(478,215,478,413,0,1,&imagine,0);
	move_bullet(478,413,430,400,-2,-1,&imagine,0);
	move_bullet(430,400,420,400,-2,0,&imagine,0);
	move_bullet(420,400,340,443,-2,1,&imagine,0);
	move_bullet(340,443,54,443,-2,0,&imagine,0);
	move_bullet(54,443,54,420,0,-1,&imagine,1);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_lasl.
Objetivo   : Anima el proceso de listos a suspendidos listos.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_lasl(int proc)
{
	void *imagine;
	boom();
	draw_bullet(145,400,proc);
	imagine = (void *) malloc(imagesize(138,393,152,407));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(138,393,152,407,imagine);
	move_bullet(138,393,450,393,2,0,&imagine,0);
	move_bullet(450,393,478,413,2,1,&imagine,0);
	move_bullet(478,413,478,240,0,-1,&imagine,0);
	move_bullet(478,240,500,215,1,-1,&imagine,0);
	move_bullet(500,215,521,215,1,0,&imagine,1);
	setcolor(LIGHTGRAY);
	line(519,210,519,235);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_easl.
Objetivo   : Anima el proceso de ejecuci¢n a suspendidos listos.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_easl(int proc)
{
	void *imagine;
	door(OPEN);
	draw_bullet(577,450,proc);
	delay(150);
	imagine = (void *) malloc(imagesize(570,443,584,457));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(570,443,584,457,imagine);
	door(CLOSE);
	move_bullet(570,443,430,443,-2,0,&imagine,0);
	move_bullet(430,443,450,400,1,-1,&imagine,0);
	move_bullet(450,400,478,413,2,1,&imagine,0);
	move_bullet(478,413,478,240,0,-1,&imagine,0);
	move_bullet(478,240,500,215,1,-1,&imagine,0);
	move_bullet(500,215,521,215,1,0,&imagine,1);
	setcolor(LIGHTGRAY);
	line(519,210,519,235);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_sbab.
Objetivo   : Anima el proceso de suspendidos bloqueados a bloqueados.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_sbab(int proc)
{
	void *imagine;
	draw_bullet(528,170,proc);
	imagine = (void *) malloc(imagesize(521,163,535,177));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(521,163,535,177,imagine);
	move_bullet(521,163,500,163,-1,0,&imagine,0);
	setcolor(LIGHTGRAY);
	line(519,155,519,185);
	move_bullet(500,163,478,183,-1,1,&imagine,0);
	move_bullet(478,183,478,413,0,1,&imagine,0);
	move_bullet(478,413,478,325,0,-1,&imagine,0);
	move_bullet(478,325,500,300,1,-1,&imagine,0);
	move_bullet(500,300,521,300,1,0,&imagine,1);
	setcolor(LIGHTGRAY);
	line(519,290,519,335);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : anim_basb.
Objetivo   : Anima el proceso de bloqueados a suspendidos bloqueados.
Par metros : int proc = Identificador de proceso.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void anim_basb(int proc)
{
	void *imagine;
	draw_bullet(528,305,proc);
	imagine = (void *) malloc(imagesize(521,298,535,312));
	if (!imagine)
		{
		error("Memoria Insuficiente");
		return;
		}
	getimage(521,298,535,312,imagine);
	move_bullet(521,298,498,298,-1,0,&imagine,0);
	setcolor(LIGHTGRAY);
	line(519,290,519,335);
	move_bullet(498,298,478,318,-1,1,&imagine,0);
	move_bullet(478,318,478,413,0,1,&imagine,0);
	move_bullet(478,413,478,190,0,-1,&imagine,0);
	move_bullet(478,190,500,160,1,-1,&imagine,0);
	move_bullet(500,160,521,160,1,0,&imagine,1);
	line(519,155,519,185);
	free(imagine);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : draw_bullet.
Objetivo   : Dibuja la bala con el proceso.
Par metros : int x,y : Coordenadas de la bala,
				 int proc : N£mero del proceso a pintar.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void draw_bullet(int x,int y,int proc)
{
	char id[3];
	sprintf(id,"%d",proc);
	setcolor(DARKGRAY);
	setfillstyle(SOLID_FILL,DARKGRAY);
	circle(x,y,5);
	floodfill(x,y,DARKGRAY);
	setcolor(WHITE);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	settextstyle(SMALL_FONT,HORIZ_DIR,3);
	outtextxy(x,y-1,id);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : move_bullet.
Objetivo   : Mueve la bala con el proceso.
Par metros : int x,y : Coordenadas de la primera bala,
				 int x1,y1 : Coordenadas de la £ltima bala,
				 int incx,incy : Desplazamientos en x y y,
				 void **imagine : N£mero del proceso a pintar,
				 int borra : 1 - Borra el bullet, 0 no.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void move_bullet(int x,int y,int x1,int y1,int incx,int incy,void **imagine,
																						int borra)
{
	register int i,j;
	for (i=x,j=y;i!=x1||j!=y1;)
		{
		if(_check_button(&__x, &__y,0))
			ratonazo = 1;
		putimage(i,j,*imagine,COPY_PUT);
		delay(VEL);
		if (i!=x1)
			i+=incx;
		if (j!=y1)
			j+=incy;
		if(_check_button(&__x, &__y,0))
			ratonazo = 1;
		}
	if (borra)
		{
		setviewport(x1,y1,x1+12,y1+13,1);
		clearviewport();
		setviewport(0,0,getmaxx(),getmaxy(),1);
		}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : paint_recursos.
Objetivo   : Mueve el nivel de los recursos en los term¢metros indicadores.
Par metros : int recurso = No. del recurso a manipular, y
				 int cantidad = Cuanto le voy a a¤adir o a quitar (n£mero
									 negativo indica un GET, el FRE se indica con uno
									 positivo).
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void paint_recursos(int recurso,int cantidad)
{
	int cant = cantidad;
	_show_hide_cur(HIDE);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,DARKGRAY);
	switch(recurso)
		{
		case 0:
			line(47,r1-cant,53,r1-cant);
			floodfill(50,r1-cant+5,WHITE);
			if (cantidad < 0)
				{
				setfillstyle(SOLID_FILL,BLACK);
				floodfill(50,r1-cant-5,WHITE);
				setcolor(BLACK);
				}
			else
				setcolor(DARKGRAY);
			line(48,r1,52,r1);
			r1 -= cant;
			break;
		case 1:
			line(77,r2-cant,83,r2-cant);
			floodfill(80,r2-cant+5,WHITE);
			if (cantidad < 0)
				{
				setfillstyle(SOLID_FILL,BLACK);
				floodfill(80,r2-cant-5,WHITE);
				setcolor(BLACK);
				}
			else
				setcolor(DARKGRAY);
			line(78,r2,82,r2);
			r2 -= cant;
			break;
		case 2:
			line(107,r3-cant,113,r3-cant);
			floodfill(110,r3-cant+5,WHITE);
			if (cantidad < 0)
				{
				setfillstyle(SOLID_FILL,BLACK);
				floodfill(110,r3-cant-5,WHITE);
				setcolor(BLACK);
				}
			else
				setcolor(DARKGRAY);
			line(108,r3,112,r3);
			r3 -= cant;
			break;
		case 3:
			line(137,r4-cant,143,r4-cant);
			floodfill(140,r4-cant+5,WHITE);
			if (cantidad < 0)
				{
				setfillstyle(SOLID_FILL,BLACK);
				floodfill(140,r4-cant-5,WHITE);
				setcolor(BLACK);
				}
			else
				setcolor(DARKGRAY);
			line(138,r4,142,r4);
			r4 -= cant;
			break;
		case 4:
			line(167,r5-cant,173,r5-cant);
			floodfill(170,r5-cant+5,WHITE);
			if (cantidad < 0)
				{
				setfillstyle(SOLID_FILL,BLACK);
				floodfill(170,r5-cant-5,WHITE);
				setcolor(BLACK);
				}
			else
				setcolor(DARKGRAY);
			line(168,r5,172,r5);
			r5 -= cant;
			break;
		}
	_show_hide_cur(SHOW);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : paint_registers.
Objetivo   : Escribe el contenido de los registros en la pantalla.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void paint_registers()
{
	int i,x;
	char valor[7];
	setcolor(WHITE);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	for (i=0,x=200;i<5;i++,x+=60)
		{
		setviewport(x+8,48,x+62,67,1);
		clearviewport();
		sprintf(valor,"%d",regs[i]);
		outtextxy(39,2,valor);
		setviewport(0,0,getmaxx(),getmaxy(),1);
		}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : paint_flags.
Objetivo   : Escribe el contenido de las banderas en la pantalla.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void paint_flags()
{
	int i,x;
	char valor[2];
	setcolor(WHITE);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	for (i=0,x=510;i<2;i++,x+=64)
		{
		setviewport(x+3,48,x+61,67,1);
		clearviewport();
		if (i)
			sprintf(valor,"%d",ZF);
		else
			sprintf(valor,"%d",SF);
		outtextxy(39,2,valor);
		setviewport(0,0,getmaxx(),getmaxy(),1);
		}
}

/* -------------------------------------------------------------------------
FUNCION   : paint_line_mem
OBJETIVO  : Pintar una linea de pixeles del  rea de memoria
PARAMETROS: int pos -> Posici¢n en x
				PCB *v_ll -> Vac¡o (0) o lleno (otro).
SALIDA    : Ninguna
------------------------------------------------------------------------- */
void paint_line_mem(int pos, PCB *v_ll)
{  int color = LIGHTGRAY;
	register int i, y = 295;

	if(!v_ll) color = LIGHTCYAN;
	for(i=0; i<10; i++)
		putpixel(pos, y+i, color);
}

/* -------------------------------------------------------------------------
FUNCION   : paint_mem
OBJETIVO  : Pintar en el  rea de memoria una parte llena o vac¡a
PARAMETROS: int pos -> Posici¢n en la memoria (0 - 1000)
				PCB *v_ll-> Vac¡o (NULL) o lleno (otro).
				longitud -> longitud del  rea.
SALIDA    : Ninguna
------------------------------------------------------------------------- */
void paint_mem(int pos, PCB *v_ll, int longitud)
{	int n_pixeles, pos_inic;
	register i;

	n_pixeles = longitud/BxP;
	if(longitud % BxP)
		n_pixeles++;
	pos_inic = 195+(pos/BxP);
	_show_hide_cur(HIDE);
	for(i=0; i<n_pixeles; i++)
		paint_line_mem(pos_inic + i, v_ll);
	_show_hide_cur(SHOW);
}