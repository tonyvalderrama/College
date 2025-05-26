/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
						 INGENIERIA EN SISTEMAS COMPUTACIONALES
										Sistemas Operativos
M¢dulo     : Funciones de los comandos del Simulador.
Archivo    : graf-ms3.c
Objetivo   : Funciones para la ejecuci¢n de comandos del Simulador de un
				 Sistema Operativo.
Dise¤o     : Jorge Eduardo Aparicio Mayorga  -  90120511-3,
				 Jos‚ Antonio Valderrama Ramos  -  90120509-9.
Implemento : Los mismos.
Versi¢n    : Noviembre 1993.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include "mouse.h"
#include "dsls.h"
#include "so01.h"
#include "real_mem.h"

#define DEF_PRIO 5

extern int vel;
extern unsigned int syspri;
extern hls_ptr l_listos,l_slistos, l_bloqueados, l_sbloqueados, l_dormidos,
					l_admem;
extern PCB *pcb_exec;
extern char* memoria;

void help(),directorio(),cargar(),matar(),suspender(),reanudar(),ps();

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : help.
Objetivo   : Mostrar un mensaje de ayuda en la pantalla.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void help()
{
	int x,y;
	_show_hide_cur(HIDE);
	setcolor(YELLOW);
	settextstyle(/*DEFAULT_FONT*/EUROPEAN_FONT,HORIZ_DIR,1);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy(320,75,"Help");
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	outtextxy(395,80,"Toolbox");
	draw_toolbox(220,155);
	setcolor(LIGHTMAGENTA);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	outtextxy(215,110,"0 Mover toolbox");
	outtextxy(215,120,"1 Cargar proceso");
	outtextxy(215,130,"2 Matar proceso");
	outtextxy(215,140,"3 Suspender proceso");
	outtextxy(330,110,"4 Reanudar proceso");
	outtextxy(330,120,"5 Ayuda");
	outtextxy(330,130,"6 Velocidad");
	outtextxy(215,220,"7 Prioridad Sistema");
	outtextxy(215,230,"8 Prioridad Proceso");
	outtextxy(215,240,"9 Directorio");
	outtextxy(215,250,"10 Process Status");
	outtextxy(330,220,"11 Editor");
	outtextxy(330,230,"12 Salir");
	setcolor(LIGHTCYAN);
	outtextxy(215,110,"0");
	outtextxy(215,120,"1");
	outtextxy(215,130,"2");
	outtextxy(215,140,"3");
	outtextxy(330,110,"4");
	outtextxy(330,120,"5");
	outtextxy(330,130,"6");
	outtextxy(215,220,"7");
	outtextxy(215,230,"8");
	outtextxy(215,240,"9");
	outtextxy(215,250,"10");
	outtextxy(330,220,"11");
	outtextxy(330,230,"12");
	setcolor(YELLOW);
	outtextxy(223,180,"0");
	outtextxy(250,155,"1");
	outtextxy(280,155,"2");
	outtextxy(310,155,"3");
	outtextxy(340,155,"4");
	outtextxy(370,155,"5");
	outtextxy(400,155,"6");
	outtextxy(250,185,"7");
	outtextxy(280,185,"8");
	outtextxy(310,185,"9");
	outtextxy(335,185,"10");
	outtextxy(365,185,"11");
	outtextxy(395,185,"12");
	delay(300);
	_def_cur_graph(_SMILY);
	_show_hide_cur(SHOW);
	while(!_check_button(&x, &y,0) && !kbhit());
	_def_cur_graph(_FINGER);
	erase_window();
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : directorio.
Objetivo   : Mostrar los archivos del directorio.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void directorio()
{
	int a, b;
	int x,y,j,valor;
	struct ffblk dir;
	_show_hide_cur(HIDE);
	setcolor(LIGHTBLUE);
	settextstyle(/*DEFAULT_FONT*/SCRIPT_FONT,HORIZ_DIR,1);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy(320,78,"Directorio");
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	setcolor(LIGHTGRAY);
	valor = findfirst("*.*",&dir,0);
	y = x = 5;
	setviewport(199,105,441,269,1);
	_def_cur_graph(_SMILY);
	while (!valor)
		{
		if (y==155)
			{
			y = 5;
			x += 150;
			if (x==305)
				{
				_show_hide_cur(SHOW);
				while (!kbhit() && !_check_button(&a,&b,0)) ;
				_show_hide_cur(HIDE);
				delay(300);
				clearviewport();
				x = 5;
				}
			}
		outtextxy(x,y,dir.ff_name);
		y+=10;
		valor = findnext(&dir);
		}
	_show_hide_cur(SHOW);
	while (!kbhit() && !_check_button(&x,&y,0)) ;
	_def_cur_graph(_FINGER);
	erase_window();
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : cargar.
Objetivo   : Lanza un proceso.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void cargar()
{  PCB *pcb_aux;
	char proceso[13];
	_show_hide_cur(HIDE);
	dialog_box("Cargar Proceso","Proceso a cargar:");
	pide_cad(40,13,proceso,12);
	_show_hide_cur(SHOW);
	erase_window();
	pcb_aux = (PCB*) crea_proc(proceso, DEF_PRIO);
	if(pcb_aux)
		if (syspri <= DEF_PRIO)
			inserta_ordenado(l_listos, pcb_aux);
		else
			insert_sls(l_slistos, (ls_ptr) pcb_aux);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : matar.
Objetivo   : Elimina un proceso (kill).
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void matar()
{
	char proceso[3];
	_show_hide_cur(HIDE);
	dialog_box("Matar Proceso","Proceso a matar:");
	pide_cad(40,13,proceso,2);
	_show_hide_cur(SHOW);
	erase_window();
	kill_proc((unsigned int) atoi(proceso));
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : suspender.
Objetivo   : Suspende un proceso.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void suspender()
{
	char proceso[3];
	_show_hide_cur(HIDE);
	dialog_box("Suspender","Proceso a suspender:");
	pide_cad(40,13,proceso,2);
	erase_window();
	_show_hide_cur(SHOW);
	suspende_proc((unsigned int) atoi(proceso));
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : reanudar.
Objetivo   : Reanuda un proceso.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void reanudar()
{
	char proceso[3];
	_show_hide_cur(HIDE);
	dialog_box("Reanudar","Proceso a reanudar:");
	pide_cad(40,13,proceso,2);
	erase_window();
	_show_hide_cur(SHOW);
	reanuda_proc((unsigned int) atoi(proceso));
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : velocidad.
Objetivo   : Establece la velocidad del sistema.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void velocidad()
{
	char velocidad[3];
	int aux;
	_show_hide_cur(HIDE);
	dialog_box("Velocidad","Nueva velocidad:");
	pide_cad(40,13,velocidad,2);
	aux = atoi(velocidad);
	erase_window();
	_show_hide_cur(SHOW);
	if (aux >= 0 && aux <= 10) vel=aux;
	else error("Rango: 0 - 10");
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : priosys.
Objetivo   : Establece prioridad del sistema.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void priosys()
{	int aux;
	char prio[3];
	_show_hide_cur(HIDE);
	dialog_box("SYS prioridad","Nueva prioridad:");
	pide_cad(40,13,prio,2);
	erase_window();
	_show_hide_cur(SHOW);
	aux = atoi(prio);
	if (aux >= 0 && aux <= 10) pri_sys((unsigned int) aux);
	else error("Rango: 0 - 10");
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : prioproc.
Objetivo   : Establece prioridad de un proceso.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void prioproc()
{	int aux;
	char proceso[3];
	char prio[3];
	_show_hide_cur(HIDE);
	dialog_box("PROC prioridad","Proceso:");
	pide_cad(40,13, proceso, 2);
	erase_window();
	dialog_box("PROC prioridad", "Nueva Prioridad");
	pide_cad(40, 13, prio, 2);
	erase_window();
	_show_hide_cur(SHOW);
	aux = atoi(prio);
	if (aux >= 0 && aux <= 10)
		pri_proc((unsigned int) atoi(proceso), (unsigned int) atoi(prio));
	else
		error("Rango: 0 - 10");
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : ps.
Objetivo   : Funci¢n que ejecuta el comando ps.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void ps()
{
	int y,a,b;
	PCB *pcb_aux;
	PCTRL *p_control;
	_show_hide_cur(HIDE);
	_def_cur_graph(_SMILY);
	settextstyle(/*DEFAULT_FONT*/SCRIPT_FONT,HORIZ_DIR,1);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	setcolor(LIGHTBLUE);
	outtextxy(320,78,"Process Status");              /* Pongo los t¡tulos. */
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	setcolor(LIGHTCYAN);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	outtextxy(200,100,"Id.  Nombre         Estado     Prioridad");
	y = 3;
	setcolor(LIGHTGRAY);
	setviewport(199,110,441,269,1);
	clearviewport();
	for_ls(l_listos,pcb_aux,PCB)    /* Imprimiendo los procesos en listos. */
		print_ps(3,&y,pcb_aux,"LISTOS");
	if (pcb_exec)
		print_ps(3,&y,pcb_exec,"EJECUCION");  /* Imprimo proc en ejecuci¢n. */
	for_ls(l_slistos,pcb_aux,PCB)           /* Imprimiendo procesos en ... */
		print_ps(3,&y,pcb_aux,"SLISTOS");        /* ... suspendidos listos. */
	for_ls(l_bloqueados, p_control, PCTRL)
		print_ps(3,&y,p_control->pcb, "BLOQUEADO");      /* ... bloqueados. */
	for_ls(l_sbloqueados, p_control, PCTRL)
		print_ps(3,&y,p_control->pcb, "SBLOQUEADO");    /* ... sbloqueados. */
	for_ls(l_dormidos, pcb_aux, PCB)
		print_ps(3,&y,pcb_aux,"DORMIDOS");					/* ... dormidos.    */
	_show_hide_cur(SHOW);
	delay(300);
	while(!kbhit() && !_check_button(&a,&b,0)) ;
	erase_window();
	setviewport(0,0,getmaxx(),getmaxy(),1);
	_def_cur_graph(_FINGER);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : mem.
Objetivo   : Mostrar el estado de la memoria.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void mem()
{	int a, b;
	int x,y,j;
	NODO_MEM *n_mem;
	char c_aux[50];

	_show_hide_cur(HIDE);
	setcolor(LIGHTBLUE);
	settextstyle(/*DEFAULT_FONT*/SCRIPT_FONT,HORIZ_DIR,1);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy(320,78,"Estado de Memoria");
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	outtextxy(200,100,"Id.  Nombre         Inicio     Longitud");
	setcolor(LIGHTGRAY);
	y = 5;
	setviewport(199,105,441,269,1);
	_def_cur_graph(_SMILY);
	for_ls(l_admem, n_mem, NODO_MEM)
	{	if (y==155)
		{	y = 5;
			_show_hide_cur(SHOW);
			_clean_mouse();
			while (!kbhit() && !_check_button(&a,&b,0)) ;
			_show_hide_cur(HIDE);
			delay(300);
			clearviewport();
		}
		if(n_mem->hop)
			sprintf(c_aux, "%d    %s", n_mem->hop->id_proc,
											  n_mem->hop->name_proc);
		else
			sprintf(c_aux, "     HUECO");
		outtextxy(5, y, c_aux);
		sprintf(c_aux, "%4d        %4d", (int)((char*)n_mem->inicio - memoria),
												n_mem->tam);
		outtextxy(125, y, c_aux);
		y+=10;
	}
	_show_hide_cur(SHOW);
	while (!kbhit() && !_check_button(&x,&y,0));
	_def_cur_graph(_FINGER);
	erase_window();
}