/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
							INSTITUTO TECNOLOGICO DE MORELIA
						INGENIERIA EN SISTEMAS COMPUTACIONALES
										 Bases de Datos

M¢dulo     : Rutina principal del sistema.
Archivo    : MAINHASH.C
Objetivo   : Programa que muestre el funcionamiento de una tabla de Hash.
Dise¤o     : Jorge E. Aparicio Mayorga (90120511-3), y
				 Jos‚ Antonio Valderrama Ramos (90120509-9).
Implemento : Los mismos.
Versi¢n    : 1.00 (Abril 1994).
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <stdio.h>
#include "video.h"
#include "bd01.h"
#include "tonyhash.h"

char *screen,*opciones[] = { "Altas",
									  "Bajas",
									  "Consultas",
									  "Listado Secuencial",
									  "Mapeo de la Tabla",
									  "Salir" };

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : main.
Objetivo   : Funci¢n que arranca el sistema y controla su ejecuci¢n
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void main()
{
	int opcion = 0;
	MENATR menatr;
	PANT menu0;
	CURSOR cursor;
	menu0.buffer = NULL;
	menatr.atr = MARCO_VENT;
	menatr.somb = 0x39;
	menatr.atropc = FONDO_VENT;
	menatr.atrbar = 0x3B;
	inic_vid();
	PIDE_BUFFER(screen);
	copia_screen(screen);
	borra_screen();
	ini_pant();
	do
		{
		dis_cursor(&cursor);
		opcion = menu(40,0,opcion,6,opciones,&menatr,&menu0);
		app_cursor(&cursor);
		switch(opcion)
			{
			case 0:
				high();
				break;
			case 1:
				down();
				break;
			case 2:
				consult();
				break;
			case 3:
				list_seq();
				break;
			case 4:
				map();
				break;
			}
		}
	while(opcion != -1 && opcion != 5);
	close_menu(&menu0);
	rest_screen(screen);
	FREE_BUFFER(screen);
}
