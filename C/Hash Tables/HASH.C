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
#include <string.h>
#include "bd01.h"
#include "tonyhash.h"

HASH_TABLE Table[TABLE_SIZE];                        /* La tabla de Hash. */
KEY Desparrame[DES_SIZE];                       /* La tabla del desborde. */
int desborde;

void ini_table(),compact();
int ins_hash(KEY),ins_desborde(KEY),findhash(char *,int *),bajahash(char *);
int des_search(char *,int);
unsigned hash_pjw(unsigned char *);

/*=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : ins_hash.
Objetivo   : Inserta un elemento a la tabla de hash.
Par metros : KEY key = Registro a insertar.
Salida     : 1 - operaci¢n exitosa, 0 en caso contrario.
=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+*/
int ins_hash(KEY key)
{
	register i;
	int pos = hash_pjw(key.key) % TABLE_SIZE;
	for (i=0;i<CUBE_SIZE;i++)
		if (Table[pos].regs[i].fill == 0)
			break;
		else if (!strcmp(Table[pos].regs[i].key,key.key) &&
				  (Table[pos].regs[i].id == key.id)          )
			{
			error("Llave repetida.  No puedo dar de alta.  Perdone");
			return 0;
			}
	if (i<CUBE_SIZE)                          /* Elemento cabe en el cubo. */
		{
		Table[pos].regs[i].fill = 1;
		Table[pos].regs[i].id = key.id;
		strcpy(Table[pos].regs[i].key,key.key);
		return 1;
		}
	if (i == CUBE_SIZE)
		Table[pos].desborde = 1;
	if (!ins_desborde(key))
		return 0;
	else
		return 1;
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : ini_table.
Objetivo   : Inicializar la tabla de hash y la de desparrame en ceros.
Par metros : Ninguno.
Salida     : Ninguna.
=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+= */
void ini_table()
{
	register i,j;
	desborde = 0;
	for (i=0;i<TABLE_SIZE;i++)
		{
		for (j=0;j<CUBE_SIZE;j++)
			{
			strcpy(Table[i].regs[j].key,"\0");
			Table[i].regs[j].id = Table[i].regs[j].fill = 0;
			}
		Table[i].desborde = 0;
		}
	for (i=0;i<DES_SIZE;i++)
		{
		strcpy(Desparrame[i].key,"\0");
		Desparrame[i].id = Desparrame[i].fill = 0;
		}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : ins_desborde.
Objetivo   : Insertar un registro en el  rea de desborde.
Par metros : KEY reg  =  Registro a insertar.
Salida     : 0 si no se pudo, 1 si fue posible.
=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+= */
int ins_desborde(KEY reg)
{
	register i;
	if (desborde == DES_SIZE-1)
		{
		error("No cupo el registro en el  rea de desborde.");
		return 0;
		}
	for (i=0;i<=desborde;i++)
		if (!strcmp(Desparrame[i].key,reg.key) &&
			 (Desparrame[i].id == reg.id)          )
			{
			error("Llave repetida.  No puedo dar de alta.  Perdone.");
			return 0;
			}
	strcpy(Desparrame[desborde].key,reg.key);
	Desparrame[desborde].fill = reg.fill;
	Desparrame[desborde].id = reg.id;
	desborde ++;
	return 1;
}

/*=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : findhash.
Objetivo   : Encontrar un registro en la tabla hash.
Par metros : char *llave = Llave del registro a buscar.
				 int *esdes  = Aqu¡ se regresa un 1 si est  en la tabla, un 0 si
									est  en el desborde.
Salida     : Posici¢n del cubo o dentro del  rea de desborde donde se
				 encuentra.
=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+*/
int findhash(char *llave,int *esdes)
{
	register i;
	int pos = hash_pjw(llave) % TABLE_SIZE;
	for (i=0;i<CUBE_SIZE;i++)
		if (!strcmp(Table[pos].regs[i].key,llave))
			{
			*esdes = 1;
			return pos;
			}
	if (Table[pos].desborde)
		if ((i=des_search(llave,0)) != NO_FIND)
			{
			*esdes = 0;
			return i;
			}
	return NO_FIND;
}

/*=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : des_search.
Objetivo   : Realiza una b£squeda en el  rea de desborde.
Par metros : char *llave = Llave a buscar.
				 int primpos = Primera posici¢n donde voy a buscar.
Salida     : Entero con la posici¢n en el  rea de desborde, o el valor de
				 NO_FIND en caso de no hallarlo.
=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+*/
int des_search(char *llave,int primpos)
{
	register i;
	for (i=primpos;i<=desborde;i++)
		{
		if (!strcmp(llave,Desparrame[i].key))
		return i;
		}
	return NO_FIND;
}

/*=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : hash_pjw.
Objetivo   : Calcular el valor de la localidad de la tabla para una llave.
Par metros : unsigned char *name = Apuntador a la llave.
Salida     : Valor de la localidad correspondiente.
=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+*/
unsigned hash_pjw(unsigned char *name)
{
	unsigned val =0;
	unsigned i;
	for(;*name;++name)
		{
		val = (val << 2) + *name;
		if((i=val & ~0x3fff))
			val = (val ^ (i>>2)) & 0x3fff ;
		}
  return val;
}

/*=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : bajahash.
Objetivo   : Dar de baja un registro en la tabla hash y reutiliza el
				 espacio, en caso necesario.
Par metros : char *llave = Llave del registro a dar de baja.
Salida     : 1 - Operaci¢n exitosa, 0 - lo contrario.
=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+*/
int bajahash(char *llave)
{
	register i;
	char op,kk[4];
	int pos = hash_pjw(llave) % TABLE_SIZE;
	for(i=0;i<CUBE_SIZE;i++)
		if(!strcmp(Table[pos].regs[i].key,llave))
			{
			ventana(21,10,59,15,0,FONDO_VENT,MARCO_VENT);
			desp_cad(32,10," Baja de Registro ",C_TITULO);
			desp_cad(25,12,"Identificador:",MARCO_VENT);
			itoa(Table[pos].regs[i].id,kk,10);
			desp_cad(40,12,kk,C_ESPERA);
			desp_cad(25,13,"¨Desea borrar registro (S/N)?",C_TONY);
			do
				op = getch();
			while(op != 's' && op != 'S' && op != 'n' && op != 'N');
			if (op == 's' || op == 'S')
				{
				strcpy(Table[pos].regs[i].key,"\0");
				Table[pos].regs[i].id = 0;
				Table[pos].regs[i].fill = 0;
				compact();
				return 1;
				}
			}
	if (Table[pos].desborde)
		{
		i = des_search(llave,0);
		while (i != NO_FIND)
			{
			ventana(21,10,59,15,0,FONDO_VENT,MARCO_VENT);
			desp_cad(32,10," Baja de Registro ",C_TITULO);
			desp_cad(25,12,"Identificador:",MARCO_VENT);
			itoa(Desparrame[i].id,kk,10);
			desp_cad(40,12,kk,C_ESPERA);
			desp_cad(25,13,"¨Desea borrar registro (S/N)?",C_TONY);
			do
				op = getch();
			while(op != 's' && op != 'S' && op != 'n' && op != 'N');
			if (op == 's' || op == 'S')
				{
				strcpy(Desparrame[i].key,"\0");
				Desparrame[i].id = 0;
				Desparrame[i].fill = 0;
				compact();
				return 1;
				}
			i = des_search(llave,i+1);
			}
		}
	return 0;
}

/*=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : compact.
Objetivo   : Algoritmo que compacta la tabla de hash (usado despu‚s de dar
				 bajas).
Par metros : Ninguno.
Salida     : Ninguna.
=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+*/
void compact()
{
	register i,j,k;
	int tmp;
	for (i=0;i<TABLE_SIZE;i++)
		{
		for(j=0;j<CUBE_SIZE;j++)
			if (!Table[i].regs[j].fill && Table[i].regs[j+1].fill)
				{
				strcpy(Table[i].regs[j].key,Table[i].regs[j+1].key);
				strcpy(Table[i].regs[j+1].key,"\0");
				Table[i].regs[j].id = Table[i].regs[j+1].id;
				Table[i].regs[j].fill = 1;
				Table[i].regs[j+1].id = Table[i].regs[j+1].fill = 0;
				}
		if (Table[i].desborde)
			{
			Table[i].desborde = 0;
			j --;
			for (k=0;k<desborde;k++)
				{
				tmp = hash_pjw(Desparrame[k].key) % TABLE_SIZE;
				if (tmp == i)
					{
					strcpy(Table[i].regs[j].key,Desparrame[k].key);
					strcpy(Desparrame[k].key,"\0");
					Table[i].regs[j].id = Desparrame[k].id;
					Desparrame[k].id = Desparrame[k].fill = 0;
					Table[i].regs[j].fill = 1;
					Table[i].desborde = 1;
					break;
					}
				}
			}
		}
	for (i=0,j=0;i<desborde;i++)
		if (!Desparrame[i].fill && Desparrame[i+1].fill)
			{
			j ++;
			strcpy(Desparrame[i].key,Desparrame[i+1].key);
			strcpy(Desparrame[i+1].key,"\0");
			Desparrame[i].id = Desparrame[i+1].id;
			Desparrame[i+1].id = Desparrame[i+1].fill = 0;
			Desparrame[i].fill = 1;
			}
	desborde -= j;
}
