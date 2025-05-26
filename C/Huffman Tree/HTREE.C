/*--------------------------------------------------------------------------
		       INSTITUTO TECNOLOGICO DE MORELIA
		    INGENIERIA EN SISTEMAS COMPUTACIONALES
			  Algoritmos Computacionales

M¢dulo     : prototipo de una funci¢n (buildtree).
Archivo    : HTREE.C.
Objetivo   : funci¢n que crea un  rbol Huffman (una especie de trie).
Dise¤o     : Al Stevens (Dr. Dobb's Journal).
Implemento : Jos‚ Antonio Valderrama Ramos.
Versi¢n    : Diciembre 1992.
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "htree.h"

struct htree ht[512];
struct htree *root;

/*--------------------------------------------------------------------------
Funci¢n    : buildtree.
Objetivo   : contruye el  rbol de Huffman de un arreglo de frecuencias.
Par metros : ninguno.
Salida     : ninguna.
--------------------------------------------------------------------------*/
void buildtree()
{
   int treect = 256,i;
   while (1)                        /* Construcci¢n del  rbol de Huffman. */
      {
      struct htree *h1 = NULL, *h2 = NULL;
      for (i=0;i<treect;i++)       /* Encontrar las 2 frecuencias m s ... */
	 {                                               /* ... peque¤as. */
	 if (ht+i != h1)
	    {
	    if (ht[i].cnt > 0  &&  ht[i].parent == NULL)
	       {
	       if (h1 == NULL || ht[i].cnt < h1->cnt)
		  {
		  if (h2 == NULL || h1->cnt < h2->cnt)
		     h2 = h1;
		  h1 = ht+i;
		  }
	       else
		  if (h2 == NULL || ht[i].cnt < h2->cnt)
		     h2 = ht+i;
	       }
	    }
	 }
      if (h2 == NULL)
	 {
	 root = h1 ;
	 break;
	 }
      h1->parent = ht + treect;     /* Combinar los 2 nodos y a¤adir uno. */
      h2->parent = ht + treect;
      ht[treect].cnt = h1->cnt + h2->cnt;
      ht[treect].right = h1;
      ht[treect].left = h2;
      treect++;
      }
}
