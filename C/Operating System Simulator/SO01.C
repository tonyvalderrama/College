/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
						 INGENIERIA EN SISTEMAS COMPUTACIONALES
										Sistemas Operativos
M¢dulo     : Funciones principales.
Archivo    : SO01.C
Objetivo   : Funciones para el manejo intr¡nseco (en bajo nivel) del SO.
Dise¤o     : Jorge Eduardo Aparicio Mayorga  -  90120511-3,
				 Jos‚ Antonio Valderrama Ramos  -  90120509-9.
Implemento : Los mismos.
Versi¢n    : Marzo de 1994.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dsls.h"
#include "so01.h"
#include "graf-ms3.h"

extern int op, proc_counter;
extern hls_ptr l_listos, l_slistos, l_bloqueados, l_sbloqueados;
extern char *memoria;
char *tab[]={"ADD","BEGIN","CALL","CALLM","CMP","DEC","EMPTY","END","ENDM",
				  "ENDP","FINMO","FRE","GET","INC","INIMO","JEQ","JGE","JLE",
				  "JMP","JNE","MOV","MOVR","MOVRV","MOVV","NOP","P","PARBEGIN",
				  "PAREND","PROC","PROCM","SET","SIGNAL","SUB","V","WAIT"};

int regs[NUM_REGS];
unsigned char ZF, SF;
int recursos[NUM_RECS] = {MAXREC, MAXREC, MAXREC, MAXREC, MAXREC};
PCB *pcb_exec = NULL;
unsigned int syspri=INIC_PRI;
unsigned int timer=0;

/* ------------------------------------------------------------------------
Definici¢n de prototipos
------------------------------------------------------------------------- */
void lee_linea(FILE*,char*[]);
PCB *crea_proc(char*,int);
int busbin(char*,int,int);
int valint(char*);
void inic_pcb(PCB*,char*,int,int,PCB*,int,int,int);
void ejecuta_proc(void);
void tiempo_terminado(void);
void guarda_regs(PCB*);
PCB *despachar(void);
void restablece_regs(PCB*);
void free_proc(PCB*);
int compara(PCB*, PCB*);
unsigned int ejec_instr(PCB*);
void inserta_ordenado(hls_ptr, PCB*);
void suspende_proc(unsigned int);
int busca_proc(PCB*, unsigned int*);
int busca_pctrl(PCTRL*, unsigned int*);
void pri_proc(unsigned int, unsigned int);
void kill_proc(unsigned int);
void termina_listas(void);
void clean_lista(hls_ptr);
unsigned int semaphore_p(unsigned int, PCB*);
void semaphore_v(unsigned int,unsigned char,PCB *);

/* ------------------------------------------------------------------------
FUNCION: lee_linea
OBJETIVO: Leer una l¡nea de c¢digo del archivo de texto.
PARAMETROS: FILE *fp: Puntero al archivo de texto,
				char *codigo[]: Ptr a array de c¢digos,
------------------------------------------------------------------------ */
void lee_linea(FILE *fp, char *codigo[])
{	register int i, pos;
	register char aux, indica_code=0;
	for(i=0; i<MAXCODIGOS; i++)
		codigo[i] = NULL;
	aux = getc(fp);
	pos=0;
	while (!feof(fp) && aux != '\n')
	{  if(aux == ' ' || aux == '\t')
		{	while(aux == ' ' || aux == '\t')
				aux = getc(fp);
			indica_code++;
			pos=0;
			continue;
		}
		if (!codigo[indica_code])
			codigo[indica_code] = (char*) malloc (25);
		codigo[indica_code][pos] = toupper(aux);
		codigo[indica_code][++pos] = '\0';
		aux = getc(fp);
	}
}

/* ------------------------------------------------------------------------
Funci¢n:	crea_proc
Objetivo: Leer un programa de disco design ndolo como un proceso
Parametros: char *fname	: Nombre del proceso en disco
				int prio		: Prioridad del proceso
Regresa: Un puntero al PCB correspondiente a este proceso.
			NULL si ocurri¢ una contingencia.
-------------------------------------------------------------------------- */
PCB *crea_proc(char *fname, int prio)
{	PCB *pcb_act;
	FILE *fp;
	char *codigos[MAXCODIGOS], aux[18];
	unsigned int numlin,numvars,numsem,numvarcon,tam,tam_prg;
	register i,j,cp=0;
	struct tproc
	{	char proc[15];
		unsigned int pos;
	} tabproc[10];
	if ((fp = fopen(fname, "r"))==NULL)
	{	error ("Error de apertura");
		return NULL;
	}
	lee_linea(fp, codigos);
	if (strcmp(codigos[0], "MEMORY"))
	{  error ("Falta instruccion MEMORY");
		fclose(fp);
		return NULL;
	}
	numlin	= atoi(codigos[1]);
	numvars	= atoi(codigos[2]);
	numsem	= atoi(codigos[3]);
	numvarcon= atoi(codigos[4]);
	tam = numlin*sizeof(instruccion) + numvars*sizeof(int);
	tam += (numsem+1)*sizeof(semaforo) + numvarcon * sizeof(hls_ptr);
	tam_prg = numlin*sizeof(code_line);
	if((pcb_act=(PCB*) malloc(sizeof(PCB)))==NULL)
	{	error ("Memoria agotada");
		fclose(fp);
		return NULL;
	}
	if((pcb_act->CS=(instruccion*) mymalloc(tam, pcb_act))==NULL)
	{	error ("Memoria agotada");
		fclose(fp);
		free(pcb_act);
		return NULL;
	}
	if((pcb_act->PRG=(code_line*) malloc(tam_prg))==NULL)
	{	error ("Memoria agotada");
		fclose(fp);
		myfree(pcb_act->CS);
		free(pcb_act);
		return NULL;
	}
	inic_pcb(pcb_act,fname,proc_counter++,prio,NULL,numlin,numvars,numsem+1);
	for(i=0; i < numsem+1; i++)
		SEMAFOROS[i].l_sem = create_ls(NULL);
	for(i=0;i<numvarcon;i++)
		VARCON[i] = create_ls(NULL);
	if (numvarcon) SEMAFOROS[0].edo = 1;
	for(i=1; i<numlin && !feof(fp); i++)
	{	lee_linea (fp, codigos);
		if ( !strcmp(codigos[0],"PROC") || !strcmp(codigos[0],"PROCM") )
		{	strcpy(tabproc[cp].proc, codigos[1]);
			tabproc[cp++].pos = i+1;
			codigos[1] = '\0';
		}
		else
			if (!strcmp(codigos[0],"CALL") || !strcmp(codigos[0],"CALLM") )
			{	for (j=0; j<cp; j++)
					if(!strcmp(codigos[1], tabproc[j].proc))
					{	sprintf(codigos[1], "%u", tabproc[j].pos);
						break;
					}
			}
			else
				if (!strcmp(codigos[0], "BEGIN"))
					pcb_act->prim_inst = pcb_act->IP = i+1;
		pcb_act->CS[i].id_op = busbin(codigos[0],34,0);
		pcb_act->CS[i].op1 = valint(codigos[1]);
		pcb_act->CS[i].op2 = valint(codigos[2]);
		*aux = '\0';
		for(j=0; codigos[j]; j++)
		{	strcat(aux, codigos[j]);
			strcat(aux, " ");
		}
		aux[strlen(aux)-1]='\0';
		strcpy(pcb_act->PRG[i].line, aux);
	}
	for (j=0; j<MAXCODIGOS; j++) free (codigos[j]);
	fclose(fp);
	return pcb_act;
}

/* ------------------------------------------------------------------------
Funci¢n    : busbin
Objetivo   : Hacer una b£squeda binaria en la tabla de palabras reservadas
Par metros : char *id : Palabra a buscar
				 int max  : L¡mite m ximo de la tabla
				 int min  : L¡mite m¡nimo de la tabla
Salida     : Un entero que identifica a la palabra reservada
				 -1 en caso de que no la encuentre.
-------------------------------------------------------------------------- */
int busbin(char *id, int max, int min)
{
	int p;
	if(max<min) return -1;
	p=(max+min)/2;
	if(!strcmp(tab[p], id)) return p;
	else
		{
		if (strcmp(id, tab[p])>0) min=++p;
		else max=--p;
		busbin(id,max,min);
		}
}

/* ------------------------------------------------------------------------
Funci¢n    : valint
Objetivo   : Obtiene un valor entero, que indica un n£mero de registro
				 o bien un valor constante
Par metros : char *str: Cadena con nombre del registro o con un n£mero.
Salida     : Un entero.
-------------------------------------------------------------------------- */
int valint(char *str)
{	if (!strcmp(str, "AX")) return 0;
	if (!strcmp(str, "BX")) return 1;
	if (!strcmp(str, "CX")) return 2;
	if (!strcmp(str, "DX")) return 3;
	if (!strcmp(str, "EX")) return 4;
	return atoi(str);
}

/* ------------------------------------------------------------------------
Funci¢n    : inic_pcb
Objetivo   : Cargar un PCB con valores de arranque.
Par metros : PCB *pcb_act:		Puntero PCB a incializar.
				 char *name_proc: Nonmbre del proceso.
				 int id_proc:		Identificador del proceso.
				 int prio:			Prioridad inicial del proceso.
				 int numlin:		N£mero de l¡neas del proceso.
				 PCB *p_padre:		Puntero al proceso padre.
Salida:      Ninguna.
-------------------------------------------------------------------------- */
void inic_pcb(pcb_act,name_proc,id_proc,prio,p_padre,numlin,numvars,numsems)
PCB *pcb_act,*p_padre;
char *name_proc;
int id_proc,prio,numlin,numvars,numsems;
{  register i;
	strcpy(pcb_act->name_proc, name_proc);
	pcb_act->DS = (unsigned int*)((char*)(pcb_act->CS)+numlin*sizeof(instruccion));
	pcb_act->sems = (semaforo*)((char*)(pcb_act->DS)+numvars*sizeof(unsigned int));
	pcb_act->var_con = (hls_ptr*)((char *)(pcb_act->sems)+numsems*sizeof(semaforo));
	pcb_act->id_proc = id_proc;
	pcb_act->prio = prio;
	pcb_act->pcb_padre = p_padre;
	pcb_act->hijos = 0;
	pcb_act->edo_proc = LISTO;
	pcb_act->rec_solic = 0;
	pcb_act->num_solic = 0;
	pcb_act->SF = pcb_act->ZF = 0;
	for (i=0;i<NUM_REGS;i++)
		pcb_act->reg_maq[i]=0;
	for (i=0;i<NUM_RECS;i++)
		pcb_act->rec[i]=0;
}

/* ------------------------------------------------------------------------
Funci¢n    : ejecuta_proc
Objetivo   : Enviar a ejecuci¢n un proceso, a trav‚s de un an lisis del
				 timer.
Par metros : Ninguno.
Salida:      Ninguna
-------------------------------------------------------------------------- */
void ejecuta_proc()
{  if(!timer)
	{	if(pcb_exec) tiempo_terminado();
		op = check_command();
		if(!op) return;
		if(!empty_ls(l_listos))
			pcb_exec = despachar();
	}
	switch(ejec_instr(pcb_exec))
		{	case FUE_BLOQ:
				timer=0;
				pcb_exec=NULL;
				break;
			case 0:
				timer = 0;
				free_proc(pcb_exec);
				pcb_exec=NULL;
				break;
			default:
				timer--;
		}
}

/* ------------------------------------------------------------------------
Funci¢n    : tiempo_terminado
Objetivo   : Sacar a un proceso de ejecuci¢n y mandarlo  al estado de
				 suspendido listo.
Par metros : Ninguno.
Salida:      Ninguna.
-------------------------------------------------------------------------- */
void tiempo_terminado()
{  guarda_regs(pcb_exec);
	inserta_ordenado(l_listos, pcb_exec);
	pcb_exec->edo_proc = LISTO;
	move_proc(EXEC_2_LISTO, pcb_exec->id_proc);
	pcb_exec = NULL;
}

/* ------------------------------------------------------------------------
Funci¢n    : guarda_regs
Objetivo   : Grabar el estado de los registros para un proceso en su PCB.
Par metros : PCB *pcb_act: Puntero al PCB del proceso.
Salida:      Ninguna.
-------------------------------------------------------------------------- */
void guarda_regs(PCB *pcb_act)
{	register i;
	pcb_act->ZF = ZF;
	pcb_act->SF = SF;
	for(i=0; i<NUM_REGS; i++)
		pcb_act->reg_maq[i]=regs[i];
}

/* ------------------------------------------------------------------------
Funci¢n    : despachar
Objetivo   : Sacar un proceso de LISTOS y enviarlo a ejecuci¢n.
Par metros : Ninguno.
Salida 	  : PCB * Puntero al PCB del proceso despachado.
-------------------------------------------------------------------------- */
PCB *despachar()
{	PCB *pcb_act;
	pcb_act = (PCB*)pop_sls(l_listos);
	pcb_act->edo_proc = EJECUCION;
	restablece_regs(pcb_act);
	timer = INIC_TIMER;
	move_proc(LISTO_2_EXEC, pcb_act->id_proc);
	return pcb_act;
}

/* ------------------------------------------------------------------------
Funci¢n    : restablece_regs
Objetivo   : Restablece el estado de los registros para un proceso desde
				 su PCB.
Par metros : PCB *pcb_act: Puntero al PCB del proceso.
Salida:      Ninguna.
-------------------------------------------------------------------------- */
void restablece_regs(PCB *pcb_act)
{	register i;
	SF = pcb_act->SF;
	ZF = pcb_act->ZF;
	for(i=0; i<NUM_REGS; i++)
		regs[i] = pcb_act->reg_maq[i];
	paint_registers();
}

/* ------------------------------------------------------------------------
Funci¢n    : free_proc
Objetivo   : Le da en lo que m s le duele (su madre) a un proceso soltando
				 sus recursos y liberando su memoria.
Par metros : PCB *pcb_act: Puntero al PCB del proceso.
Salida:      Ninguna.
-------------------------------------------------------------------------- */
void free_proc(PCB *pcb_act)
{  register i;
	if(pcb_act->pcb_padre)
	{	for (i=0;i<NUM_RECS;i++)
		{	recursos[i] += pcb_act->rec[i] - pcb_act->pcb_padre->rec[i];
			paint_recursos(i, pcb_act->rec[i]);
		}
		pcb_act->pcb_padre = NULL;
	}
	else
	{	for (i=0;i<NUM_RECS;i++)
		{	recursos[i] += pcb_act->rec[i];
			paint_recursos(i, pcb_act->rec[i]);
		}
		myfree(pcb_act->CS);
		free(pcb_act->PRG);
	}
	free(pcb_act);
}

/* ------------------------------------------------------------------------
Funci¢n    : compara
Objetivo   : Compara dos nodos de acuerdo a prioridad.
Par metros : PCB *nodo1
			  : PCB *nodo2
Salida:    : 1 nodo1 primero a nodo2
				 0 no importa el orden
				 -1 nodo2 antes que nodo1
-------------------------------------------------------------------------- */
int compara(PCB *nodo1, PCB*nodo2)
{	if(nodo1->prio > nodo2->prio) return 1;
	else return -1;
}

/* ------------------------------------------------------------------------
Funci¢n    : inserta_ordenado
Objetivo   : Inserta un nodo ordenado en una lista, de acuerdo a la prio-
				 ridad que ‚ste tenga.
Par metros : hls_ptr lista:	Lista en que se habr  de insertar
			  : PCB *pcb_act:    proceso a insertar.
Salida:    : Ninguna.
-------------------------------------------------------------------------- */
void inserta_ordenado(hls_ptr lista, PCB *pcb_act)
{	PCB *pcb_aux;
	for_ls(lista, pcb_aux, PCB)
		if(pcb_aux->prio < pcb_act->prio)
		{	insert_before_sls(lista, (ls_ptr)pcb_aux, (ls_ptr)pcb_act);
			return;
		}
	insert_sls(lista, (ls_ptr)pcb_act);
}

/* ------------------------------------------------------------------------
Funci¢n    : suspende_proc
Objetivo   : Suspender un proceso mand ndolo a la lista de suspendidos
				 listos.
Par metros : unsigned int id_proc:	Identificador de un proceso.
Salida     : Ninguna.
-------------------------------------------------------------------------- */
void suspende_proc(unsigned int id_proc)
{	PCB *pcb_aux;
	PCTRL *p_control;
	if(pcb_exec->id_proc == id_proc)
	{	guarda_regs(pcb_exec);
/*		Aqu¡ debiera liberar recursos		*/
		insert_sls(l_slistos, (ls_ptr)pcb_exec);
		pcb_exec->edo_proc = SLISTO;
		pcb_exec = NULL;
		timer = 0;
		move_proc(EXEC_2_SLISTO, id_proc);
	}
	else
	{	pcb_aux = (PCB*) looking_for_ls(l_listos, busca_proc, &id_proc);
		if(pcb_aux)
		{	pcb_aux = (PCB*) delete_sls(l_listos, (ls_ptr) pcb_aux);
/*			Aqu¡ debiera liberar recursos		*/
			insert_sls(l_slistos, (ls_ptr)pcb_aux);
			pcb_aux->edo_proc = SLISTO;
			move_proc(LISTO_2_SLISTO, id_proc);
		}
		else
		{	p_control =(PCTRL*)looking_for_ls(l_bloqueados, busca_pctrl, &id_proc);
			if(p_control)
			{	p_control=(PCTRL*) delete_sls(l_bloqueados, (ls_ptr) p_control);
				insert_sls(l_sbloqueados, (ls_ptr) p_control);
				p_control->pcb->edo_proc = SBLOQUEADO;
				move_proc(BLOQ_2_SBLOQ, id_proc);
			}
			else
				error("Proceso No Localizado");
		}
	}
}

/* ------------------------------------------------------------------------
Funci¢n    : reanuda_proc
Objetivo   : Reanudar un proceso, previamente suspendido.
Par metros : unsigned int id_proc:	Identificador de un proceso.
Salida	  : Ninguna.
-------------------------------------------------------------------------- */
void reanuda_proc(unsigned int id_proc)
{	PCB *pcb_aux;
	PCTRL *p_control;
	pcb_aux = (PCB*) looking_for_ls(l_slistos, busca_proc, &id_proc);
	if(pcb_aux)
		if (pcb_aux->prio >= syspri)
		{	pcb_aux = (PCB*) delete_sls(l_slistos, (ls_ptr)pcb_aux);
/*			Aqu¡ debiera restablecer recursos	*/
			inserta_ordenado(l_listos, pcb_aux);
			pcb_aux->edo_proc = LISTO;
			move_proc(SLISTO_2_LISTO, id_proc);
		}
		else
			error("Proceso No Cumple Prioridad");
	else
	{	p_control =(PCTRL*)looking_for_ls(l_sbloqueados, busca_pctrl, &id_proc);
		if(p_control)
			if(p_control->pcb->prio >= syspri)
			{	p_control=(PCTRL*) delete_sls(l_sbloqueados, (ls_ptr) p_control);
				insert_sls(l_bloqueados, (ls_ptr) p_control);
				p_control->pcb->edo_proc = BLOQUEADO;
				move_proc(SBLOQ_2_BLOQ, id_proc);
			}
			else
				error("Proceso No Cumple Prioridad");
		else
			error("Proceso No Localizado");
	}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : pri_proc.
Objetivo   : Cambia la prioridad de un proceso, verificando contra la
				 prioridad del sistema, tomando las acciones necesarias.
Par metros : unsigned int id_proc = Identificador del proceso,
				 unsigned int nva_pri = Nueva Prioridad.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void pri_proc(unsigned int id_proc,unsigned int nva_pri)
{  PCTRL *p_control;
	PCB *pcb_aux;
	pcb_aux = (PCB *) looking_for_ls(l_listos,busca_proc,&id_proc);
	if (pcb_aux)
	{	pcb_aux->prio = nva_pri;
		if (nva_pri < syspri)
			suspende_proc(id_proc);
	}
	else
		if (pcb_exec->id_proc == id_proc)
		{	pcb_exec->prio = nva_pri;
			if (nva_pri < syspri)
				suspende_proc(id_proc);
		}
		else
		{	pcb_aux = (PCB *) looking_for_ls(l_slistos,busca_proc,&id_proc);
			if (pcb_aux)
			{	pcb_aux->prio = nva_pri;
				if (nva_pri >= syspri)
					reanuda_proc(id_proc);
			}
			else
			{	p_control =(PCTRL*)looking_for_ls(l_bloqueados, busca_pctrl, &id_proc);
				if(p_control)
				{	p_control->pcb->prio = nva_pri;
					if (nva_pri < syspri)
						suspende_proc(id_proc);
				}
				else
				{	p_control =(PCTRL*)looking_for_ls(l_sbloqueados, busca_pctrl, &id_proc);
					if(p_control)
					{	p_control->pcb->prio = nva_pri;
						if (nva_pri >= syspri)
							reanuda_proc(id_proc);
					}
					else
						error ("Proceso No Localizado");
				}
			}
		}
}

/* ------------------------------------------------------------------------
Funci¢n    : pri_sys
Objetivo   : Cambia la prioridad del sistema.
Par metros : unsigned int nva_pri:	Nueva prioridad del sistema.
Salida	  : Ninguna.
-------------------------------------------------------------------------- */
void pri_sys(unsigned int nva_pri)
{	PCB *pcb_aux, *pcb_aux2;
	PCTRL *p_control, *p_control2;
	register i;
	syspri = nva_pri;
	if(pcb_exec->prio < nva_pri)
		suspende_proc(pcb_exec->id_proc);
	for(pcb_aux=(PCB*) first_ls(l_listos); pcb_aux;)
	{  pcb_aux2 = (PCB*) next_sls(pcb_aux);
		if(pcb_aux->prio < nva_pri)
			suspende_proc(pcb_aux->id_proc);
		pcb_aux = pcb_aux2;
	}
	for(pcb_aux=(PCB*) first_ls(l_slistos); pcb_aux;)
	{  pcb_aux2 = (PCB*) next_sls(pcb_aux);
		if(pcb_aux->prio >= nva_pri)
			reanuda_proc(pcb_aux->id_proc);
		pcb_aux = pcb_aux2;
	}
	for(p_control=(PCTRL*) first_ls(l_bloqueados); p_control;)
	{  p_control2 = (PCTRL*) next_sls(p_control);
		if(p_control->pcb->prio < nva_pri)
			suspende_proc(p_control->pcb->id_proc);
		p_control = p_control2;
	}
	for(p_control=(PCTRL*) first_ls(l_sbloqueados); p_control;)
	{  p_control2 = (PCTRL*) next_sls(p_control);
		if(p_control->pcb->prio >= nva_pri)
			reanuda_proc(p_control->pcb->id_proc);
		p_control = p_control2;
	}
}

/* ------------------------------------------------------------------------
Funci¢n    : kill_proc
Objetivo   : Elimina totalmente un proceso.
Par metros : unsigned int id_proc:	Identificador de un proceso
Salida	  : Ninguna.
-------------------------------------------------------------------------- */
void kill_proc(unsigned int id_proc)
{	PCB *pcb_aux;
	if(pcb_exec->id_proc == id_proc)
	{	pcb_aux = pcb_exec;
		pcb_exec = NULL;
		timer = 0;
	}
	else
	{	pcb_aux = (PCB*) looking_for_ls(l_listos, busca_proc, &id_proc);
		if(pcb_aux)
			pcb_aux = (PCB*) delete_sls(l_listos, (ls_ptr)pcb_aux);
		else
		{	pcb_aux = (PCB*) looking_for_ls(l_slistos, busca_proc, &id_proc);
			if(pcb_aux)
				pcb_aux = (PCB*) delete_sls(l_slistos, (ls_ptr)pcb_aux);
			else
			{	error("Proceso No Localizado");
				return;
			}
		}
	}
	if(pcb_aux->pcb_padre)
	{	pcb_aux->pcb_padre->hijos--;
		if(!pcb_aux->pcb_padre->hijos)
		{	inserta_ordenado(l_listos, pcb_aux->pcb_padre);
			pcb_aux->pcb_padre->edo_proc = LISTO;
		}
	}
	free_proc(pcb_aux);
}

/* ------------------------------------------------------------------------
Funci¢n    : busca_proc
Objetivo   : Compara el identificador de un proceso contra otro
				 identificador (Usada por looking_for_ls).
Par metros : PCB *pcb_act:	Puntero a un proceso
				 unsigned int *id_proc:	Puntero a identificador de un proceso
												(Dato de prueba).
Salida     : 1 -> El proceso es el que ando buscando.
				 0 -> Caso contrario.
-------------------------------------------------------------------------- */
int busca_proc(PCB *pcb_act, unsigned int *id_proc)
{	return (pcb_act->id_proc == *id_proc) ? 1:0;
}

/* ------------------------------------------------------------------------
Funci¢n    : busca_pctrl
Objetivo   : Compara el identificador de un proceso contra otro
				 identificador (Usada por looking_for_ls).
Par metros : PCTRL *p_control:	Puntero a un proceso
				 unsigned int *id_proc:	Puntero a identificador de un proceso
												(Dato de prueba).
Salida     : 1 -> El proceso es el que ando buscando.
				 0 -> Caso contrario.
-------------------------------------------------------------------------- */
int busca_pctrl(PCTRL *p_control, unsigned int *id_proc)
{	return (p_control->pcb->id_proc == *id_proc) ? 1:0;
}

/* ------------------------------------------------------------------------
Funci¢n    : free_pcblist
Objetivo   : Libera un lista de pcb's.
Par metros : hls_ptr lista:  Lista a borrar.
Salida     : Ninguna.
-------------------------------------------------------------------------- */
void free_pcblist(hls_ptr lista)
{	PCB *pcb_aux;
	for_ls(lista, pcb_aux, PCB)
	{	if(!pcb_aux->pcb_padre)
		{/* myfree(pcb_aux->CS);*/
			free(pcb_aux->PRG);
		}
	}
	destroy_ls(&lista);
}

/* ------------------------------------------------------------------------
Funci¢n    : semaphore_p
Objetivo   : Ejecuta la operaci¢n P sobre un sem foro.
Par metros : unsigned int num_sem:	N£mero de sem foro.
				 PCB	*pcb_act:		Direcci¢n del pcb susceptible a bloqueo.
Salida     : FUE_BLOQ ->	Si el proceso fue bloqueado.
				 1			 ->	En caso contrario.
-------------------------------------------------------------------------- */
unsigned int semaphore_p(unsigned int num_sem, PCB *pcb_act)
{  PCTRL *p_control;
	if(SEMAFOROS[num_sem].edo > 0)
		SEMAFOROS[num_sem].edo--;
	else
	{	pcb_act->edo_proc = BLOQUEADO;
		move_proc(EXEC_2_BLOQ, pcb_act->id_proc);
		if((p_control = (PCTRL*) malloc(sizeof(PCTRL)))==NULL)
			error("Memoria Agotada");
		else
		{	p_control->pcb = pcb_act;
			insert_sls(l_bloqueados, (ls_ptr) p_control);
		}
		insert_sls(SEMAFOROS[num_sem].l_sem, (ls_ptr)pcb_act);
		return FUE_BLOQ;
	}
	return 1;
}

/* ------------------------------------------------------------------------
Funci¢n    : semaphore_v
Objetivo   : Ejecuta la operaci¢n V' sobre un sem foro.
Par metros : unsigned int num_sem:	N£mero de sem foro.
				 unsigned char v_prima: Ejecutar v' ¢ v.
Salida     : Ninguno.
-------------------------------------------------------------------------- */
void semaphore_v(unsigned int num_sem,unsigned char v_prima,PCB *pcb_act)
{  PCB *pcb_aux;
	PCTRL	*p_control;
	if(!empty_ls(SEMAFOROS[num_sem].l_sem))
	{	pcb_aux = (PCB*)pop_sls(SEMAFOROS[num_sem].l_sem);
		if(pcb_aux->edo_proc == BLOQUEADO)
		{	pcb_aux->edo_proc = LISTO;
			p_control=(PCTRL*)looking_for_ls(l_bloqueados, busca_pctrl, &pcb_aux->id_proc);
			move_proc(BLOQ_2_LISTO, pcb_aux->id_proc);
			delete_sls(l_bloqueados, (ls_ptr) p_control);
			free(p_control);
			inserta_ordenado(l_listos, pcb_aux);
		}
		else
		{	pcb_aux->edo_proc = SLISTO;
			p_control = (PCTRL*)looking_for_ls(l_sbloqueados, busca_pctrl, &pcb_aux->id_proc);
			move_proc(SBLOQ_2_SLISTO, pcb_aux->id_proc);
			delete_sls(l_sbloqueados, (ls_ptr) p_control);
			free(p_control);
			insert_sls(l_slistos, (ls_ptr) pcb_aux);
		}
	}
	else
		if(v_prima)
			SEMAFOROS[num_sem].edo = 1;
		else
			SEMAFOROS[num_sem].edo++;
}

/*--------------------------------------------------------------------------
FUNCION 	 : libera_recursos
OBJETIVO  : Libera los recursos que tiene asignados un proceso.
PARAMETROS: PCB *pcb_act
SALIDA    : Ninguna
---------------------------------------------------------------------------*/
void libera_recursos(PCB *pcb_act)
{ 	register i;
	if(pcb_act->pcb_padre)
		for (i=0;i<NUM_RECS;i++)
		{	recursos[i] += pcb_act->rec[i] - pcb_act->pcb_padre->rec[i];
			paint_recursos(i, pcb_act->rec[i]);
			pcb_act->rec[i] = 0;
		}
	else
		for (i=0;i<NUM_RECS;i++)
		{	recursos[i] += pcb_act->rec[i];
			paint_recursos(i, pcb_act->rec[i]);
         pcb_act->rec[i] = 0;
		}
}

/* ------------------------------------------------------------------------
Funci¢n    : ejec_instr
Objetivo   : Ejecutar una instrucci¢n de un proceso.
Par metros : PCB *pcb_act: PCB del proceso del cual se ejecutar  la
									instrucci¢n.
Salida     : 0 -> Si fue la £ltima instrucci¢n del proceso(END o ENDP si es hijo).
				 FUE_BLOQ -> Si la victima de la solucion de un interbloqueo fue
								 el proceso en ejecuci¢n, o si se bloqueo el proceso.
				 1 -> Ejecuci¢n normal.
-------------------------------------------------------------------------- */
unsigned int ejec_instr(PCB *pcb_act)
{  PCB *pcb_aux;
	PCTRL *p_control;
	unsigned int inst, aux;
	static int intrblok_counter = 0;

	if(++intrblok_counter == INTRBLOK_TIME)
	{ intrblok();
	  intrblok_counter = 0;
	}
	if(!pcb_exec)
		return FUE_BLOQ;
	write_instruc(CODE);
	delay(300);
	write_instruc("                  ");
	switch(INSTR)
	{	case ADD:
			regs[OP1] += OP2;
			pcb_act->IP++;
			break;
		case CALL:
			pcb_act->SS = pcb_act->IP+1;
			pcb_act->IP = OP1;
			break;
		case CALLM:
			pcb_act->SS = pcb_act->IP+1;
			pcb_act->IP = OP1;
			if(semaphore_p(0,pcb_act)==FUE_BLOQ)
				return FUE_BLOQ;
			break;
		case CMP:
			if(regs[OP1] == OP2)
			{	CLEAR_ZF;
				CLEAR_SF;
			}
			else
			{	SET_ZF;
				if(regs[OP1] > OP2) CLEAR_SF;
				else SET_SF;
			}
			pcb_act->IP++;
			break;
		case DEC:
			regs[OP1]--;
			pcb_act->IP++;
			break;
		case EMPTY:
			if (empty_ls(VARCON[OP1]))
				{
				CLEAR_ZF;
				CLEAR_SF;
				}
			else
				{
				SET_ZF;
				CLEAR_SF;
				}
			break;
		case END:
			return 0;
		case ENDM:
			pcb_act->IP = pcb_act->SS;
			semaphore_v(0,1,pcb_act);
			break;
		case ENDP:
			if(!pcb_act->pcb_padre)
				pcb_act->IP = pcb_act->SS;
			else
			{	pcb_act->pcb_padre->hijos--;
				if(!pcb_act->pcb_padre->hijos)
				{	pcb_act->pcb_padre->edo_proc = LISTO;
					move_proc(BLOQ_2_LISTO, pcb_act->pcb_padre->id_proc);
					inserta_ordenado(l_listos, pcb_act->pcb_padre);
				}
				return 0;
			}
			break;
		case FRE:
			if (OP2<=pcb_act->rec[OP1])
			{	recursos[OP1] += OP2;
				pcb_act->rec[OP1] -= OP2;
				paint_recursos(OP1,OP2);
			}
			pcb_act->IP++;
			break;
		case GET:
			if(OP2 > recursos[OP1])
			{	CLEAR_ZF;
				CLEAR_SF;
				pcb_act->num_solic++;
				if(pcb_act->rec_solic != OP1)
					pcb_act->rec_solic = OP1;
			}
			else
			{	SET_ZF;
				pcb_act->num_solic = 0;
				pcb_act->rec[OP1] += OP2;
				recursos[OP1] -= OP2;
				paint_recursos(OP1,-OP2);
			}
			pcb_act->IP++;
			break;
		case INC:
			regs[OP1]++;
			pcb_act->IP++;
			break;
		case JEQ:
			if(!ZF) pcb_act->IP = OP1;
			else pcb_act->IP++;
			break;
		case JGE:
			if(ZF && !SF) pcb_act->IP = OP1;
			else pcb_act->IP++;
			break;
		case JLE:
			if(ZF && SF) pcb_act->IP = OP1;
			else pcb_act->IP++;
			break;
		case JMP:
			pcb_act->IP = OP1;
			break;
		case JNE:
			if(ZF) pcb_act->IP = OP1;
			else pcb_act->IP++;
			break;
		case MOV:
			regs[OP1] = OP2;
			pcb_act->IP++;
			break;
		case MOVR:
			regs[OP1] = regs[OP2];
			pcb_act->IP++;
			break;
		case MOVRV:
			pcb_act->DS[OP1] = regs[OP2];
			pcb_act->IP++;
			break;
		case MOVV:
			regs[OP1] = pcb_act->DS[OP2];
			pcb_act->IP++;
			break;
		case NOP:
			pcb_act->IP++;
			break;
		case P:
			aux = semaphore_p(OP1,pcb_act);
			pcb_act->IP++;
			if( aux == FUE_BLOQ)
				return FUE_BLOQ;
			break;
		case PARBEGIN:
			pcb_act->IP++;
			while(INSTR != PAREND)
			{	if((pcb_aux=(PCB*)malloc(sizeof(PCB)))==NULL)
					error("Memoria Agotada");
				else
				{ *(pcb_aux) = *(pcb_act);
					pcb_aux->id_proc = proc_counter++;
					pcb_aux->IP = OP1;
					pcb_aux->pcb_padre = pcb_act;
					pcb_aux->edo_proc = LISTO;
					inserta_ordenado(l_listos, pcb_aux);
					move_proc(EXEC_2_LISTO, pcb_aux->id_proc);
					pcb_act->hijos++;
				}
				pcb_act->IP++;
			}
			pcb_act->IP++;
			pcb_act->edo_proc = BLOQUEADO;
			move_proc(EXEC_2_BLOQ, pcb_act->id_proc);
			return FUE_BLOQ;
		case SIGNAL:
			if (!empty_ls(VARCON[OP1]))
				{
				pcb_aux = pop_sls(VARCON[OP1]);
				if (pcb_aux->edo_proc == BLOQUEADO)
					{
					pcb_aux->edo_proc = LISTO;
					move_proc(BLOQ_2_LISTO,pcb_aux->id_proc);
					p_control=(PCTRL *)looking_for_ls(l_bloqueados,busca_pctrl,&pcb_aux->id_proc);
					delete_sls(l_bloqueados,(ls_ptr)p_control);
					free(p_control);
					inserta_ordenado(l_listos,pcb_aux);
					}
				else
					{
					pcb_aux->edo_proc = SLISTO;
					move_proc(SBLOQ_2_SLISTO,pcb_aux->id_proc);
					p_control=(PCTRL *)looking_for_ls(l_sbloqueados,busca_pctrl,&pcb_aux->id_proc);
					delete_sls(l_bloqueados,(ls_ptr)p_control);
					free(p_control);
					insert_sls(l_slistos,(ls_ptr)pcb_aux);
					}
				}
			else
				semaphore_v(0,1,pcb_act);
			pcb_act->IP = pcb_act->SS;
			break;
		case SUB:
			regs[OP1] -= OP2;
			pcb_act->IP++;
			break;
		case SET:
			SEMAFOROS[OP1].edo = OP2;
			pcb_act->IP++;
			break;
		case V:
			semaphore_v(OP1,0,pcb_act);
			pcb_act->IP++;
			break;
		case WAIT:
			pcb_act->edo_proc = BLOQUEADO;
			move_proc(EXEC_2_BLOQ,pcb_act->id_proc);
			push_sls(VARCON[OP1],(ls_ptr)pcb_act);
			p_control = (PCTRL *)node_ls(NULL,PCTRL);
			if(p_control)
				{
				p_control->pcb = pcb_act;
				insert_sls(l_bloqueados,(ls_ptr)p_control);
				}
			else
				error("Memoria Agotada");
			semaphore_v(0,1,pcb_act);
			pcb_act->IP++;
			return FUE_BLOQ;
	}
	paint_registers();
	paint_flags();
	return 1;
}
