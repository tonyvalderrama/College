/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
							  INSTITUTO TECNOLOGICO DE MOREIA
											Bases de Datos

Archivo    : BTREE.H
Objetivo   : Definici¢n de tipos y macros necesarias para el manejo
				 de los  rboles B.
Dise¤o     : Jos‚ Antonio Valderrama Ramos
				 Jorge E. Aparicio Mayorga.
Implemento : Los mismos.
Versi¢n    : 1.00 (Marzo 1994).
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#define M 2
#define MM 4
struct node {int cnt, key[MM]; struct node *ptr[MM+1];};
typedef struct node NODE;