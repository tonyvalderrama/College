/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
						 INGENIERIA EN SISTEMAS COMPUTACIONALES
										Sistemas Operativos
M¢dulo     : Funciones de interbloqueo.
Archivo    : INTRBLOK.C.
Objetivo   : Funciones para la resolver el interbloqueo en nuestro Simulador
				 de un Sistema Operativo.
Dise¤o     : Jorge Eduardo Aparicio Mayorga  -  90120511-3,
				 Jos‚ Antonio Valderrama Ramos  -  90120509-9.
Implemento : Los mismos.
Versi¢n    : Abril 1994.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
#include <stdio.h>
#include <stdlib.h>
#include "dsls.h"
#include "so01.h"

typedef struct
{	HEADSLS;
	PCB *pcb;
	unsigned char tiene_nec[2];
} TIENE_NEC;

extern hls_ptr l_listos, l_dormidos, l_procesos;
extern PCB* pcb_exec;
unsigned char rec_tab[NUM_RECS][2];

void reduce_lista();
TIENE_NEC *selecciona_victima();
void duerme_victima(PCB*);
void despierta_victimas();
void libera_recursos(PCB*);
int prep_table();
void llena_tabla_con(),intrblok();

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : prep_table.
Objetivo   : Funci¢n que se encarga de preparar las tablas utilizadas para
				 detectar y resolver el interbloqueo.
Par metros : Ninguno.
Salida     : 0 en caso de alg£n problema, un valor TRUE (>1) en caso
				 contrario.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
int prep_table()
{
	register i,j;

	for (i=0;i<NUM_RECS;i++)     /* Inicializar la tabla de recursos en 0. */
		for(j=0;j<2;j++)
			rec_tab[i][j] = 0;
	llena_tabla_con();                        /* Llenar tabla de procesos. */
	return 1;
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : llena_tabla_con.
Objetivo   : Funci¢n que recorre una lista y mete procesos que tengan
				 asignados recursos y que se les han solicitado recursos no
				 otorgados a la tabla de procesos.
Par metros : Ninguno.
Salida     : Ninguno.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void llena_tabla_con()
{	TIENE_NEC *tm;
	PCB *pcb_act;
	register i,j;
	char flag;

	if(pcb_exec)
		push_sls(l_listos, (ls_ptr) pcb_exec);
	for_ls(l_listos,pcb_act,PCB)
		if (pcb_act->num_solic > 1)
			for (i=0;i<NUM_RECS;i++)
				if (pcb_act->rec[i])
					{
					tm = (TIENE_NEC *) node_ls(NULL,TIENE_NEC);
					tm->pcb = pcb_act;
					tm->tiene_nec[0] = i;
					tm->tiene_nec[1] = pcb_act->rec_solic;
					insert_sls(l_procesos,(ls_ptr)tm);
					rec_tab[i][0]++;
					rec_tab[pcb_act->rec_solic][1]++;
					}
	if(pcb_exec)
		pop_sls(l_listos);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : intrblok.
Objetivo   : Funci¢n que detecta y resuelve el interbloqueo.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void intrblok()
{  despierta_victimas();
	if (!prep_table())
		return;
	reduce_lista();
}
/*--------------------------------------------------------------------------
FUNCION 	 : reduce_lista
OBJETIVO  : Reducir la lista de procesos con posible interbloqueo hasta
				darle soluci¢n al mismo.
PARAMETROS: Ninguno.
SALIDA    : Ninguna
---------------------------------------------------------------------------*/
void reduce_lista()
{	register int exist_changes;
	unsigned char tiene, nec;
	TIENE_NEC *proc, *p_aux;

	while(!empty_ls(l_procesos))
	{  do
		{  exist_changes = 0;
			proc = (TIENE_NEC*) first_ls(l_procesos);
			while(proc)
			{	p_aux = (TIENE_NEC*) next_sls(proc);
				tiene = proc->tiene_nec[0];
				nec 	= proc->tiene_nec[1];
				if(!rec_tab[tiene][1] || !rec_tab[nec][0])
				{	rec_tab[tiene][0]--;
					rec_tab[nec][1]--;
					free(delete_sls(l_procesos, (ls_ptr) proc));
					exist_changes++;
				}
				proc = p_aux;
			}
		}
		while(exist_changes);
		if(!empty_ls(l_procesos))
		{  aviso("­­ INTERBLOQUEO !!");
			p_aux = selecciona_victima();
			libera_recursos(p_aux->pcb);
			duerme_victima(p_aux->pcb);
			rec_tab[p_aux->tiene_nec[0]][0]--;
			rec_tab[p_aux->tiene_nec[1]][1]--;
			free(delete_sls(l_procesos, (ls_ptr) p_aux));
			erase_window();
		}
	}
	purge_ls(l_procesos);
}

/*--------------------------------------------------------------------------
FUNCION 	 : selecciona_victima
OBJETIVO  : Seleccionar una victima para eliminarla.
PARAMETROS: Ninguno.
SALIDA    : (TIENE_NEC *) proceso seleccionado.
---------------------------------------------------------------------------*/
TIENE_NEC *selecciona_victima()
{	TIENE_NEC *proc, *proc_select = NULL;

	proc_select = (TIENE_NEC*) first_ls(l_procesos);
	for_ls(l_procesos, proc, TIENE_NEC)
		if(proc->pcb->prio < proc_select->pcb->prio)
			proc_select = proc;
	return proc_select;
}

/*--------------------------------------------------------------------------
FUNCION 	 : duerme_victima
OBJETIVO  : Dormir a la victima.
PARAMETROS: PCB *pcb_act -> Proceso a dormir
SALIDA    : Ninguna
---------------------------------------------------------------------------*/
void duerme_victima(PCB *pcb_act)
{
	pcb_act->IP = pcb_act->prim_inst;
	if(pcb_act == pcb_exec)
	{  anim_easl(pcb_act->id_proc);
		pcb_exec = NULL;
	}
	else
	{  anim_lasl(pcb_act->id_proc);
		delete_sls(l_listos, (ls_ptr) pcb_act);
	}
	insert_sls(l_dormidos, (ls_ptr) pcb_act);
	pcb_act->edo_proc = DORMIDO;
}

/*--------------------------------------------------------------------------
FUNCION 	 : despierta_victimas
OBJETIVO  : Desperta todas las v¡ctimas anteriores.
PARAMETROS: Ninguna
SALIDA    : Ninguna
---------------------------------------------------------------------------*/
void despierta_victimas()
{  PCB *pcb_act;
	while(!empty_ls(l_dormidos))
	{  pcb_act = (PCB*) pop_sls(l_dormidos);
		anim_slal(pcb_act->id_proc);
		inserta_ordenado(l_listos, pcb_act);
		pcb_act->edo_proc = LISTO;
	}
}

