/* -------------------------------------------------------------------------
	Funciones para el manejo de Memoria de Video
	Archivo de Funciones  ........................................ VIDEO.C
Dise§o:Jorge Eduardo Aparicio Mayorga
Implementaci¢n: Jorge Eduardo Aparicio Mayorga
Versi¢n: Septiembre de 1993.

------------------------------------------------------------------------- */
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include "video.h"

char far *_VIDEO;
/* -------------------------------------------------------------------------
FUNCION: inic_vid()
Objetivo: Inicializar la variable VIDEO de acuerdo al tipo de monitor.
Par†metros: Ninguno
Salida: Ninguna
------------------------------------------------------------------------- */
void inic_vid()
{	int vmode;
	union REGS r;
	r.h.ah = 15;
	vmode = int86(0x10, &r, &r)&255;
	if (vmode==7) _VIDEO=(char far *) MK_FP(0xB000,0000);
	else _VIDEO=(char far *) MK_FP(0xB800,0000);;
}

/* -------------------------------------------------------------------------
FUNCION: escribe_car
Objetivo: Desplegar un mismo caracter n veces
Par†metros: char x1, y1: Posici¢n inicial
				char car: 	 Caracter a desplegar.
				char atr: 	 Atributo con el que se habr† de imprimir
				int cuantos: N£mero de impresiones
------------------------------------------------------------------------- */
void escribe_car(x1, y1, car, cuantos, atr)
char x1, y1, car, atr;
int cuantos;
{	register int i;
	char far *v;

	v = POSXY(_VIDEO, x1, y1);
	for (i=0; i<cuantos; i++)
	{	*v++ = car;
		*v++ = atr;
	}
}
/* -------------------------------------------------------------------------
FUNCION: borra_screen
Objetivo: Borrar la pantalla completa
------------------------------------------------------------------------- */
void borra_screen()
{	register int i;
	char far *v = _VIDEO;

	for (i=0; i<MAXSCREEN; i++)
	{	*v++ = ' ';
		*v++ = _NORMAL;
	}
}
/* -------------------------------------------------------------------------
FUNCION: dis_cursor
Objetivo: Desaparecer el cursor
Par†metros: CURSOR *cur: Puntero a estructura para guardar informaci¢n
				sobre el estado actual del cursor
------------------------------------------------------------------------- */
void dis_cursor(cur)
CURSOR *cur;
{	union REGS regs;

	regs.h.ah=0x03;
	regs.h.bh=0;
	int86(INTVID, &regs, &regs);
	cur->stline = regs.h.ch;
	cur->endline = regs.h.cl;
	regs.h.ah=0x01;
	regs.h.ch=0x20;
	int86(INTVID, &regs, &regs);
}
/* -------------------------------------------------------------------------
FUNCION: app_cursor
Objetivo: Aparecer el cursor
Par†metros: CURSOR *cur: Puntero a estructura con informaci¢n sobre el
				estado del cursor
------------------------------------------------------------------------- */
void app_cursor(cur)
CURSOR *cur;
{  union REGS regs;
	regs.h.ah=0x01;
	regs.h.ch=cur->stline;
	regs.h.cl=cur->endline;
	int86(INTVID, &regs, &regs);
}
/* -------------------------------------------------------------------------
FUNCION: ventana
Objetivo: Desplegar una ventana en la pantalla
Par†metros: char x1, y1: Esquina superior izquierda
				char x2, y2: Esquina inferior derecha
				char tipo  : 0-> L°nea Sencilla
								 1-> L°nea Doble
				char fondo : Color del fondo
				char atr   : Atributo
------------------------------------------------------------------------- */
void ventana(x1, y1, x2, y2, tipo, fondo, atr)
char x1, y1, x2, y2, tipo, fondo, atr;
{	register int i;
	char *s[2]= {"’Õ∏≥¿ƒŸ", "…Õª∫»Õº"};
	char far *v = _VIDEO;

	un_car(x1,y1,s[tipo][0], atr);
	un_car(x2,y1,s[tipo][2], atr);
	un_car(x1,y2,s[tipo][4], atr);
	un_car(x2,y2,s[tipo][6], atr);
	v = POSXY(_VIDEO,x1+1,y1);
	for(i=x1+1; i<x2; i++)
	{	*v++ = s[tipo][1];
		*v++ = atr;
	}
	v = POSXY(_VIDEO,x1+1,y2);
	for(i=x1+1; i<x2; i++)
	{	*v++ = s[tipo][5];
		*v++ = atr;
	}
	v = _VIDEO;
	for (i=y1+1; i<y2; i++)
	{	v[x1*2+160*i]=v[x2*2+160*i]=s[tipo][3];
		v[x1*2+160*i+1]=v[x2*2+160*i+1]=atr;
	}
	fondo_vent(x1+1, y1+1, x2-1, y2-1, fondo);
}
/* -------------------------------------------------------------------------
FUNCION: v_explode
Objetivo: Desplegar una ventana en la pantalla del centro a los bordes
Par†metros: char x1, y1: Esquina superior izquierda
				char x2, y2: Esquina inferior derecha
				char tipo  : 0-> L°nea Sencilla
								 1-> L°nea Doble
				char fondo : Color del fondo
            char atr   : Atributo
------------------------------------------------------------------------- */
void v_explode(x1,y1,x2,y2,tipo,fondo,atr)
char x1,y1,x2,y2,tipo,fondo,atr;
{	register xx1, yy1, xx2, yy2;

	xx1=(x1+x2)/2;
	yy1=(y1+y2)/2;
	xx2=xx1+1;
	yy2=yy1+1;
	ventana(xx1,yy1,xx2,yy2,tipo,fondo,atr);
	while(xx1>x1||yy1>y1||xx2<x2||yy2<y2)
	{  delay(10);
		if(xx1>x1) xx1--;
		if(yy1>y1) yy1--;
		if(xx2<x2) xx2++;
		if(yy2<y2) yy2++;
		ventana(xx1,yy1,xx2,yy2,tipo,fondo,atr);
	}
}
/* -------------------------------------------------------------------------
FUNCION: fondo_vent
Objetivo: pintar el fondo de una ventana
Par†metros: char x1, y1: Esquina superior izquierda
				char x2, y2: Esquina inferior derecha
				char fondo : Color del fondo
------------------------------------------------------------------------- */
void fondo_vent(x1, y1, x2, y2, fondo)
char x1, y1, x2, y2, fondo;
{	register int i, j;
	char far *v;

	for(i=y1; i<=y2; i++)
	{  v = POSXY(_VIDEO, x1, i);
		for (j=x1; j<=x2; j++)
		{ *v++ = ' ';
		  *v++ = fondo;
		}
	}
}
/* -------------------------------------------------------------------------
FUNCION: shadow
Objetivo: Desplegar sombra a un marco de la pantalla
Par†metros: char x1, y1: Esquina superior izquierda
				char x2, y2: Esquina inferior derecha
				char atr   : Atributo
------------------------------------------------------------------------- */
void shadow(x1, y1, x2, y2, atr)
char x1, y1, x2, y2, atr;
{  register int i;
	char far *somb;

	somb = POSXY(_VIDEO, x1+1, y2+1)+1;
	for(i=x1; i<x2; i++)
		*(somb+=2) = atr;
	somb = POSXY(_VIDEO, x2+1, y1+1)+1;
	for(i=y1; i<y2; i++)
		*(somb+=SIG_REN) = atr;
}
/* -------------------------------------------------------------------------
FUNCION: change_atr
Objetivo: cambiar el atributo a una †rea en la pantalla
Par†metros: char x1, y1: Esquina superior izquierda
				char x2, y2: Esquina inferior derecha
				char atr   : Atributo
------------------------------------------------------------------------- */
void change_atr(x1,y1,x2,y2,atr)
char x1, y1, x2, y2, atr;
{	register int i, j;
	char far *v;

	for(i=y1; i<=y2; i++)
	{  v = POSXY(_VIDEO, x1, i)+1;
		for (j=x1; j<=x2; j++, v+=2)
		  *v=atr;
	}
}
/* -------------------------------------------------------------------------
FUNCION: copia_screen
Objetivo: Copiar la pantalla a un buffer
Par†metros: char  *buffer: Direcci¢n donde se habr† de guardar la pantalla
------------------------------------------------------------------------- */
void copia_screen(buffer)
char *buffer;
{  register int i;
	char far *v = _VIDEO;
	for(i=0; i<MAXSCREEN; i++)
		*buffer++ = *v++;
}
/* -------------------------------------------------------------------------
FUNCION: rest_screen
Objetivo: Copiar a la pantalla desde un buffer
Par†metros: char  *buffer: Direcci¢n donde se halla la informaci¢n
------------------------------------------------------------------------- */
void rest_screen(buffer)
char *buffer;
{  register int i;
	char far *v = _VIDEO;
	for(i=0; i<MAXSCREEN; i++)
		*v++ = *buffer++;
}
/* -------------------------------------------------------------------------
FUNCION: copia_area
Objetivo: Copiar una †rea de la pantalla a un buffer
Par†metros: PANT  *zona: Direcci¢n donde se habr† de guardar
				char 	x1, y1 : Esquina superior izquierda
				char  x2, y2 : Esquina inferior derecha
------------------------------------------------------------------------- */
void copia_area(x1, y1, x2, y2, zona)
char x1, y1, x2, y2;
PANT *zona;
{  register int i, j;
	char *aux;
	char far *v;

	aux = zona->buffer = (char*) malloc(2*(x2-x1+1)*(y2-y1+1));
	zona->x1 = x1; zona->y1 = y1;
	zona->x2 = x2; zona->y2 = y2;
	for (j=y1; j<=y2; j++)
	{	v = POSXY(_VIDEO,x1,j);
		for (i=0; i<2*(x2-x1+1); i++)
			*aux++ = *v++;
	}
}
/* -------------------------------------------------------------------------
FUNCION: rest_area
Objetivo: Copiar una †rea desde un buffer a la pantalla
Par†metros: PANT  *zona: Direcci¢n de la estructura con informaci¢n
------------------------------------------------------------------------- */
void rest_area(zona)
PANT *zona;
{  register int i, j;
	char *aux;
	char far *v;

	aux = zona->buffer;
	for (j=zona->y1; j<=zona->y2; j++)
	{	v = POSXY(_VIDEO,zona->x1,j);
		for (i=0; i<2*(zona->x2-zona->x1+1); i++)
			*v++ = *aux++;
	}
	FREE_BUFFER(zona->buffer);
}
/* -------------------------------------------------------------------------
FUNCION: menu
OBJETIVO: Procesar un men£ de barras
PARAMETROS: char x1, y1			:  Esquina superior izquierda
				MENATR *menatr    :  Direcci¢n a estructura con informaci¢n de
					char atr				:	Atributo del marco
					char somb			:	Color de la sombra del marco
					char atropc			:	Atributo de las opciones
					char atrbar			:	Atributo de la barra
				char posbar		   :  Posici¢n inicial de la barra
				char numopc       :  N£mero de opciones
				char *opciones[]	:	Arreglo de punteros con direcciones de
											las opciones
				PANT *menu0			:	Direcci¢n donde habr† de guardarse
											informaci¢n sobre pantalla anterior al men£
DEVUELVE	   -1 		: Si se oprimi¢ ESC
				 0 - n   : N£mero de selecci¢n
------------------------------------------------------------------------ */
int menu(x1,y1,posbar,numopc,opc,menatr,menu0)
char x1,y1,posbar,numopc,*opc[];
MENATR *menatr;
PANT *menu0;
{	char maxlon=0;
	int t;
	register x,y,i;

	for(i=0;i<numopc;i++)
		if(strlen(opc[i])>maxlon) maxlon=strlen(opc[i]);
	if (!(menu0->buffer))
		copia_area(x1,y1,x1+maxlon+2,y1+numopc+2,menu0);
	ventana(x1,y1,x1+maxlon+1,y1+numopc+1,0,menatr->atropc,menatr->atr);
	shadow(x1,y1,x1+maxlon+1,y1+numopc+1,menatr->somb);
	for(y=y1+1,i=0;i<numopc;i++,y++)
		desp_cad(x1+1,y,opc[i],menatr->atropc);
	if (posbar >= numopc) posbar=0;
	x=x1+1; y=y1+1;
	for(;;)
	{	change_atr(x,y+posbar,x1+maxlon,y+posbar,menatr->atrbar);
		t= getcode();
		switch(t)
		{	case 72:	/* Arriba */
				change_atr(x,y+posbar,x1+maxlon,y+posbar,menatr->atropc);
				if(!posbar) posbar=numopc-1;
				else posbar--;
				break;
			case 80:	/* Abajo */
				change_atr(x,y+posbar,x1+maxlon,y+posbar,menatr->atropc);
				if (posbar==(numopc-1)) posbar=0;
				else posbar++;
				break;
			case 13:	/* Enter */
				return posbar;
			case 27:	/* Esc */
				return (-1);
		}
	}
}
/* -------------------------------------------------------------------------
FUNCION: 	close_menu
Objetivo: 	Recibir una estructura tipo PANT para restituir la	pantalla y
				posteriormente limpiar la estructura.
Par†metros: PANT *menu0: Direcci¢n de la estructura
------------------------------------------------------------------------- */
void close_menu(PANT *menu0)
{  rest_area(menu0);
	menu0->x1=menu0->x2=menu0->y1=menu0->y2=0;
	menu0->buffer=NULL;
}

/* -------------------------------------------------------------------------
FUNCION: 	getcode
Objetivo: 	Recibir una tecla doble c¢digo, Enter o Escape
Par†metros: Ninguno
Devuelve:   Un entero que representa el n£mero de tecla.
------------------------------------------------------------------------- */
int getcode()
{	int c;
	for(;;)
	{	c=getch();
		if (c==13||c==27) return c;
		if (!c)	return getch();
	}
}
/* -------------------------------------------------------------------------
FUNCION: desp_cad
Objetivo: Desplegar una cadena en la pantalla
Par†metros: char  x1, y1: Posici¢n de la cadena a desplegar
				char *s		: Cadena
				char  atr   : Atributo

------------------------------------------------------------------------- */
void desp_cad(x1, y1, s, atr)
char x1, y1, *s, atr;
{  char far *v;

	v = POSXY(_VIDEO, x1, y1);
	for (;*s;)
	{	*v++ = *s++;
		*v++ = atr;
	}
}
/* -------------------------------------------------------------------------
FUNCION: pide_cad
Objetivo: Recibir una cadena en pantalla y memoria
Par†metros: char  x1, y1: Posici¢n donde se habr† de recibir
				char *s		: Direcci¢n de la variable que recibir† la cadena
				char max		: M†ximo de caracteres por recibir

------------------------------------------------------------------------- */
void pide_cad(x1, y1, s, max)
char x1, y1, *s, max;
{  register int i=0, exit=0, X, Y;
	char far *v;
	char *chptr=s, car;

	v = POSXY(_VIDEO,x1,y1);
	gotoxy(x1+1, y1+1);
	while(!exit)
	{  X = wherex();
		Y = wherey();
		car = getch();
		switch(car)
		{	case 	8:   /* backspace */
				if (i)
				{	if (*(--chptr)==9)
					{	i -= 3; v -= 6;
                  gotoxy(X-3,Y);
					}
					else
					{	v -= 2;
						*v = ' '; i--;
						gotoxy(X-1, Y);
					}
				}
				break;
			case 	9:   /* tab */
				if (i+3 <= max)
			{  *chptr++ = 9;
					i += 3; v += 6;
					gotoxy(X+3,Y);
				}
				break;
			case 13:
				*chptr = '\0';
				exit = 1;
				break;
			default:
				if (i<max && car>31)
				{	*chptr++ = *v = car;
					v += 2; i++;
					gotoxy(X+1, Y);
				}
		}
	}
}