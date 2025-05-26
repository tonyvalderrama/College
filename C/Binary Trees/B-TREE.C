/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
							  INSTITUTO TECNOLOGICO DE MOREIA
											Bases de Datos

Archivo    : B-TREE.C
Objetivo   : La funci¢n que controla la ejecuci¢n del sistema (el main()).
Dise¤o     : Jos‚ Antonio Valderrama Ramos
				 Jorge E. Aparicio Mayorga.
Implemento : Los mismos.
Versi¢n    : 1.00 (Marzo 1994).
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <graphics.h>
#include "btree.h"
#include "dsls.h"

typedef struct
	{
	HEADSLS;
	NODE *nodo;
	}ANT;

extern NODOS[];
NODE *root;
hls_ptr l_ant;

void main()
{
	int i,accion;
	ANT node_act,*node_aux;

	l_ant=create_ls(NULL);
	node_act.nodo = root = NULL;
	ini_gui();
	for (i=0;i<6;i++)
		NODOS[i]=NULL;
	while ((accion = que_hice()) != 10)
		{
		switch(accion)
			{
			case 1:
				high();
				node_act.nodo = root;
				break;
			case 2:
				down();
				node_act.nodo = root;
				break;
			case 3:
				consul();
				break;
			case 4:
				if (!empty_ls(l_ant))
					{
					node_act = *((ANT *) pop_sls(l_ant));
					desp_peh(node_act.nodo);
					}
				break;
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				if (NODOS[accion-4])
					{
					node_aux = (ANT *) node_ls(NULL,ANT);
					*node_aux = node_act;
					push_sls(l_ant,(ls_ptr)node_aux);
					node_act.nodo = NODOS[accion-4];
					desp_peh(node_act.nodo);
					}
				break;
			}
		}
	destroy_ls(&l_ant);
	closegraph();
}
