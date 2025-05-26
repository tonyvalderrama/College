/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
							  INSTITUTO TECNOLOGICO DE MOREIA
											Bases de Datos

Archivo    : GUIBTREE.C
Objetivo   : Una serie de rutinas para la interface de un sistema que
				 muestre como funcionan los  rboles B (B-Tree).
Dise¤o     : Jos‚ Antonio Valderrama Ramos
				 Jorge E. Aparicio Mayorga.
Implemento : Los mismos.
Versi¢n    : 1.00 (Marzo 1994).
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include "mouse.h"
#include "button.h"
#include "gui.h"

void ini_gui(),dibuja_boton(int,int),dibuja_arbolB(),imprimef(char *);
void escribe_llave(int,int,int),borra_ventana();
int que_hice();

const int BUTTON_PRESS = 250;

const int DOWN = 0;
const int UP   = 1;

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : ini_gui.
Objetivo   : Inicializa el sistema.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void ini_gui()
{
	int driver=VGA,mode=VGAHI;

	initgraph(&driver,&mode,"");           /* Inicializo modo gr fico VGA. */
	if (graphresult())
		{
		printf("\nNo puedo inicializar modo gr fico, perdone.\n");
		exit(0);
		}

	if (!_inic_mouse())                            /* Inicializo el mouse. */
		{
		printf("Perdone, el programa requiere de un mouse.\n");
		printf("Revise si est  cargado el driver, y si todo est  en orden\n");
		printf("y vuelva a correr el programa.  Gracias.\n");
		exit (1);
		}
      
	dibuja_boton(ALTA,UP);              /* Dibujo los botones de comandos. */
	dibuja_boton(BAJA,UP);
	dibuja_boton(CONS,UP);
	dibuja_boton(SALI,UP);

	dibuja_arbolB();                    /* Dibujo el  rbol B+ en pantalla. */

	setcolor(WHITE);                /* Dibujo el cuadro para los mensajes. */
	rectangle(5,360,getmaxx()-5,getmaxy()-5);

	_def_cur_graph(_SMILY);     /* Defino el cursor del mouse y lo prendo. */
	_show_hide_cur(SHOW);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : dibuja_arbolB.
Objetivo   : Dibuja el  rbol B+.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void dibuja_arbolB()
{
	_show_hide_cur(HIDE);
	setcolor(MAGENTA);
	rectangle(5,45,getmaxx()-5,355);

	setfillstyle(SOLID_FILL,BLACK);
	floodfill(getmaxx()/2,200,MAGENTA);

	_draw_button(getmaxx()/2-84,50,getmaxx()/2-54,90,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()/2-38,50,getmaxx()/2-8,90,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()/2+8,50,getmaxx()/2+38,90,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()/2+54,50,getmaxx()/2+84,90,3,LIGHTGRAY,DARKGRAY,WHITE,1);

	_draw_button(26,150,56,190,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(72,150,102,190,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(118,150,148,190,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(164,150,194,190,3,LIGHTGRAY,DARKGRAY,WHITE,1);

	_draw_button(getmaxx()-194,150,getmaxx()-164,190,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()-148,150,getmaxx()-118,190,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()-102,150,getmaxx()-72,190,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()-56,150,getmaxx()-26,190,3,LIGHTGRAY,DARKGRAY,WHITE,1);

	_draw_button(91,230,121,270,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(137,230,167,270,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(183,230,213,270,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(229,230,259,270,3,LIGHTGRAY,DARKGRAY,WHITE,1);

	_draw_button(getmaxx()-259,230,getmaxx()-229,270,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()-213,230,getmaxx()-183,270,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()-167,230,getmaxx()-137,270,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()-121,230,getmaxx()-91,270,3,LIGHTGRAY,DARKGRAY,WHITE,1);

	_draw_button(getmaxx()/2-84,310,getmaxx()/2-54,350,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()/2-38,310,getmaxx()/2-8,350,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()/2+8,310,getmaxx()/2+38,350,3,LIGHTGRAY,DARKGRAY,WHITE,1);
	_draw_button(getmaxx()/2+54,310,getmaxx()/2+84,350,3,LIGHTGRAY,DARKGRAY,WHITE,1);

	setcolor(LIGHTRED);
	setlinestyle(SOLID_LINE,1,THICK_WIDTH);
	line(getmaxx()/2-92,70,110,145);
	line(getmaxx()/2+92,70,getmaxx()-110,145);
	line(getmaxx()/2-46,70,250,225);
	line(getmaxx()/2+46,70,getmaxx()-250,225);
	line(getmaxx()/2,70,getmaxx()/2,305);

	_show_hide_cur(SHOW);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : escribe_llave.
Objetivo   : Escribe las llaves en el nodo correspondiente.
Par metros : int nodo  = El nodo del  rbol en pantalla (0,1,2,3,4 o 5).
				 int pos   = La posici¢n en el nodo (0,1,2 o 3).
				 int llave = Indice a pintar.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void escribe_llave(int nodo,int pos,int llave)
{
	int x,y;
	char str[4];
	setcolor(LIGHTBLUE);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	if (!nodo)
		{
		y = 70;
		switch(pos)
			{
			case 0:
				x = getmaxx()/2 - 69;
				break;
			case 1:
				x = getmaxx()/2 - 23;
				break;
			case 2:
				x = getmaxx()/2 + 23;
				break;
			case 3:
				x = getmaxx()/2 + 69;
				break;
			}
		}
	else if (nodo == 1)
		{
		y = 170;
		switch(pos)
			{
			case 0:
				x = 41;
				break;
			case 1:
				x = 87;
				break;
			case 2:
				x = 133;
				break;
			case 3:
				x = 179;
				break;
			}
		}
	else if (nodo == 2)
		{
		y = 250;
		switch(pos)
			{
			case 0:
				x = 106;
				break;
			case 1:
				x = 152;
				break;
			case 2:
				x = 198;
				break;
			case 3:
				x = 244;
				break;
			}
		}
	else if (nodo == 3)
		{
		y = 330;
		switch(pos)
			{
			case 0:
				x = getmaxx()/2 - 69;
				break;
			case 1:
				x = getmaxx()/2 - 23;
				break;
			case 2:
				x = getmaxx()/2 + 23;
				break;
			case 3:
				x = getmaxx()/2 + 69;
				break;
			}
		}
	else if (nodo == 4)
		{
		y = 250;
		switch(pos)
			{
			case 0:
				x = getmaxx() - 244;
				break;
			case 1:
				x = getmaxx() - 198;
				break;
			case 2:
				x = getmaxx() - 152;
				break;
			case 3:
				x = getmaxx() - 106;
				break;
			}
		}
	else if (nodo == 5)
		{
		y = 170;
		switch(pos)
			{
			case 0:
				x = getmaxx() - 179;
				break;
			case 1:
				x = getmaxx() - 133;
				break;
			case 2:
				x = getmaxx() - 87;
				break;
			case 3:
				x = getmaxx() - 41;
				break;
			}
		}
	itoa(llave,str,10);
	_show_hide_cur(HIDE);
	outtextxy(x,y,str);
	_show_hide_cur(SHOW);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : dibuja_boton.
Objetivo   : Dibuja los botones de la interface.
Par metros : int boton = Indica el bot¢n a dibujar.
				 int pos   = Indica si est  presionado o no.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void dibuja_boton(int boton,int pos)
{
	int color = getcolor();
	settextstyle(SMALL_FONT,HORIZ_DIR,5);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	_show_hide_cur(HIDE);
	switch(boton)
		{
		case 1:
			if (pos == UP)
				{
				_draw_button(105,5,195,25,3,LIGHTGRAY,DARKGRAY,WHITE,1);
				setcolor(BLACK);
				outtextxy(150,13,"Altas");
				}
			else
				{
				_draw_button(105,5,195,25,3,LIGHTGRAY,DARKGRAY,WHITE,0);
				setcolor(WHITE);
				outtextxy(150,13,"Altas");
				sound(750);
				delay(BUTTON_PRESS);
				nosound();
				dibuja_boton(ALTA,UP);
				}
			break;
		case 2:
			if (pos == UP)
				{
				_draw_button(205,5,295,25,3,LIGHTGRAY,DARKGRAY,WHITE,1);
				setcolor(BLACK);
				outtextxy(250,13,"Bajas");
				}
			else
				{
				_draw_button(205,5,295,25,3,LIGHTGRAY,DARKGRAY,WHITE,0);
				setcolor(WHITE);
				outtextxy(250,13,"Bajas");
				sound(750);
				delay(BUTTON_PRESS);
				nosound();
				dibuja_boton(BAJA,UP);
				}
			break;
		case 3:
			if (pos == UP)
				{
				_draw_button(305,5,395,25,3,LIGHTGRAY,DARKGRAY,WHITE,1);
				setcolor(BLACK);
				outtextxy(350,13,"Consultas");
				}
			else
				{
				_draw_button(305,5,395,25,3,LIGHTGRAY,DARKGRAY,WHITE,0);
				setcolor(WHITE);
				outtextxy(350,13,"Consultas");
				sound(750);
				delay(BUTTON_PRESS);
				nosound();
				dibuja_boton(CONS,UP);
				}
			break;
		case 10:
			if (pos == UP)
				{
				_draw_button(405,5,495,25,3,LIGHTGRAY,DARKGRAY,WHITE,1);
				setcolor(BLACK);
				outtextxy(450,13,"Salir");
				}
			else
				{
				_draw_button(405,5,495,25,3,LIGHTGRAY,DARKGRAY,WHITE,0);
				setcolor(WHITE);
				outtextxy(450,13,"Salir");
				sound(750);
				delay(BUTTON_PRESS);
				nosound();
				dibuja_boton(SALI,UP);
				}
			break;
		}
	_show_hide_cur(SHOW);
	setcolor(color);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : que_hice.
Objetivo   : Busca el lugar de la pantalla en el que estoy y regreso un
				 valor que indica la acci¢n que quiero hacer.
Par metros : Ninguno.
Salida     : Un entero que inidca la acci¢n realizada:
						 0 = No hice nada.
						 1 = Altas.
						 2 = Bajas.
						 3 = Consultas.
						 4 = Nodo 0 (Nodo Padre).
						 5 = Nodo 1 (1er hijo).
						 6 = Nodo 2.
						 7 = Nodo 3.
						 8 = Nodo 4.
						 9 = Nodo 5.
						 10= Salir.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
int que_hice()
{
	int x,y;
	if (!_pos_status(&x,&y))
		return 0;
	if (y>5 && y<25)
		{
		if (x>105 && x<195)
			{
			dibuja_boton(ALTA,DOWN);
			return ALTA;
			}
		if (x>205 && x<295)
			{
			dibuja_boton(BAJA,DOWN);
			return BAJA;
			}
		if (x>305 && x<395)
			{
			dibuja_boton(CONS,DOWN);
			return CONS;
			}
		if (x>405 && x<495)
			{
			dibuja_boton(SALI,DOWN);
			return SALI;
			}
		}
	else if (y>150 && y<190)
		{
		if (x>10 && x<210)
			return 5;
		if (x>getmaxx()-210 && x<getmaxx()-10)
			return 9;
		}
	else if (y>230 && y<270)
		{
		if (x>75 && x<275)
			return 6;
		if (x>getmaxx()-275 && x<getmaxx()-75)
			return 8;
		}
	else if (x>getmaxx()/2-100 && x<getmaxx()/2+100)
		if (y>310 && y<350)
			return 7;
		if (y>50 && y<90)
			return 4;
	return 0;
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : borra_ventana.
Objetivo   : Limpia el  rea de mensajes.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void borra_ventana()
{
	setfillstyle(SOLID_FILL,BLACK);
	_show_hide_cur(HIDE);
	floodfill(getmaxx()/2,370,WHITE);
	_show_hide_cur(SHOW);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : error.
Objetivo   : Despliega un mensaje de error.
Par metros : char *str = Cadena de error a desplegar.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void error(char *str)
{
	setfillstyle(SOLID_FILL,LIGHTRED);
	setcolor(YELLOW);
	_show_hide_cur(HIDE);
	floodfill(getmaxx()/2,370,WHITE);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy(getmaxx()/2,390,str);
	_show_hide_cur(SHOW);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : imprimef.
Objetivo   : Despliega un mensaje en la ventana de mensajes al usuario.
Par metros : char *str = Cadena de error a desplegar.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void imprimef(char *str)
{
	int x=getmaxx()/2,y=370;
	settextjustify(CENTER_TEXT,TOP_TEXT);
	settextstyle(SMALL_FONT,HORIZ_DIR,5);
	setcolor(LIGHTMAGENTA);
	_show_hide_cur(HIDE);
	outtextxy(x,y,str);
	_show_hide_cur(SHOW);
}
