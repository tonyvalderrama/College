/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
						 INGENIERIA EN SISTEMAS COMPUTACIONALES
										Sistemas Operativos
M¢dulo     : M¢dulo principal del Simulador de un Sistema Operativo.
Archivo    : ms-tres.c
Objetivo   : Funciones principales del Sistema.
Dise¤o     : Jorge Eduardo Aparicio Mayorga  -  90120511-3,
				 Jos‚ Antonio Valderrama Ramos  -  90120509-9.
Implemento : Los mismos.
Versi¢n    : Noviembre 1993.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include "graf-ms3.h"
int op = 1;

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : main
Objetivo   : Rutina principal del programa, la que controla todo.
Par metros : Ninguna.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void main()
{	start_system();
	while(op)
	{  op = check_command();
		_clean_mouse();
		ejecuta_proc();
		_clean_mouse();
	}
	shut_down();
}
