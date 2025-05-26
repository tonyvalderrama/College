/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
							  INSTITUTO TECNOLOGICO DE MOREIA
											Bases de Datos

Archivo    : INTBTREE.C
Objetivo   : M¢dulo con funciones que utilizan llamadas a funciones
				 localizadas en el m¢dulo de la interface.
Dise¤o     : Jos‚ Antonio Valderrama Ramos
				 Jorge E. Aparicio Mayorga.
Implemento : Los mismos.
Versi¢n    : 1.00 (Marzo 1994).
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include "btree.h"
#include "mouse.h"
#include "dsls.h"

NODE *NODOS[MM+2], nodo_ant;

extern NODE *root;
extern hls_ptr l_ant;

void desp_peh(NODE *),desp_nodo(int,NODE *),high(),down(),consul();
void pide_cad(char,char,char *,char), reconstruye_arbol();

/*-------------------------------------------------------------------------
FUNCION:		desp_peh
OBJETIVO:	Funci¢n intermedia que hace el llamado para el despliegue de
				un nodo padre y toda su prole (puros hijos).
PARAMETROS: NODE *nodo: Nodo a desplegar
SALIDA:     Ninguna.
------------------------------------------------------------------------- */
void desp_peh(NODE *nodo)
{	register i;
	dibuja_arbolB();
	if(nodo)
	{ for(i=0;i<6;i++) NODOS[i]=NULL;
		desp_nodo(0, nodo);
		for(i=0; i <= nodo->cnt; i++)
			desp_nodo(i+1, nodo->ptr[i]);
	}
}

/*-------------------------------------------------------------------------
FUNCION:		desp_nodo
OBJETIVO:	Funci¢n intermedia que hace el llamado para el despliegue de
				un nodo.
PARAMETROS: int num_nodo: N£mero del nodo en  la pantalla
				NODE *nodo: Direcci¢n del nodo a desplegar
SALIDA:     Ninguna.
------------------------------------------------------------------------- */
void desp_nodo(int num_nodo, NODE *nodo)
{	register i;
	if(nodo)
	{	NODOS[num_nodo] = nodo;
		for(i=0; i < nodo->cnt; i++)
			escribe_llave(num_nodo, i, nodo->key[i]);
	}
	else
		NODOS[num_nodo] = NULL;
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : high.
Objetivo   : Da de alta un valor a la base de datos.  Es una funci¢n
				 intermedia que llama a la funci¢n insert (archivo btree.c).
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void high()
{
	char s[4];
	int ss;
	settextstyle(SMALL_FONT,HORIZ_DIR,5);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	borra_ventana();
	setcolor(LIGHTGREEN);
	_show_hide_cur(HIDE);
	outtextxy(20,380,"Llave de Registro a dar de alta:");
	pide_cad(35,25,s,3);
	_show_hide_cur(SHOW);
	ss = atoi(s);
	root = insert(ss,root);
	reconstruye_arbol();
	borra_ventana();
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : down.
Objetivo   : Elimina un registro de la Base de Datos.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void down()
{
	char s[4];
	int ss;
	settextstyle(SMALL_FONT,HORIZ_DIR,5);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	borra_ventana();
	setcolor(LIGHTGREEN);
	_show_hide_cur(HIDE);
	outtextxy(20,380,"Llave de Registro a dar de baja:");
	pide_cad(35,25,s,3);
	_show_hide_cur(SHOW);
	ss = atoi(s);
	root = delete(ss,root);
	reconstruye_arbol();
	borra_ventana();
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : reconstruye_arbol.
Objetivo   : Reconstruye las listas utilizadas en el despliege del  rbol.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void reconstruye_arbol()
{
	purge_ls(l_ant);
	desp_peh(root);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : consul.
Objetivo   : Consulta el  rbol B.  Busca una llave y muestra los pasos
				 intermedios (los nodos que tuvo que visitar).
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void consul()
{
	char s[4];
	int ss;
	settextstyle(SMALL_FONT,HORIZ_DIR,5);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	borra_ventana();
	setcolor(LIGHTGREEN);
	_show_hide_cur(HIDE);
	outtextxy(20,380,"Llave de Registro a consultar:");
	pide_cad(35,25,s,3);
	_show_hide_cur(SHOW);
	ss = atoi(s);
	search(ss,root);
	borra_ventana();
}

/* -------------------------------------------------------------------------
FUNCION: pide_cad
Objetivo: Recibir una cadena en pantalla y memoria
Par metros: char  x1, y1: Posici¢n donde se habr  de recibir
				char *s		: Direcci¢n de la variable que recibir  la cadena
				char max		: M ximo de caracteres por recibir
------------------------------------------------------------------------- */
void pide_cad(x1, y1, s, max)
char x1, y1, *s, max;
{  register int i=0, exit=0, X, Y;
	char *chptr=s, car;
	gotoxy(x1, y1);
	while(!exit)
	{  X = wherex();
		Y = wherey();
		car = getch();
		switch(car)
		{	case 	8:   /* backspace */
				if (i)
				{  i--; chptr--;
					gotoxy(X-1, Y);
					printf(" ");
					gotoxy(X-1, Y);
				}
				break;
			case 13:
				*chptr = '\0';
				exit = 1;
				break;
			default:
				if (i<max && car>31)
				{	*chptr++ = car;
					i++;
					printf("%c", car);
					gotoxy(X+1, Y);
				}
		}
	}
}
