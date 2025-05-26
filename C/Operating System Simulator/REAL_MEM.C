/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
						 INGENIERIA EN SISTEMAS COMPUTACIONALES
										Sistemas Operativos
M¢dulo     : Funciones del manejador de memoria para el Simulador.
Archivo    : real_mem.c
Objetivo   : Definici¢n de todas las funciones para el manejo de
				 memoria real.
Dise¤o     : Jorge Eduardo Aparicio Mayorga  -  90120511-3,
				 Jos‚ Antonio Valderrama Ramos  -  90120509-9.
Implemento : Los mismos.
Versi¢n    : Mayo 1994.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <stdio.h>
#include "dsls.h"
#include "so01.h"
#include "real_mem.h"

extern int proc_counter;
extern char* memoria;
hls_ptr l_admem, l_huecos;

/* -------------------------------------------------------------------------
FUNCION   : compara_tam
OBJETIVO  : Comparar el espacio disponible en dos NODOS_MEM
PARAMETROS: CTRL_MEM *nodo1, *nodo2
SALIDA    :  1 -> nodo1 debe ser primero que nodo2
				 0 -> no importa el orden
				-1 -> nodo1 debe ser despu‚s que nodo2
------------------------------------------------------------------------- */
int compara_tam(CTRL_MEM *nodo1, CTRL_MEM *nodo2)
{ if(nodo1->node_mem->tam < nodo2->node_mem->tam) return 1;
  if(nodo1->node_mem->tam == nodo2->node_mem->tam) return 0;
  return -1;
}

/* -------------------------------------------------------------------------
FUNCION   : inicia_mem
OBJETIVO  : Pedir la memoria del sistema y marcarla como un gran hueco.
PARAMETROS: Ninguno
SALIDA    : La direcci¢n de inicio de la memoria.
				NULL -> Si no hay espacio.
------------------------------------------------------------------------- */
void *inicia_mem()
{	NODO_MEM *n_mem;
	CTRL_MEM *nc_mem;
	void *aux;

	l_admem = create_ls(NULL);
	l_huecos = create_ls(NULL);
	aux = (void *)malloc(TAM_MEM);
   n_mem = (NODO_MEM*) node_ls(NULL, NODO_MEM);
	if(aux && n_mem)
	{	n_mem->hop = HUECO;
		n_mem->inicio = aux;
		n_mem->tam = TAM_MEM;
		nc_mem = (CTRL_MEM*) node_ls(NULL, CTRL_MEM);
		if(nc_mem)
		{  nc_mem->node_mem = n_mem;
			insert_dls(l_admem, (dls_ptr) n_mem);
			sort_insert_sls(l_huecos, (ls_ptr) nc_mem, compara_tam);
			return aux;
		}
	}
	return NULL;
}

/* -------------------------------------------------------------------------
FUNCION   : peor_ajuste
OBJETIVO  : Devolver el nodo con peor ajuste de la lista de huecos.
PARAMETROS: unsigned int tam -> Tama¤o requerido
SALIDA    : NODO_MEM* -> Nodo con peor ajuste
				NULL -> Si no alcanza la memoria.
------------------------------------------------------------------------- */
NODO_MEM *peor_ajuste(unsigned int tam)
{	CTRL_MEM *nc_mem;

	if(!empty_ls(l_huecos))
	{	nc_mem = (CTRL_MEM*) first_ls(l_huecos);
		if(nc_mem->node_mem->tam >= tam)
		{	nc_mem = pop_sls(l_huecos);
			return nc_mem->node_mem;
		}
	}
	return NULL;
}
/* -------------------------------------------------------------------------
FUNCION   : mymalloc
OBJETIVO  : Asignaci¢n de un espacio de memoria
PARAMETROS: unsigned int tam -> Longitud de memoria requerida
				PCB *pcb_act -> Proceso que ocupar  la memoria
SALIDA    : Un puntero void a la nueva localidad de memoria.
				NULL si no hay memoria.
------------------------------------------------------------------------- */
void *mymalloc(unsigned int tam, PCB *pcb_act)
{	NODO_MEM *n_mem, *n_memaux;
	CTRL_MEM *nc_memaux;

	n_mem = peor_ajuste(tam);
	if(n_mem)
	{	n_mem->hop = pcb_act;
		if(tam < n_mem->tam)
		{	n_memaux = (NODO_MEM*) node_ls(NULL, NODO_MEM);
			nc_memaux = (CTRL_MEM*) node_ls(NULL, CTRL_MEM);
			n_memaux->hop = HUECO;
			n_memaux->inicio = (void *)((char *)n_mem->inicio + tam);
			n_memaux->tam = n_mem->tam-tam;
			nc_memaux->node_mem = n_memaux;
			n_mem->tam = tam;
			insert_after_dls(l_admem, (dls_ptr) n_mem, (dls_ptr) n_memaux);
			sort_insert_sls(l_huecos, (ls_ptr) nc_memaux, compara_tam);
		}
		paint_mem((int)((char*)n_mem->inicio - memoria), n_mem->hop, tam);
		return n_mem->inicio;
	}
	return NULL;
}

/* -------------------------------------------------------------------------
FUNCION   : busca_nodo
OBJETIVO  : Probar un nodo y ver si es el buscado
PARAMETROS: NODO_MEM* nodo
				void *inicio
SALIDA    : 1 -> Nodo es el buscado
				0 -> Nodo no es el buscado.
------------------------------------------------------------------------- */
int busca_nodo(NODO_MEM* nodo, void *inicio)
{	return ((nodo->inicio == inicio) && (nodo->hop)) ? 1 : 0;
}

/* -------------------------------------------------------------------------
FUNCION   : busca_mem
OBJETIVO  : Buscar un bloque de memoria ocupado en la lista de
				administraci¢n
PARAMETROS: void *inicio -> Posici¢n inicial del bloque
SALIDA    : NODO_MEM* -> Nodo de la lista con info sobre el bloque
				NULL Si no es encontrado o est  libre.
------------------------------------------------------------------------- */
NODO_MEM *busca_mem(void *inicio)
{	NODO_MEM *n_mem;

	n_mem = (NODO_MEM*) looking_for_ls(l_admem, busca_nodo, inicio);
	return n_mem;
}

/* -------------------------------------------------------------------------
FUNCION   : busca_control
OBJETIVO  : Probar un nodo de control y ver si es el buscado
PARAMETROS: CTRL_MEM* n_ctrl
				NODO_MEM* node_mem -> Dato buscado
SALIDA    : 1 -> Nodo es el buscado
				0 -> Nodo no es el buscado.
------------------------------------------------------------------------- */
int busca_control(CTRL_MEM* n_ctrl, NODO_MEM *node_mem)
{	return (n_ctrl->node_mem == node_mem) ? 1 : 0;
}

/* -------------------------------------------------------------------------
FUNCION   : recoleccion_izq
OBJETIVO  : Juntar dos huecos en uno solo para la izquierda.
PARAMETROS: NODO_MEM *n_mem -> Nodo a partir del cual se har  la
										 recolecci¢n
SALIDA    : Ninguna.
------------------------------------------------------------------------- */
void recoleccion_izq(NODO_MEM *n_mem)
{  NODO_MEM *n_aux;
	CTRL_MEM *nc_aux;

	n_aux = (NODO_MEM*) prev_dls(n_mem);
	if(n_aux)
		if(n_aux->hop == HUECO)
		{	n_mem->inicio = n_aux->inicio;
			n_mem->tam += n_aux->tam;
			nc_aux = (CTRL_MEM*) looking_for_ls(l_huecos, busca_control, n_aux);
			delete_sls(l_huecos, (ls_ptr) nc_aux);
			delete_dls(l_admem, (dls_ptr) n_aux);
			free(nc_aux);
			free(n_aux);
		}
}

/* -------------------------------------------------------------------------
FUNCION   : recoleccion_der
OBJETIVO  : Juntar dos huecos en uno solo para la derecha.
PARAMETROS: NODO_MEM *n_mem -> Nodo a partir del cual se har  la
										 recolecci¢n
SALIDA    : Ninguna.
------------------------------------------------------------------------- */
void recoleccion_der(NODO_MEM *n_mem)
{  NODO_MEM *n_aux;
	CTRL_MEM *nc_aux;

	n_aux = (NODO_MEM*) next_dls(n_mem);
	if(n_aux)
		if(n_aux->hop == HUECO)
		{	n_mem->tam += n_aux->tam;
			nc_aux = (CTRL_MEM*) looking_for_ls(l_huecos, busca_control, n_aux);
			delete_sls(l_huecos, (ls_ptr) nc_aux);
			delete_dls(l_admem, (dls_ptr) n_aux);
			free(nc_aux);
			free(n_aux);
		}
}

/* -------------------------------------------------------------------------
FUNCION   : myfree
OBJETIVO  : Liberar un bloque de memoria pedido con mymalloc.
PARAMETROS: void *inicio -> Posici¢n inicial del bloque
SALIDA    : Ninguna
------------------------------------------------------------------------- */
void myfree(void *inicio)
{	NODO_MEM *n_mem;
	CTRL_MEM *nc_mem;

	n_mem = busca_mem(inicio);
	if(n_mem)
	{	n_mem->hop = HUECO;
		recoleccion_izq(n_mem);
		recoleccion_der(n_mem);
		nc_mem = (CTRL_MEM*) node_ls(NULL, CTRL_MEM);
		if(nc_mem)
		{  nc_mem->node_mem = n_mem;
			sort_insert_sls(l_huecos, (ls_ptr) nc_mem, compara_tam);
		}
		paint_mem((int)((char*)n_mem->inicio - memoria), 0, n_mem->tam);
	}
}
