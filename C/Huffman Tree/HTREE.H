/*--------------------------------------------------------------------------
		       INSTITUTO TECNOLOGICO DE MORELIA
		    INGENIERIA EN SISTEMAS COMPUTACIONALES
			  Algoritmos Computacionales

M¢dulo     : prototipos para crear  rboles Huffman.
Archivo    : HTREE.H.
Objetivo   : definici¢n de tipos necesarios para hacer programas que
	     usan el algoritmo de Huffman.
Dise¤o     : Al Stevens (Dr. Dobb's Journal).
Implemento : Jos‚ Antonio Valderrama Ramos.
Versi¢n    : Diciembre 1992.
--------------------------------------------------------------------------*/

#ifndef HTREE

#define HTREE

typedef unsigned int BYTECOUNTER;

struct htree                             /* Estructura del  rbol Huffman. */
   {
   unsigned char ch;                                /* Valor del caracter */
   BYTECOUNTER cnt;                            /* Frecuencia del caracter */
   struct htree *parent;                       /* Apuntador al nodo padre */
   struct htree *right;                      /* Apuntador al hijo derecho */
   struct htree *left;                     /* Apuntador al hijo izquierdo */
   };

extern struct htree ht[];
extern struct htree *root;

void buildtree(void);      /* Prototipo de la funci¢n que genera el  rbol */

#endif
