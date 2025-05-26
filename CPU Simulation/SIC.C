/*--------------------------------------------------------------------------
		       INSTITUTO TECNOLOGICO DE MORELIA
		     ACADEMIA DE SISTEMAS COMPUTACIONALES
			     Lenguaje Ensamblador

M¢dulo    : Compilador de lenguaje ensamblador para nuestra computadora
	    simulada (el programa 80586.C).
Archivo   : SIC.C
Objetivo  : Crear un compilador que nos permita escribir programas para la
	    computadora que simulamos en el programa 80586.C en lenguaje
	    ensamblador, en vez de lenguaje maquinita.  Adem s, agrega el
	    uso de etiquetas.
Dise¤o    : Ivonne Guerrero Pardo
	    H‚ctor Villal¢n P‚rez
	    Jos‚ Antonio Valderrama Ramos
Implemento: Los mismos.
Versi¢n   : Spring 1993 (Abril 1993).
--------------------------------------------------------------------------*/

#include <conio.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <screen.h>
#include <screen.c>
#include "assembly.h"

#define ALTA 30
#define ALTC 46
#define ALTS 31

#define CWL 5
#define CWT 5
#define CWR 75
#define CWB 22

void paso1(),paso2(),main(),pinta_pant(),error1(),abre_arch(),mete_a_m(),
     postorden(),libera(),libera_tablas();
int lee_linea(),reservada(),mete_tabla(),wrap(),expresion();
unsigned int buscaetiq(),evalua();
struct nodo *crea();

struct mesa
   {
   char etiq[18];
   unsigned int dir;
   struct mesa *sig;
   };
struct registro_m
   {
   unsigned int m;
   struct registro_m *sig;
   };
struct nodo
   {
   char flag;
   int dig;
   char op;
   struct nodo *der, *izq;
   };
typedef struct head header;
typedef struct code text;
typedef struct fin end;
typedef struct mesa table;
typedef struct registro_m *M;

FILE *archie;
unsigned int warflag=0,errorflag=0,START_ADDR=0,openflag=0,compflag=0,
	     regist=0,sepa;
char mens[50],arch[13],num[6];
table *TABLAIZQ=NULL,*TABLADER=NULL;

/*--------------------------------------------------------------------------
Funci¢n   : main.
Objetivo  : controlar la ejecuci¢n del programa.
Par metros: ninguno.
Salida    : ninguna.
--------------------------------------------------------------------------*/
void main()
{
   int op,buff[80][25];
   gettext(1,1,80,25,buff);
   pinta_pant();
   do
      {
      op = getch();
      if (op == 0)
	 op = getch();
      switch(op)
	 {
	 case ALTA:
	    abre_arch();
	    break;
	 case ALTC:
	    if(!compflag)
	      {
	      paso1();
	      if(!errorflag)
		 paso2();
	      else
		 {
		 getch();
                 window(CWL,CWT,CWR,CWB);
		 textcolor(YELLOW);
		 textbackground(BLUE);
		 clrscr();
		 }
	      compflag=1;
	      }
	    else
	      {
	      if(openflag)
		 {
		 window(20,10,60,12);
		 textcolor(WHITE);
		 textbackground(MAGENTA);
		 clrscr();
		 gotoxy(6,2);
		 cprintf("El programa ya se compil¢ !!!");
		 getch();
		 }
	      window(CWL,CWT,CWR,CWB);
	      textcolor(YELLOW);
	      textbackground(BLUE);
	      clrscr();
	      }
	    break;
	 }

      }
   while (op!=ALTS);
   if (archie!=NULL)
      fclose(archie);
   puttext(1,1,80,25,buff);
}

/*--------------------------------------------------------------------------
Funci¢n   : abre_arch.
Objetivo  : abre el archivo de texto para ser compilado.
Par metros: ninguno.
Salida    : ninguna.
--------------------------------------------------------------------------*/
void abre_arch()
{
   int buff[80][25];
   char op='S';
   gettext(1,1,80,24,buff);
   window(20,10,60,12);
   textcolor(WHITE);
   textbackground(MAGENTA);
   clrscr();
   gotoxy(4,2);
   if(openflag)
      {
      cprintf("Cerrar %-13s (S/N) ? [ ]",arch);
      do
	 {
	 gotoxy(34,2);
	 op=toupper(getche());
	 }
      while(op!='S'&&op!='N');
      if(op=='S')
	 fclose(archie);
      }
   if(op!='N')
      {
      gotoxy(4,2);
      clreol();
      libera_tablas();
      cprintf("File Name: ");
      strcpy(arch,"            ");
      chtgets(arch,13);
      openflag=1;
      compflag=0;
      if((archie=fopen(arch,"r"))==NULL)
	 {
	 gotoxy(4,2);
	 cprintf(" Error de apertura de archivo !!! ");
	 openflag=0;
	 strcpy(arch,"            ");
	 getch();
	 }
      }
   window(1,1,80,25);
   textcolor(LIGHTGREEN);
   textbackground(BLUE);
   gotoxy(1,25);clreol();
   gotoxy((80-strlen(arch))/2,25);
   cprintf("%s",arch);
   window(CWL,CWT,CWR,CWB);
   textcolor(YELLOW);
   textbackground(BLUE);
   puttext(1,1,80,24,buff);
}

/*--------------------------------------------------------------------------
Funci¢n   : pinta_pant.
Objetivo  : dibuja la interfase para el usuario.
Par metros: ninguno.
Salida    : ninguna.
--------------------------------------------------------------------------*/
void pinta_pant()
{
   int i;
   textbackground(BLUE);
   textcolor(LIGHTGRAY);
   clrscr();
   textcolor(BLACK);
   textbackground(LIGHTGRAY);
   gotoxy(1,1);clreol();
   gotoxy(4,1);
   cprintf("Abrir archivo   Compilar   Salir");
   textcolor(RED);
   gotoxy(4,1);
   cprintf("A");
   gotoxy(20,1);
   cprintf("C");
   gotoxy(31,1);
   cprintf("S");
   textcolor(YELLOW);
   textbackground(BLUE);
   gotoxy(CWL-1,CWT-1);
   cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ Mensajes ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
   for (i=CWT;i<=CWB;i++)
      {
      gotoxy(CWL-1,i);
      cprintf("³");
      gotoxy(CWR+1,i);
      cprintf("³");
      }
   gotoxy(CWL-1,CWB+1);
   cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
}

/*--------------------------------------------------------------------------
Funci¢n   : paso1.
Objetivo  : Construye tablas de etiquetas y revisa la sintaxis del programa.
Par metros: ninguno.
Salida    : ninguna.
--------------------------------------------------------------------------*/
void paso1()
{
   char campo1[19],campo2[6],campo3[20],c=' ';
   table *movida1,*movida2;
   long aux;
   int tipo,l=0;
   unsigned int NL;
   window(CWL,CWT,CWR,CWB);
   gotoxy(1,1);
   cprintf("Compilando...    PASO 1");
   window(CWL,CWT+2,CWR,CWB);
   if (archie==NULL)
      {
      strcpy(mens,"ERROR - Archivo no abierto");
      error1(0);
      errorflag=7;
      compflag=0;
      return;
      }
   lee_linea(campo1,campo2,campo3,l);
   if (strcmp(campo2,"START")==0)
      {
      NL = START_ADDR = atoi(campo3);
      l++;
      }
   else
      {
      START_ADDR = NL = 0;
      rewind(archie);
      }
   while(!feof(archie)&&strcmp(campo2,"END"))
      {
      tipo = lee_linea(campo1,campo2,campo3,l);
      if (tipo==2)
	 continue;
      l++;
      if (campo1[0]!='\0')
	 {
	 if (reservada(campo1,&c))
	    {
	    strcpy(mens,"La etiqueta es palabra reservada");
	    error1(l);
	    continue;
	    }
	 else
	    if (mete_tabla(&TABLAIZQ,campo1,NL))
	       {
	       strcpy(mens,"Etiqueta izquierda repetida");
	       error1(l);
	       }
	 }
      tipo=reservada(campo2,&c);
      if (!tipo)
	 {
	 strcpy(mens,"Instrucci¢n no v lida");
	 error1(l);
	 continue;
	 }
      else
	 if (tipo==1)
	    {
	    NL++;
	    if (campo3[0]!='\0')
	       warning(l,"Instrucci¢n no requiere par metros");
	    }
	 else
	    if (tipo==3)
	       {
	       NL+=3;
	       if (campo3[0]=='\0')
		  {
		  strcpy(mens,"Par metro requerido");
		  error1(l);
		  }
	       else
		  if (isalpha(campo3[0]))
		     mete_tabla(&TABLADER,campo3,NL);
	       }
	    else
	       {
	       if (tipo==4)
		  {
		  if (strcmp(campo2,"RESW")==0)
		     NL+=2*atoi(campo3);
		  if (strcmp(campo2,"RESB")==0)
		     NL+=atoi(campo3);
		  if (strcmp(campo2,"WORD")==0)
		     NL+=2*atoi(campo3);
		  if (strcmp(campo2,"BYTE")==0)
		     {
		     if (campo3[0]=='\'')  /* Comilla */
			{
			for (aux=0;campo3[aux]!='\'';aux++);
			aux--;
			NL+=aux;
			}
		     else
			NL+= atoi(campo3);
		     }
		  }
	       }
      }
   if (strcmp(campo2,"END"))
      {
      strcpy(mens,"Programa sin END");
      error1(l);
      return;
      }
   movida1 = TABLADER;
   movida2 = TABLAIZQ;
   while (movida1!=NULL)
      {
      while (movida2!=NULL)
	 {
	 if (strcmp(movida2->etiq,movida1->etiq)==0)
	    break;
	 movida2 = movida2->sig;
	 if (movida2->sig==NULL)
	    {
	    strcpy(mens,"No coinciden las tablas");
	    error1(l);
	    movida1 = NULL;
	    }
	 }
      if(movida1==NULL)
	 break;
      movida2 = TABLAIZQ;
      movida1 = movida1->sig;
      }
   if(errorflag==7)
      errorflag=0;
}

/*--------------------------------------------------------------------------
Funci¢n   : lee_linea.
Objetivo  : Pone los diferentes campos en cadenas camp1, camp2 y camp3.
Par metros: camp1, camp2 y camp3 -> punteros a las cadenas.
Salida    : 1 si hubo problemas al leer la l¡nea, 0 si no.
--------------------------------------------------------------------------*/
int lee_linea(char *camp1,char *camp2,char *camp3,int l)
{
   char line[81];
   int i=0,j=0;
   camp1[0] = camp2[0] = camp3[0] = '\0';
   fgets(line,80,archie);
   strupr(line);
   if(line[0]=='.'||line[0]==';'||line[0]=='\r'||line[0]=='\n')
      return 2;
   if(line[0]!=' ')
      {
      for(i=0;line[i]!=':'&&i<19;camp1[i]=line[i++])
	 if(line[i]==' ')
	    {
	    strcpy(mens,"Etiqueta inv lida");
	    error1(l);
	    return 2;
	    }
      if(line[i]!=':')
	  {
	  strcpy(mens,"Etiqueta inv lida");
	  error1(l);
	  return 2;
	  }
      camp1[i]='\0';
      }
   for(;line[i]==' '||line[i]==':';i++);
   for(j=0;line[i]!=' '&&line[i]!='\r'&&line[i]!='\n';camp2[j++]=line[i++]);
   camp2[j]='\0';
   for(;line[i]==' '&&line[i]!='\r'&&line[i]!='\n';i++);
   for(j=0;line[i]!='\r'&&line[i]!=' '&&line[i]!='\n';camp3[j++]=line[i++]);
   camp3[j]='\0';
   return 0;
}

/*--------------------------------------------------------------------------
Funci¢n   : mete_tabla.
Objetivo  : crea las tablas de etiquetas.
Par metros: tabla = puntero a la tabla a modificar,
	    str   = etiqueta a introducir.
Salida    : regresa 1 si es repetida, 0 si no.
--------------------------------------------------------------------------*/
int mete_tabla(table **aux,char *str,unsigned int NL)
{
   int flag=0;
   table *tabla;
   tabla=*aux;
   if (tabla!=NULL)
      {
      for(;tabla->sig!=NULL;tabla=tabla->sig)
	 if (!strcmp(tabla->etiq,str))
	    flag=1;
      tabla->sig=(table*)malloc(sizeof (table));
      tabla=tabla->sig;
      }
   else
      {
      tabla=(table*)malloc(sizeof (table));
      *aux=tabla;
      }
   strcpy(tabla->etiq,str);
   tabla->dir=NL;
   tabla->sig=NULL;
   return flag;
}

/*--------------------------------------------------------------------------
Funci¢n   : reservada.
Objetivo  : Checha si la cadena es una palabra reservada.
Par metros: puntero a la cadena a checar
Salida    : n£mero de bytes que ocupa la instrucci¢n y en caso de que no
	    sea instrucci¢n, devuelve un cero.  Si es pseudomadre, regresa
	    un 4.  Regresa en un char, por medio del puntero c, el valor
	    binario de la instrucci¢n, si es posible.
--------------------------------------------------------------------------*/
int reservada(char *palres,char *c)
{
   if(strcmp(palres,"ADD")==0)
      {
      *c=(char)ADD;
      return 3;
      }
   if(strcmp(palres,"SUB")==0)
      {
      *c=(char)SUB;
      return 3;
      }
   if(strcmp(palres,"MUL")==0)
      {
      *c=(char)MUL;
      return 3;
      }
   if(strcmp(palres,"DIV")==0)
      {
      *c=(char)DIV;
      return 3;
      }
   if(strcmp(palres,"ADDX")==0)
      {
      *c=(char)ADDX;
      return 1;
      }
   if(strcmp(palres,"ADDY")==0)
      {
      *c=(char)ADDY;
      return 1;
      }
   if(strcmp(palres,"SUBX")==0)
      {
      *c=(char)SUBX;
      return 1;
      }
   if(strcmp(palres,"SUBY")==0)
      {
      *c=(char)SUBY;
      return 1;
      }
   if(strcmp(palres,"MULX")==0)
      {
      *c=(char)MULX;
      return 1;
      }
   if(strcmp(palres,"MULY")==0)
      {
      *c=(char)MULY;
      return 1;
      }
   if(strcmp(palres,"DIVX")==0)
      {
      *c=(char)DIVX;
      return 1;
      }
   if(strcmp(palres,"DIVY")==0)
      {
      *c=(char)DIVY;
      return 1;
      }
   if(strcmp(palres,"INCA")==0)
      {
      *c=(char)INCA;
      return 1;
      }
   if(strcmp(palres,"INCX")==0)
      {
      *c=(char)INCX;
      return 1;
      }
   if(strcmp(palres,"INCY")==0)
      {
      *c=(char)INCY;
      return 1;
      }
   if(strcmp(palres,"DECA")==0)
      {
      *c=(char)DECA;
      return 1;
      }
   if(strcmp(palres,"DECX")==0)
      {
      *c=(char)DECX;
      return 1;
      }
   if(strcmp(palres,"DECY")==0)
      {
      *c=(char)DECY;
      return 1;
      }
   if(strcmp(palres,"AND")==0)
      {
      *c=(char)AND;
      return 3;
      }
   if(strcmp(palres,"OR")==0)
      {
      *c=(char)OR;
      return 3;
      }
   if(strcmp(palres,"SHL")==0)
      {
      *c=(char)SHL;
      return 3;
      }
   if(strcmp(palres,"SHR")==0)
      {
      *c=(char)SHR;
      return 3;
      }
   if(strcmp(palres,"SHLC")==0)
      {
      *c=(char)SHLC;
      return 3;
      }
   if(strcmp(palres,"SHRC")==0)
      {
      *c=(char)SHRC;
      return 3;
      }
   if(strcmp(palres,"XOR")==0)
      {
      *c=(char)XOR;
      return 3;
      }
   if(strcmp(palres,"CS")==0)
      {
      *c=(char)CS;
      return 1;
      }
   if(strcmp(palres,"CC")==0)
      {
      *c=(char)CC;
      return 1;
      }
   if(strcmp(palres,"ZS")==0)
      {
      *c=(char)ZS;
      return 1;
      }
   if(strcmp(palres,"ZC")==0)
      {
      *c=(char)ZC;
      return 1;
      }
   if(strcmp(palres,"IS")==0)
      {
      *c=(char)IS;
      return 1;
      }
   if(strcmp(palres,"IC")==0)
      {
      *c=(char)IC;
      return 1;
      }
   if(strcmp(palres,"SS")==0)
      {
      *c=(char)SS;
      return 1;
      }
   if(strcmp(palres,"SC")==0)
      {
      *c=(char)SC;
      return 1;
      }
   if(strcmp(palres,"CMPA")==0)
      {
      *c=(char)CMPA;
      return 3;
      }
   if(strcmp(palres,"CMPX")==0)
      {
      *c=(char)CMPX;
      return 3;
      }
   if(strcmp(palres,"CMPY")==0)
      {
      *c=(char)CMPY;
      return 3;
      }
   if(strcmp(palres,"J")==0)
      {
      *c=(char)J;
      return 3;
      }
   if(strcmp(palres,"JZ")==0)
      {
      *c=(char)JZ;
      return 3;
      }
   if(strcmp(palres,"JL")==0)
      {
      *c=(char)JL;
      return 3;
      }
   if(strcmp(palres,"JE")==0)
      {
      *c=(char)JE;
      return 3;
      }
   if(strcmp(palres,"JG")==0)
      {
      *c=(char)JG;
      return 3;
      }
   if(strcmp(palres,"JNL")==0)
      {
      *c=(char)JNL;
      return 3;
      }
   if(strcmp(palres,"JNE")==0)
      {
      *c=(char)JNE;
      return 3;
      }
   if(strcmp(palres,"JNG")==0)
      {
      *c=(char)JNG;
      return 3;
      }
   if(strcmp(palres,"JS")==0)
      {
      *c=(char)JS;
      return 3;
      }
   if(strcmp(palres,"LOOP")==0)
      {
      *c=(char)LOOP;
      return 3;
      }
   if(strcmp(palres,"INT")==0)
      {
      *c=(char)INT;
      return 3;
      }
   if(strcmp(palres,"RET")==0)
      {
      *c=(char)RET;
      return 1;
      }
   if(strcmp(palres,"RTI")==0)
      {
      *c=(char)RTI;
      return 1;
      }
   if(strcmp(palres,"PUSH")==0)
      {
      *c=(char)PUSH;
      return 3;
      }
   if(strcmp(palres,"PUSHA")==0)
      {
      *c=(char)PUSHA;
      return 1;
      }
   if(strcmp(palres,"PUSHX")==0)
      {
      *c=(char)PUSHX;
      return 1;
      }
   if(strcmp(palres,"PUSHY")==0)
      {
      *c=(char)PUSHY;
      return 1;
      }
   if(strcmp(palres,"POP")==0)
      {
      *c=(char)POP;
      return 3;
      }
   if(strcmp(palres,"POPA")==0)
      {
      *c=(char)POPA;
      return 1;
      }
   if(strcmp(palres,"POPX")==0)
      {
      *c=(char)POPX;
      return 1;
      }
   if(strcmp(palres,"POPY")==0)
      {
      *c=(char)POPY;
      return 1;
      }
   if(strcmp(palres,"LDA")==0)
      {
      *c=(char)LDA;
      return 3;
      }
   if(strcmp(palres,"LDX")==0)
      {
      *c=(char)LDX;
      return 3;
      }
   if(strcmp(palres,"LDY")==0)
      {
      *c=(char)LDY;
      return 3;
      }
   if(strcmp(palres,"LDPC")==0)
      {
      *c=(char)LDPC;
      return 3;
      }
   if(strcmp(palres,"LDBP")==0)
      {
      *c=(char)LDBP;
      return 3;
      }
   if(strcmp(palres,"LDSW")==0)
      {
      *c=(char)LDSW;
      return 3;
      }
   if(strcmp(palres,"STA")==0)
      {
      *c=(char)STA;
      return 3;
      }
   if(strcmp(palres,"STX")==0)
      {
      *c=(char)STX;
      return 3;
      }
   if(strcmp(palres,"STY")==0)
      {
      *c=(char)STY;
      return 3;
      }
   if(strcmp(palres,"STPC")==0)
      {
      *c=(char)STPC;
      return 3;
      }
   if(strcmp(palres,"STBP")==0)
      {
      *c=(char)STBP;
      return 3;
      }
   if(strcmp(palres,"STSW")==0)
      {
      *c=(char)STSW;
      return 3;
      }
   if(strcmp(palres,"LDAL")==0)
      {
      *c=(char)LDAL;
      return 3;
      }
   if(strcmp(palres,"STAL")==0)
      {
      *c=(char)STAL;
      return 3;
      }
   if(strcmp(palres,"LDXL")==0)
      {
      *c=(char)LDXL;
      return 3;
      }
   if(strcmp(palres,"STXL")==0)
      {
      *c=(char)STXL;
      return 3;
      }
   if(strcmp(palres,"LDYL")==0)
      {
      *c=(char)LDYL;
      return 3;
      }
   if(strcmp(palres,"STYL")==0)
      {
      *c=(char)STYL;
      return 3;
      }
   if(strcmp(palres,"BYTE")==0)
      return 4;
   if(strcmp(palres,"WORD")==0)
      return 4;
   if(strcmp(palres,"RESB")==0)
      return 4;
   if(strcmp(palres,"RESW")==0)
      return 4;
   if(strcmp(palres,"EQU")==0)
      return 4;
   if(strcmp(palres,"END")==0)
      return 5;
   return 0;
}

/*--------------------------------------------------------------------------
Funci¢n   : error1.
Objetivo  : Escribe los errores en la pantalla.
Par metros: l - n£mero de l¡nea (en el c¢digo fuente) de la l¡nea con error.
Salida    : ninguno.
--------------------------------------------------------------------------*/
void error1(int m)
{
   errorflag++;
   cprintf("ERROR en l¡nea %3d, %s\r\n",m,mens);
}

/*--------------------------------------------------------------------------
Funci¢n   : warning.
Objetivo  : Escribe los warnings en la pantalla.
Par metros: l    - n£mero de l¡nea (en el c¢digo fuente) de la l¡nea
		   con advertencia.
Salida    : ninguno.
--------------------------------------------------------------------------*/
warning(int l)
{
   warflag++;
   cprintf("WARNING en l¡nea %3d, %s\r\n",l,mens);
}

/*--------------------------------------------------------------------------
Funci¢n   : paso2.
Objetivo  : Escribe el archivo ejecutable (.SIC).
Par metros: ninguno.
Salida    : ninguno.
--------------------------------------------------------------------------*/
void paso2()
{
   FILE *archie2;
   char arch2[13],campo1[19],campo2[5],campo3[20],sepalachi[20];
   unsigned char c=' ';
   unsigned int l,i,NL=0,j,bin,n;
   int aux,tlong=500,bincod;
   header cabeza;
   text texto;
   end fifi;
   M mod=NULL;/*,mandm; -> Esto es para versi¢n con c¢digo relocalizable. */
   i=wherex();j=wherey();
   window(CWL,CWT,CWR,CWB);
   gotoxy(1,1);cprintf("Compilando...    PASO 2");
   window(CWL,CWT+2,CWR,CWB);
   gotoxy(i,j);
   rewind(archie);
   for(l=0;arch[l]!='.';arch2[l]=arch[l],l++);
   arch2[l]='\0';
   rewind(archie);
   i=lee_linea(campo1,campo2,campo3,l++);
   if (!strcmp(campo2,"START"))
      {
      NL=START_ADDR=atoi(campo3);
      i=lee_linea(campo1,campo2,campo3,l++);
      }
   cabeza.H='H';
   cabeza.size_text=0;
   cabeza.size_sic=0;
   cabeza.reg_t=0;
   cabeza.dir_in=NL=START_ADDR;
   strupr(arch2);
   strcpy(cabeza.nombre_arch,strcat(arch2,".SIC"));
   n=l;
   l=0;
   if ( (archie2=fopen(arch2,"wb")) == NULL)
      {
      strcpy(mens,"Disk error");
      error1(l);
      return;
      }
   fwrite(&cabeza,sizeof(struct head),1,archie2);
   while(strcmp(campo2,"END")!=0)
      {
      if (i==2)
	 {
	 l++;
	 i=lee_linea(campo1,campo2,campo3,l);
	 continue;
	 }
      aux=reservada(campo2,&c);
      switch (aux)
	 {
	 case 4:                                          /* pseudomadre */
	    if (!isdigit(campo3[0])&&campo3[0]!='\'')
	       strcpy(sepalachi,&campo3[1]);
	    else
	       strcpy(sepalachi,campo3);
	    if (isdigit(sepalachi[strlen(sepalachi)-1]))
	       j=atoi(sepalachi);
	    if (sepalachi[strlen(sepalachi)-1]=='O')
	       {
	       sepalachi[strlen(sepalachi)-1]='\0';
	       sscanf(sepalachi,"%o",&j);
	       }
	    if (sepalachi[strlen(sepalachi)-1]=='H')
	       {
	       sepalachi[strlen(sepalachi)]='\0';
	       sscanf(sepalachi,"%x",&j);
	       }
	    if (!strcmp(campo2,"BYTE"))
	       if (sepalachi[0]=='\'')
		  {
		  tlong=wrap(tlong,j,archie2,NL);
		  j=strlen(sepalachi)-2;
		  for (bin=1;bin<j;fprintf(archie2,"%02X",sepalachi[bin++]));
		  }
	       else
		  {
		  tlong=wrap(tlong,1,archie2,NL);
		  fprintf(archie2,"%02X",(char)j);
		  j=1;
		  }
	    if (!strcmp(campo2,"WORD"))
	       {
	       tlong=wrap(tlong,2,archie2,NL);
	       fprintf(archie2,"%04X",j);
	       j=2;
	       }
	    if (!strcmp(campo2,"RESB"))
	       {
	       tlong=wrap(tlong,j,archie2,NL);
	       for (bin=1,c=' ';bin<=j;fprintf(archie2,"%02X",c));
	       }
	    if (!strcmp(campo2,"RESW"))
	       {
	       j*=2;
	       tlong=wrap(tlong,j,archie2,NL);
               for (bin=1,c=' ';bin<=j;fprintf(archie2,"%02X",c));
	       }
	    NL+=j;
	    break;
	 case 3:                                        /* Es de tama¤o 3 */
	    tlong=wrap(tlong,3,archie2,NL+3);
	    reservada(campo2,&c);
	    if (campo3[0]=='#')
	       c |= 1;
	    fprintf(archie2,"%02x",c);
	    NL++;
	    if (campo3[0]=='#'||campo3[0]=='@')
	       aux=buscaetiq(&campo3[1]);
	    else
	       aux=buscaetiq(campo3);
	    if (aux>=0)
	       itoa(aux,campo3,10);
	    bincod=evalua(campo3,mod);
	    fprintf(archie2,"%04x",bincod);
	    j=3;
	    NL+=2;
	    break;
	 case 1:                                       /* Es de tama¤o 1 */
	    tlong=wrap(tlong,1,archie2,NL);
	    reservada(campo2,&c);
	    if (campo3[0]=='#')
	       c|=1;
	    fprintf(archie2,"%02x",c);
	    NL++;
	    j=1;
	    break;
	 }
      i=lee_linea(campo1,campo2,campo3,l);
      }
   tlong=wrap(tlong,1,archie2,NL);
   NL++;
   c=END;
   fprintf(archie2,"%02x",c);
   fseek(archie2,sepa,SEEK_SET);
   texto.T='T';
   texto.dir=NL;
   texto.longi=tlong;
   fwrite(&texto,sizeof(text),1,archie2);
   regist++;
   fseek(archie2,0,SEEK_END);
   fifi.E='E';
   fifi.firstinst=START_ADDR;
   fwrite(&fifi,sizeof(struct fin),1,archie2);
/* &&&&&&&&&&&&&&&&&&&& NOTA PARA PROXIMA VERSION: &&&&&&&&&&&&&&&&&&&&&&&&&
      Aqu¡ se deber  escribir el registro M, al final del archivo .SIC.
Use el puntero mod, el cual apunta al principio de la lista simplemente
ligada M, la cual contiene todas las direcciones de los registros que se
deber n alterar (v‚ase la estructura del struct registro_m) y auxiliate
con el puntero mandm, declarada para ayudar a cumplir esto.  (No olvides
de liberar memoria).
   &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
   cabeza.size_sic=NL;
   cabeza.reg_t=regist;
   fseek(archie2,0,SEEK_END);
   cabeza.size_text=ftell(archie2);
   fseek(archie2,0,SEEK_SET);
   fwrite(&cabeza,sizeof(struct head),1,archie2);
   fclose(archie2);
   libera_tablas();
   cprintf("\r\n              %u L¡neas de texto y %d l¡neas compiladas...\r\n",n,NL);
   window(CWL,CWT,CWR,CWB);
   textcolor(LIGHTGREEN);
   cprintf("           COMPILACION EXITOSA  :  ");
   textcolor(LIGHTGREEN+BLINK);
   cprintf("Presione cualquier tecla !!!");
   getch();
   clrscr();
   window(CWL,CWT+2,CWR,CWB);
}

/*--------------------------------------------------------------------------
Funci¢n   : wrap.
Objetivo  : Verifica si una instrucci¢n cabe en el registro de texto.
Par metros: tlong   = n£mero de caracteres escritos en el registro,
	    j       = tama¤o en bytes del que quiero escribir, y
	    archie2 = puntero al archivo de salida.
Salida    : nuevo valor de tlong.
--------------------------------------------------------------------------*/
int wrap(int tlong,int j,FILE *archie2,unsigned int NL)
{
   text texto;
   int kk=tlong+j;
   if ((tlong+j)>80)
      {
      texto.T='T';
      if (tlong!=500)
	 {
	 fseek(archie2,-tlong-sizeof(struct code),SEEK_CUR);
	 texto.dir=NL;
	 texto.longi=tlong;
	 fwrite(&texto,sizeof(struct code),1,archie2);
	 regist++;
	 }
      fseek(archie2,0,SEEK_END);
      sepa=ftell(archie2);
      texto.longi=0;
      texto.dir=0;
      fwrite(&texto,sizeof(struct code),1,archie2);
      kk=j;
      }
   return kk;
}

/*--------------------------------------------------------------------------
Funci¢n   : mete_a_m.
Objetivo  : Mete una etiqueta a la lista de direcciones a incluir en el
	    registro M.
Par metros: campo3 = etiqueta a a¤adir, y
	    princ  = puntero al principio de la lista.
Salida    : ninguna.
--------------------------------------------------------------------------*/
void mete_a_m(char *campo3,M princ)
{
   M node,nodep;
   nodep=princ;
   node=malloc(sizeof(struct registro_m));
   node->m=atoi(campo3);
   node->sig=NULL;
   if (princ==NULL)
      princ=node;
   else
      {
      while(nodep->sig!=NULL)
	 nodep=nodep->sig;
      nodep->sig=node;
      }
}

/*--------------------------------------------------------------------------
Funci¢n   : buscaetiq.
Objetivo  : Busca una etiqueta en la tabla izquierda.
Par metros: campo3 = etiqueta a buscar.
Salida    : -1 si no lo encuentra y la direcci¢n que le corresponde si lo
	    halla.
--------------------------------------------------------------------------*/
unsigned int buscaetiq(char *campo3)
{
   table *recorrido=TABLAIZQ;
   while (recorrido!=NULL)
      {
      if (!strcmp(campo3,recorrido->etiq))
	 return recorrido->dir;
      recorrido=recorrido->sig;
      }
   return -1;
}

/*--------------------------------------------------------------------------
Funci¢n   : libera_tablas.
Objetivo  : libera las tablas de etiquetas.
Par metros: ninguno.
Salida    : ninguna.
--------------------------------------------------------------------------*/
void libera_tablas()
{
   table *auxilio;
   while (TABLAIZQ!=NULL)
      {
      auxilio=TABLAIZQ->sig;
      free(TABLAIZQ);
      TABLAIZQ=auxilio;
      }
   while (TABLADER!=NULL)
      {
      auxilio=TABLADER->sig;
      free(TABLADER);
      TABLADER=auxilio;
      }
}

/*--------------------------------------------------------------------------
Funci¢n   : evalua.
Objetivo  : Regresa el valor de la cadena que le pasas, tal y para escibirlo
	    en el archivo .SIC.
Par metros: string = cadena con la expresi¢n a evaluar,
	    princ  = puntero al principio de la lista de M&M's.
Salida    : el valor que se deber  escribir en el archivo .SIC (en un int).
--------------------------------------------------------------------------*/
unsigned int evalua(char *string,M princ)
{
   unsigned int sepa;
   switch(string[0])
      {
      case '#':
	 return atoi(&string[1]);
      case '@':
	 sepa=atoi(&string[1]);
	 sepa|=65536;
	 mete_a_m(&string[1],princ);
	 return sepa;
      case '+':
	 for (sepa=0;sepa<strlen(string)-1;string[sepa]=string[++sepa]);
	 sepa=expresion(string);
	 return sepa;
      default:
	 mete_a_m(string,princ);
	 return atoi(string);
      }
}

/*--------------------------------------------------------------------------
Funci¢n   : crea.
Objetivo  : Crea un arbol binario para poder evaluar una expresi¢n.
Par metros: x = cadena con la expresi¢n a evaluar.
Salida    : direcci¢n de la ra¡z del  rbol.
--------------------------------------------------------------------------*/
struct nodo *crea (char *x)
{
   struct nodo *y,*z,*aux=NULL;
   int i=0,j;
   while (x[i]!=')'&&x[i]!='\0')
      {
      if (x[i]=='(')
	 {
	 i++;
	 if (aux==NULL)
	    aux=crea(x);
	 else
	    aux->der=crea(x);
	 }
      else
	 {
	 y=(struct nodo *) malloc(sizeof(struct nodo));
	 y->dig=0;y->op=' ';y->flag=' ';
	 if(isdigit(x[i]))
	    {
	    for(j=i;isdigit(x[j]);num[j-i]=x[j],j++);
	    num[j-i]='\0';
	    y->dig=atoi(num);
	    y->flag='D';
	    i=j-1;
	    }
	 else
	    {
	    y->op=x[i];
	    y->flag='O';
	    }
	 if (isdigit(x[i]))
	    {
	    y->der=NULL;
	    y->izq=NULL;
	    if (aux==NULL)
	       aux=y;
	    else
	       aux->der=y;
	    }
	 else
	    {
	    y->izq=aux;
	    aux=y;
	    }
      }
   i++;
   }
   return(aux);
}

/*--------------------------------------------------------------------------
Funci¢n   : postorden.
Objetivo  : recorre el  rbol binario donde se encuentra la expresi¢n
	    para calcular el resultado.
Par metros: root = ra¡z del  rbol.
Salida    : ninguna.
--------------------------------------------------------------------------*/
void postorden(struct nodo *root)
{
   if (root!=NULL)
      {
      postorden(root->izq);
      postorden(root->der);
      if(root->flag=='O')
	 {
	 switch(root->op)
	    {
	    case '+':
	       root->dig = root->izq->dig + root->der->dig;
	       break;
	    case '-':
	       root->dig= root->izq->dig - root->der->dig;
	       break;
	    case '*':
	       root->dig= root->izq->dig * root->der->dig;
	       break;
	    case '/':
	       root->dig= root->izq->dig / root->der->dig;
	       break;
	    }
         root->flag='D';
	 }
      }
}

/*--------------------------------------------------------------------------
Funci¢n   : libera.
Objetivo  : libera la memoria utilizada por el  rbol.
Par metros: root = ra¡z del  rbol.
Salida    : ninguna.
--------------------------------------------------------------------------*/
void libera(struct nodo *root)
{
   if (root!=NULL)
      {
      postorden(root->izq);
      postorden(root->der);
      free(root);
      }
}

/*--------------------------------------------------------------------------
Funci¢n   : expresion.
Objetivo  : resuelve expresiones.
Par metros: x = direcci¢n de la cadena donde se encuentra la expresi¢n.
Salida    : resultado de la expresi¢n.
--------------------------------------------------------------------------*/
int expresion(char *x)
{
   int r;
   struct nodo *arbol;
   arbol=crea(x);
   postorden(arbol);
   r=arbol->dig;
   libera(arbol);
   return r;
}
