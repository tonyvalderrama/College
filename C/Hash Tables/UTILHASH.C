/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
							INSTITUTO TECNOLOGICO DE MORELIA
						INGENIERIA EN SISTEMAS COMPUTACIONALES
										 Bases de Datos

M¢dulo     : Rutinas de mantenimiento de la tabla.
Archivo    : UTILHASH.C
Objetivo   : Rutinas que manejen la tabla de Hash.
Dise¤o     : Jorge E. Aparicio Mayorga (90120511-3), y
				 Jos‚ Antonio Valderrama Ramos (90120509-9).
Implemento : Los mismos.
Versi¢n    : 1.00 (Abril 1994).
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "tonyhash.h"
#include "video.h"
#include "bd01.h"

void high(),consult(),down(),error(char *),list_seq(),insert_aux(KEY,int);
void ini_pant(),map(),cons(char *);

extern HASH_TABLE Table[TABLE_SIZE];
extern KEY Desparrame[DES_SIZE];
extern int desborde;
KEY aux[MAX_SIZE];

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : high.
Objetivo   : Rutina que da de alta a una llave en la tabla Hash.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void high()
{
	KEY tmp;
	PANT vent1;
	char aux[4];
	copia_area(21,10,59,15,&vent1);
	ventana(21,10,59,15,0,FONDO_VENT,MARCO_VENT);
	desp_cad(32,10," Alta de Registro ",C_TITULO);
	desp_cad(25,12,"Llave:",FONDO_VENT);
	desp_cad(32,12,"     ",C_ESPERA);
	desp_cad(23,14,"Identificador:",FONDO_VENT);
	desp_cad(38,14,"   ",C_ESPERA);
	pide_cad(32,12,tmp.key,KEY_SIZE-1);
	pide_cad(38,14,aux,3);
	tmp.id = atoi(aux);
	tmp.fill = 1;
	ins_hash(tmp);
	rest_area(&vent1);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : down.
Objetivo   : Rutina que da de baja a una llave en la tabla Hash.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void down()
{
	char tmp[6];
	int id;
	PANT vent1;
	copia_area(21,10,59,15,&vent1);
	ventana(21,10,59,15,0,FONDO_VENT,MARCO_VENT);
	desp_cad(32,10," Baja de Registro ",C_TITULO);
	desp_cad(23,12,"Llave:",FONDO_VENT);
	desp_cad(30,12,"     ",C_ESPERA);
	pide_cad(30,12,tmp,KEY_SIZE-1);
	if(!bajahash(tmp))
		error("No se pudo efectuar la baja, disculpe.");
	else
		error("Registro dado de baja existosamente.");
	rest_area(&vent1);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : consult.
Objetivo   : Rutina que hace una consulta directa a la tabla Hash.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void consult()
{
	char llave_cons[KEY_SIZE];
	int rec;
	PANT vent1;
	copia_area(21,10,59,15,&vent1);
	ventana(21,10,59,15,0,FONDO_VENT,MARCO_VENT);
	desp_cad(29,10," Consulta de Registros ",C_TITULO);
	desp_cad(25,12,"Llave:",FONDO_VENT);
	desp_cad(32,12,"     ",C_ESPERA);
	pide_cad(32,12,llave_cons,KEY_SIZE-1);
	cons(llave_cons);
   rest_area(&vent1);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : cons.
Objetivo   : Rutina repetitiva que hace consultas a la tabla Hash.
Par metros : KEY llave_cons = Registro a buscar.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void cons(char *llave_cons)
{
	char tmp[4];
	int l,kk = 0,reg = findhash(llave_cons,&l);
	if (reg == NO_FIND)
		{
		error("Llave no encontrada.");
		return;
		}
	while(!strcmp(Table[reg].regs[kk].key,llave_cons) && l &&
			kk < CUBE_SIZE )
		{
		ventana(21,10,59,15,0,FONDO_VENT,MARCO_VENT);
		desp_cad(29,10," Consulta de Registros ",C_TITULO);
		desp_cad(25,11,"Llave encontrada",C_TONY);
		desp_cad(25,12,"En el cubo",FONDO_VENT);
		itoa(reg,tmp,10);
		desp_cad(36,12,tmp,FONDO_VENT);
		desp_cad(25,13,"Con desplazamiento",FONDO_VENT);
		itoa(kk,tmp,10);
		desp_cad(44,13,tmp,FONDO_VENT);
		desp_cad(25,14,"Su identificador exclusivo es:",FONDO_VENT);
		itoa(Table[reg].regs[kk].id,tmp,10);
		desp_cad(56,14,tmp,FONDO_VENT);
		getch();
		kk ++;
		}
	if (Table[reg].desborde)
		for (kk=0;kk<desborde;kk++)
			if (!strcmp(Desparrame[kk].key,llave_cons))
				{
            ventana(21,10,59,15,0,FONDO_VENT,MARCO_VENT);
				desp_cad(29,10," Consulta de Registros ",C_TITULO);
				desp_cad(25,11,"Llave encontrada",C_TONY);
				desp_cad(25,12,"En el  rea de desborde",FONDO_VENT);
				desp_cad(25,13,"En la posici¢n",FONDO_VENT);
				itoa(kk,tmp,10);
				desp_cad(44,13,tmp,FONDO_VENT);
				desp_cad(25,14,"Su identificador exclusivo es:",FONDO_VENT);
				itoa(Desparrame[kk].id,tmp,10);
				desp_cad(56,14,tmp,FONDO_VENT);
				getch();
				}
	else if (!l)
		for (kk=reg;kk<DES_SIZE;kk++)
			if (!strcmp(Desparrame[kk].key,llave_cons))
				{
            ventana(21,10,59,15,0,FONDO_VENT,MARCO_VENT);
				desp_cad(29,10," Consulta de Registros ",C_TITULO);
				desp_cad(25,11,"Llave encontrada",C_TONY);
				desp_cad(25,12,"En el  rea de desborde",FONDO_VENT);
				desp_cad(25,13,"En la posici¢n",FONDO_VENT);
				itoa(kk,tmp,10);
				desp_cad(44,13,tmp,FONDO_VENT);
				desp_cad(25,14,"Su identificador exclusivo es:",FONDO_VENT);
				itoa(Desparrame[kk].id,tmp,10);
				desp_cad(56,14,tmp,FONDO_VENT);
				getch();
				}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : list_seq.
Objetivo   : Rutina que imprime un listado secuencial de todos los registros
				 dados de alta en la tabla de Hash.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void list_seq()
{
	register i,j;
	char tmp[4];
	PANT vent1;
	copia_area(0,10,79,24,&vent1);
	ventana(0,10,79,24,0,FONDO_VENT,MARCO_VENT);
	insert_aux(aux[0],1);
	for (i=0;i<MAX_SIZE;i++)
		{
		strcpy(aux[i].key,"\0");
		aux[i].fill = aux[i].id = 0;
		}
	for(i=0;i<TABLE_SIZE;i++)
		for(j=0;j<CUBE_SIZE;j++)
			if (Table[i].regs[j].fill)
				insert_aux(Table[i].regs[j],0);
	for (i=0;i<DES_SIZE;i++)
		if(Desparrame[i].fill)
			insert_aux(Desparrame[i],0);
	i = 0;
	j = 12;
	desp_cad(26,10," Listado Secuencial Ordenado. ",C_TITULO);
	desp_cad(33,12,"Llave",C_TONY);
	desp_cad(43,12,"Id.",C_TONY);
	while(aux[i].fill)
		{
		if (j == 23)
			{
			desp_cad(50,24,"Presione cualquier tecla ...",C_TONY);
			getch();
			ventana(0,10,79,24,0,FONDO_VENT,MARCO_VENT);
			desp_cad(26,10," Listado Secuencial Ordenado. ",C_TITULO);
			desp_cad(33,12,"Llave",C_TONY);
			desp_cad(43,12,"Id.",C_TONY);
			j = 12;
			}
		j++;
		desp_cad(33,j,aux[i].key,FONDO_VENT);
		itoa(aux[i].id,tmp,10);
		desp_cad(43,j,tmp,FONDO_VENT);
		i++;
		}
	getch();
	rest_area(&vent1);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : map.
Objetivo   : Rutina que mapea la tabla de Hash.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void map()
{
	register i;
	char ch,llave[KEY_SIZE];
	int pos,row,col,k;
	PANT vent1;
	copia_area(21,10,59,15,&vent1);
	ventana(21,10,59,15,0,FONDO_VENT,MARCO_VENT);
	desp_cad(27,10," Mapeo de la Tabla Hash ",C_TITULO);
	desp_cad(23,12,"¨Desea consultar alguna llave (S/N)?",FONDO_VENT);
	desp_cad(39,13," ",C_ESPERA);
	do
		{
		gotoxy(40,14);
		ch = getche();
		}
	while(ch != 'S' && ch != 'N' && ch != 's' && ch != 'n');
	if (ch == 'S' || ch == 's')
		{
		desp_cad(25,14,"Llave:",FONDO_VENT);
		desp_cad(32,14,"     ",C_ESPERA);
		pide_cad(32,14,llave,5);
		pos = findhash(llave,&k);
		if (pos == NO_FIND)
			error("Llave inexistente.  Mejor suerte para la otra.");
		}
	else
		pos = NO_FIND;
	rest_area(&vent1);
	copia_area(0,0,42,24,&vent1);
	ventana(0,0,42,24,0,FONDO_VENT,MARCO_VENT);
	desp_cad(10,0," Mapeo de la Tabla Hash ",C_TITULO);
	for (i=0,row=2,col=1;i<TABLE_SIZE;i++,col++)
		{
		if (pos != NO_FIND && k && pos==i)
			desp_cad(col,row,"\x1",C_TONY_2);
		else if (!Table[i].regs[0].fill)
			desp_cad(col,row,"\xB2",FONDO_VENT);
		else if (Table[i].regs[0].fill)
			desp_cad(col,row,"\xB2",C_TONY);
		if (col == 40)
			{
			row ++;
			col = 0;
			}
		}
	for(i=0;i<DES_SIZE;i++,col++)
		{
		if (!strcmp(Desparrame[i].key,llave))
			desp_cad(col,row,"\x1",C_TONY_2);
		else if(!Desparrame[i].fill)
			desp_cad(col,row,"\xDB",FONDO_VENT);
		else if (Desparrame[i].fill)
			desp_cad(col,row,"\xDB",C_TONY);
		if (col == 40)
			{
			row ++;
			col = 0;
			}
		}
	desp_cad(18,15,"Leyenda",C_TONY);
	desp_cad(2,16,"\x1",C_TONY_2);
	desp_cad(5,16,"= Cubo de llave buscada.",FONDO_VENT);
	desp_cad(2,17,"\xB2",FONDO_VENT);
	desp_cad(5,17,"= Cubo de la Tabla Hash desocupada.",FONDO_VENT);
	desp_cad(2,18,"\xB2",C_TONY);
	desp_cad(5,18,"= Cubo de la Tabla Hash ocupada.",FONDO_VENT);
	desp_cad(2,19,"\xDB",FONDO_VENT);
	desp_cad(5,19,"= Cubo del  rea de desborde",FONDO_VENT);
	desp_cad(5,20,"  desocupado.",FONDO_VENT);
	desp_cad(2,21,"\xDB",C_TONY);
	desp_cad(5,21,"= Cubo del  rea de desborde ocupado.",FONDO_VENT);
	getch();
	rest_area(&vent1);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : insert_aux.
Objetivo   : Funci¢n que inserta un registro en un arreglo auxiliar (para el
				 listado secuencial).
Par metros : KEY reg   = Registro a insertar.
				 int first = Si tiene 1, inicializo mi contador de posiciones
								 del arreglo auxiliar (lo inicializa).
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void insert_aux(KEY reg,int first)
{
	static int auxi = 0;
	int j;
	register i;
	if (first)
		{
		auxi = 0;
		return;
		}
	j = auxi;
	while( (strcmp(aux[j-1].key,reg.key) > 0) && (j) )
		{
		strcpy(aux[j].key,aux[j-1].key);
		aux[j].id = aux[j-1].id;
		aux[j].fill = 1;
		if (!j)
			break;
		j--;
		}
	strcpy(aux[j].key,reg.key);
	aux[j].id = reg.id;
	aux[j].fill = 1;
	auxi ++;
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : error
Objetivo   : Desplegar un mensaje de error
Par metros : char *s = Mensaje de error.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void error(char *s)
{
	PANT video1;
	register i, x1, l_mensaje;
	l_mensaje = strlen(s)+4;
	x1 = ((80-l_mensaje)/2);
	copia_area(x1, 16, x1+l_mensaje, 18, &video1);
	ventana(x1, 16, x1+l_mensaje, 18, 0, FONDO_VENT+1, MARCO_VENT);
	desp_cad(x1+2, 17, s, FONDO_VENT+1);
	getch();
	rest_area(&video1);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : ini_pant.
Objetivo   : Rutina que inicializa la pantalla del sistema.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void ini_pant()
{
	register i,j;
	for (i=0,j=1;i<25;i++,j++)
		{
		desp_cad(i,i,"Implementaci¢n de una tabla Hash.       Implementaci¢n de una tabla Hash.",j);
		if (j == 0xF)
			j = 0;
		}
}
