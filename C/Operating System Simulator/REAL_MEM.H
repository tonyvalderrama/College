/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
						 INGENIERIA EN SISTEMAS COMPUTACIONALES
										Sistemas Operativos
M¢dulo     : Definiciones para manejo de memoria.
Archivo    : real_mem.h
Objetivo   : Definiciones para el uso simulado de memoria real.
Dise¤o     : Jorge Eduardo Aparicio Mayorga  -  90120511-3,
				 Jos‚ Antonio Valderrama Ramos  -  90120509-9.
Implemento : Los mismos.
Versi¢n    : Mayo de 1994.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
#define TAM_MEM 1000
#define HUECO NULL
#define PROCESO 1

typedef struct
{	HEADDLS;
	PCB *hop;
	void *inicio;
	unsigned int tam;
} NODO_MEM;

typedef struct
{	HEADSLS;
	NODO_MEM *node_mem;
} CTRL_MEM;
