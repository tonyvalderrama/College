/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
							INSTITUTO TECNOLOGICO DE MORELIA
						INGENIERIA EN SISTEMAS COMPUTACIONALES
										 Bases de Datos

M¢dulo     : Definiciones.
Archivo    : TONYHASH.C
Objetivo   : Definiciones requeridos para el funcionamiento de la tabla de
				 HASH.
Dise¤o     : Jorge E. Aparicio Mayorga (90120511-3), y
				 Jos‚ Antonio Valderrama Ramos (90120509-9).
Implemento : Los mismos.
Versi¢n    : 1.00 (Abril 1994).
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#ifndef TONYHASH
	#define TONYHASH

	#define TABLE_SIZE  401                  /* Definiciones de constantes */
	#define DES_SIZE     79
	#define CUBE_SIZE     5
	#define KEY_SIZE      6
	#define NO_FIND    0xFF
	#define MAX_SIZE TABLE_SIZE*CUBE_SIZE+DES_SIZE

	typedef struct                               /* Definiciones de tipos. */
		{
		char key[6];
		int id;
		char fill;
		} KEY;

	typedef struct
		{
		KEY regs[CUBE_SIZE];
		char desborde;
		} HASH_TABLE;

#endif
