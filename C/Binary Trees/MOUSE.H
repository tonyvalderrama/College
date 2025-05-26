/*--------------------------------------------------------------------------
M¢dulo    : Manejo de la Pantalla.
Archivo   : MOUSE.H.
Objetivo  : Contiene las definiciones de funciones y arreglos para manejar
	    el rat¢n.
Dise¤o    : Jos‚ Antonio Valderrama Ramos.
Implemento: El mismo.
Versi¢n   : Noviembre '93
--------------------------------------------------------------------------*/

#ifndef MICKEY_MOUSE
   #define MICKEY_MOUSE
   /* Macros. */
   #define SHOW 0x1
   #define HIDE 0x2
   #define _STAR 1
   #define _SMILY 2
   #define _WATCH 3
   #define _FINGER 4
   #define _INTSIGN 5
   #define _DISK 6
	#define _HEART 7
	#define _PATYYYO 8
	/* Prototipos de funciones. */
   int _inic_mouse();
   int _def_cur_graph(int);
   int _pos_status(int *,int *);
   void _show_hide_cur(int);
   void _pos_cur(int,int);
	void _cur_range(int,int,int,int);
	int _check_button(int *,int *,int);
#endif
