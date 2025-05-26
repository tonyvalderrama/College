/*--------------------------------------------------------------------
		  INSTITUTO TECNOLOGICO de morelia
		ACADEMIA DE SISTEMAS COMPUTACIONALES
			  CODIGO GENERICO


M¢dulo    : Listas gen‚ricas simples y dobles
Archivo   : DSLS.H
objetivo  : Librer¡a para construir y manejar listas ligadas,
	    simples o dobles, de manera gen‚rica

dise¤o    : Morales L¢pez Felipe

Implemento: Morales L¢pez Felipe

Versi¢n   : Enero/93
--------------------------------------------------------------------*/

#ifndef DSLS
#define DSLS
#include <stdio.h>
/*--------------------------------------------------------------------
		 Variables simb¢licas
--------------------------------------------------------------------*/
#define HEADDLS                 dls_ptr  _next, _prev
#define HEADSLS                 dls_ptr  _next
#define DSLS_D                  2
#define DSLS_S                  1

/*--------------------------------------------------------------------
	  Definici¢n de tipos y variables externas
--------------------------------------------------------------------*/
typedef struct _headdls *dls_ptr, DLHEAD, *hls_ptr, LSHEAD;
typedef struct _headsls *ls_ptr, SLHEAD;

struct _headdls{
     dls_ptr _next;
     dls_ptr _prev;
};

struct _headsls{
     ls_ptr _next;
};

/*--------------------------------------------------------------------
		      Macros
--------------------------------------------------------------------*/
#define break_list_dls(a,b,c)   break_list_ls(a,(ls_ptr)b,c, DSLS_D)
#define break_list_sls(a,b,c)   break_list_ls(a,b,c, DSLS_S)
#define change_node_dls(a,b,c)  \
	CHANGE_node_ls((char *)a,(char*)b,sizeof(c),sizeof(DLHEAD))
#define change_node_sls(a,b,c)  \
	CHANGE_node_ls((char *)a,(char*)b,sizeof(c),sizeof(SLHEAD))
#define copy_list_dls(a,b,c)    COPY_list_ls(a,b,sizeof(c),DSLS_D)
#define copy_list_sls(a,b,c)    COPY_list_ls(a,b,sizeof(c),DSLS_S)
#define copy_node_dls(a,b,c)   \
	COPY_node_ls((char *)a,(char *)b,sizeof(c),sizeof(DLHEAD))
#define copy_node_sls(a,b,c)   \
	COPY_node_ls((char *)a,(char *)b,sizeof(c),sizeof(SLHEAD))

#define first_ls(lista)         (lista)->_next
#define for_ls(l,n,tipo)        for(n=(tipo *)l->_next;n;n=(tipo *)n->_next)
#define for_ltof_dls(l,n,tipo)  for(n=(tipo *)l->_prev;n;n=(tipo *)n->_prev)
#define empty_ls(a)             !((a)->_next)
#define last_ls(lista)          (lista)->_prev
#define next_sls(nodo)          (nodo)->_next
#define next_dls(nodo)          (nodo)->_next
#define node_ls(a,b)            NODE_ls(a, sizeof(b))
#define prev_dls(a)              (a)->_prev
#ifndef NO_exist
  #define exist(a)              a
  #define NO_exist(a)           !(a)
#endif

/*--------------------------------------------------------------------
		Prototipos de las funciones
--------------------------------------------------------------------*/
extern void      break_list_ls(hls_ptr, ls_ptr, hls_ptr, int);
extern void*     CHANGE_node_ls(char *, char *,int,int);
extern void*     COPY_list_ls(hls_ptr, hls_ptr,int,int);
extern void      COPY_node_ls(char *, char *,int,int);
extern dls_ptr   create_ls(hls_ptr);
extern void*     delete_dls(hls_ptr, dls_ptr);
extern void*     delete_sls(hls_ptr, ls_ptr);
extern void*     delete_last_dls(hls_ptr);
extern void*     delete_last_sls(hls_ptr);
extern void      destroy_ls(hls_ptr *);
extern void      destroy_all_ls(hls_ptr *, void (*)());
extern void      insert_dls(hls_ptr, dls_ptr);
extern void      insert_sls(hls_ptr, ls_ptr);
extern void      insert_after_dls(hls_ptr, dls_ptr, dls_ptr);
extern void      insert_after_sls(hls_ptr, ls_ptr,  ls_ptr);
extern void      insert_before_dls(hls_ptr,dls_ptr,dls_ptr);
extern void      insert_before_sls(hls_ptr, ls_ptr, ls_ptr);
extern void      insert_list_dls(hls_ptr, hls_ptr);
extern void      insert_list_sls(hls_ptr, hls_ptr);
extern void      insert_list_after_dls(hls_ptr, dls_ptr, hls_ptr);
extern void      insert_list_after_sls(hls_ptr, ls_ptr, hls_ptr);
extern void      insert_list_before_dls(hls_ptr, dls_ptr, hls_ptr);
extern void      insert_list_before_sls(hls_ptr, ls_ptr, hls_ptr);
extern void      invert_dls(hls_ptr);
extern int       is_member_ls(hls_ptr, ls_ptr);
extern void*     looking_for_ls(hls_ptr, int (* )(),void *);
extern void      move_ls(hls_ptr, hls_ptr);
extern void*     NODE_ls();
extern void*     nth_node_ls(hls_ptr,int);
extern void*     pop_dls(hls_ptr);
extern void*     pop_sls(hls_ptr);
extern void      purge_ls(hls_ptr);
extern void      purge_all_ls(hls_ptr, void(* )());
extern void      push_dls(hls_ptr, dls_ptr);
extern void      push_sls(hls_ptr, ls_ptr);
extern void      push_list_dls(hls_ptr, hls_ptr);
extern void      push_list_sls(hls_ptr, hls_ptr);
extern void      sort_insert_dls(hls_ptr, dls_ptr, int (*)());
extern void      sort_insert_sls(hls_ptr, ls_ptr, int (*)());
#endif /*----------------   fin del archivo  -----------------------*/
