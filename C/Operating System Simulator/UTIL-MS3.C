/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
						 INGENIERIA EN SISTEMAS COMPUTACIONALES
										Sistemas Operativos
M¢dulo     : Funciones de utiler¡a.
Archivo    : util_ms3.c
Objetivo   : Funciones diversas para el Simulador de un Sistema Operativo.
Dise¤o     : Jorge Eduardo Aparicio Mayorga  -  90120511-3,
				 Jos‚ Antonio Valderrama Ramos  -  90120509-9.
Implemento : Los mismos.
Versi¢n    : Noviembre 1993.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <conio.h>
#include <stdlib.h>
#include <graphics.h>
#include <dos.h>
#include "mouse.h"
#include "dsls.h"
#include "so01.h"

void start_system(),shut_down(),boom();
int check_mouse(),check_command();
void pide_cad(char, char, char*, char),print_ps(int,int *,PCB *,char *);

extern unsigned int _x,_y;
extern int __x, __y, ratonazo;
extern int regs[];
extern unsigned char SF,ZF;
hls_ptr l_listos, l_slistos, l_bloqueados, l_sbloqueados;
hls_ptr l_dormidos, l_procesos;
int proc_counter, vel;
char *memoria;

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : start_system
Objetivo   : Rutina que inicializa el mouse, el modo gr fico (VGA HI),
				 define el cursor del mouse, prende el cursor del mouse y dibuja
				 el toolbox de comandos.
Par metros : Ninguna.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void start_system()
{
	int i;
	if (!_inic_mouse())
		{
		printf("Perdone, el programa requiere de un mouse.\n");
		printf("Revise si est  cargado el driver, y si todo est  en orden\n");
		printf("y vuelva a correr el programa.  Gracias.\n");
		exit (1);
		}
	for (i=0;i<NUM_REGS;regs[i++]=0) ;
	ZF = SF = 0;
	proc_counter = 1;
	vel = 10;
	l_listos = create_ls(NULL);
	l_slistos = create_ls(NULL);
	l_bloqueados = create_ls(NULL);
	l_sbloqueados = create_ls(NULL);
	l_dormidos = create_ls(NULL);
	l_procesos = create_ls(NULL);           /* Crear la tabla de procesos.
															  para evaluar intrbloqueo */
	ini_graf();
	draw_interface();
	paint_registers();
	paint_flags();
	draw_toolbox(_x,_y);

	paint_mem(0, NULL, 1000);
	memoria = (char*) inicia_mem();
	if(!memoria)
		error("Memoria agotada");

	_def_cur_graph(4);
	_cur_range(1,1,200,70);
	_show_hide_cur(SHOW);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : shut_down
Objetivo   : Rutina que apaga modo gr fico, libera memoria y sale del
				 programa.
Par metros : Ninguna.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void shut_down()
{
	free_pcblist(l_listos);
	free_pcblist(l_slistos);
	destroy_ls(&l_bloqueados);
	destroy_ls(&l_sbloqueados);
	closegraph();
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : check_command
Objetivo   : Rutina que revisa si se debe de ejecutar un comando.  En caso
				 afirmativo los ejecuta, si no, regresa de donde fue llamado.
Par metros : Ninguna.
Salida     : 0 si es salir, 1 en caso contrario.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
int check_command()
{
	int i = check_mouse();
	switch(i)
		{
		case 0:
			break;
		case 1:
			cargar();
			break;
		case 2:
			matar();
			break;
		case 3:
			suspender();
			break;
		case 4:
			reanudar();
			break;
		case 5:
			help();
			break;
		case 6:
			velocidad();
			break;
		case 7:
			priosys();
			break;
		case 8:
			prioproc();
			break;
		case 9:
			directorio();
			break;
		case 10:
			ps();
			break;
		case 11:
			mem();
			break;
		case 12:
			return 0;
		}
	return 1;
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : check_mouse.
Objetivo   : Revisar si se ha presionado un bot¢n del mouse.  En caso
				 afirmativo, regresar un valor la cual indique el comando a
				 ejecutar.  Nota:  La acci¢n de mover el toolbox se hace
				 directamente desde esta funci¢n.
Par metros : Ninguno.
Salida     : Entero con el valor asignado a cada comando, como sigue:
						  0  - Nada ha ocurrido;
						  1  - Cargar un proceso;
						  2  - Matar (kill) un proceso;
						  3  - Suspender un proceso;
						  4  - Reanudar un proceso;
						  5  - Ayuda;
						  6  - Funci¢n dentro del toolbox no-definida;
						  7  - Establecer la prioridad del sistema (SysPri);
						  8  - Cambiar de prioridad a un proceso (Pri);
						  9  - Directorio (ls);
						  10 - Mostrar informaci¢n acerca de procesos (ps);
						  11 - Irse al Editor de Programas; y,
						  12 - Salir.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
int check_mouse()
{
	int x1,y1;
	unsigned int boton;

	if(ratonazo)
	{ x1 = __x;
	  y1 = __y;
	  ratonazo = 0;
	}
	else
	{  boton = _check_button(&x1,&y1,0);
		_clean_mouse();
		if (!boton /*|| boton==2 || boton==3*/)/* Si apret¢ ning£n bot¢n, el ... */
			return 0;           /* ... izquierdo, o el central, regresa un 0. */
	}
	if (x1<_x+2 || x1>_x+188 || y1<_y+2 || y1>_y+58)
		return 0;
	if (x1>_x+2&&x1<_x+8&&y1>_y+2&&y1<_y+58)
		{
		sound(800);
		delay(200);
		nosound();
		return 0;
		}
	if (y1>_y+2 && y1<_y+28)
		{
		if (x1>_x+12 && x1<_x+38)                                /* Cargar. */
			return 1;
		if (x1>_x+42 && x1<_x+68)                                 /* Matar. */
			return 2;
		if (x1>_x+72 && x1<_x+98)                             /* Suspender. */
			return 3;
		if (x1>_x+102 && x1<_x+128)                            /* Reanudar. */
			return 4;
		if (x1>_x+132 && x1<_x+158)                               /* Ayuda. */
			return 5;
		if (x1>_x+162 && x1<_x+188)                 /* Comando no-definido. */
			return 6;
		}
	else
		{
		if (y1>_y+32 && y1<_y+58)
			{
			if (x1>_x+12 && x1<_x+38)                             /* SysPri. */
				return 7;
			if (x1>_x+42 && x1<_x+68)                                /* Pri. */
				return 8;
			if (x1>_x+72 && x1<_x+98)                         /* Directorio. */
				return 9;
			if (x1>_x+102 && x1<_x+128)                               /* ps. */
				return 10;
			if (x1>_x+132 && x1<_x+158)                           /* Editor. */
				return 11;
			if (x1>_x+162 && x1<_x+188)                            /* Salir. */
				return 12;
			}
		}
	return 0;
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : boom.
Objetivo   : Ruido.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void boom()
{
	int x,y;
	for (x=1;x<50;x++)
		{
		y = (random(5)+5)*100;
		sound(y);
		delay(5);
		nosound();
		}
	delay(100);
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

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : print_ps.
Objetivo   : Funci¢n que imprime datos de los procesos.
Par metros : int x,*y = Coordenadas donde se va a imprimir la informaci¢n,
				 PCB *xxx = Apuntador a la informaci¢n del PCB del proceso, y
				 char *edo = el estado en el que se encuentra.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void print_ps(int x,int *y,PCB *xxx,char *edo)
{
	char id[3];
	int a,b;
	sprintf(id,"%d",xxx->id_proc);
	outtextxy(x,*y,id);
	outtextxy(x+20,*y,xxx->name_proc);
	outtextxy(x+115,*y,edo);
	sprintf(id,"%d",xxx->prio);
	outtextxy(x+195,*y,id);
	*y += 10;
	if (*y == 153)
		{
		_show_hide_cur(SHOW);
		while (!kbhit() && !_check_button(&a,&b,0)) ;
		_show_hide_cur(HIDE);
		clearviewport();
		*y = 3;
		}
}
