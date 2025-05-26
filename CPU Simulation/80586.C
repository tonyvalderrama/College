/*-------------------------------------------------------------------------
		      INSTITUTO TECNOLOGICO DE MORELIA
		   INGENIERIA EN SISTEMAS COMPUTACIONALES
	       Lenguaje Ensamblador (Programaci¢n de Sistemas)

M¢dulo       : simulador de un procesador.
Archivo      : 80586.C
Objetivo     : programa que emula todo el funcionamiento de un procesador de
	       una computadora.
Dise¤o       : Ivonne Guerrero Pardo           -   87121105-9
	       Jos‚ Antonio Valderrama Ramos   -   90120509-9
	       H‚ctor Villal¢n P‚rez           -   90120515-7
Implemento   : los mismos.
Versi¢n      : Febrero 1993 (versi¢n Winter '93).
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <SCREEN.H>
#include <SCREEN.C>
#include "assembly.h"           /* Contine definiciones de instrucciones. */

#define MEM_MAX 0xFFFF/2              /* Constante con tama¤o max de RAM. */
#define VGA 0xA0000000          /* Constantes para tipo de tarjeta de ... */
#define CGA 0xB8000000                                      /* ... video. */
#define HERC 0xB0000000

#define vramstart 31887
#define stackend 31806

#define CF(F) (rgs->SW)&=(0xFFFF^F);                                /* Banderas. */
#define SF(F) (rgs->SW)|=F;
#define edoband(F) ((rgs->SW)&F)
#define Z 1
#define S 2
#define C 4
#define O 8
#define U 16
#define I 32

unsigned char M[MEM_MAX];                                          /* RAM */
int segmento=0;
void carga(),si_mula(),error1(),push(),pintacrt(),mask(),pintapant();
void teclazo();
long pop();
char boot();

struct reg
{
   long A;
   unsigned int X,Y,PC,BP,SP,SW;
};
typedef struct reg RGS;

/*--------------------------------------------------------------------------
Funci¢n    : main
Objetivo   : controla la ejecuci¢n principal del programa.
Par metros : ninguno.
Salida     : ninguna.
--------------------------------------------------------------------------*/
void main()
{
   int i,x,y;
   RGS *rgs,reg;
   char PAP;
   rgs=&reg;
   PAP=boot(rgs);
   carga(rgs);
   i=0;
   for(x=5;x<78;x+=8)
      for(y=15;y<25;y++)
	 {
	 gotoxy(x,y);
	 cprintf("%2X",*(M+i));
	 i++;
	 }
   pintapant(rgs);
   while (M[rgs->PC]!=END)
      {
      si_mula(rgs);
      pintapant(rgs);
      if (PAP=='S')
	 teclazo(&PAP);            /* Aqu¡ veo que puch¢, PgUp PgDn u otro */
      }
}

/*--------------------------------------------------------------------------
Funci¢n    : boot
Objetivo   : arranca el procesador.
Par metros : RGS *rgs - puntero a los registros.
Salida     : un char que indica si se corre paso por paso.
--------------------------------------------------------------------------*/
char boot(RGS *rgs)
{
   unsigned int i,stacksize;
   char PAP;
   textcolor(WHITE);
   textbackground(BLACK);
   clrscr();
   cprintf("Correr PASO a PASO (S/N) ? [ ]");
   do
     {
     gotoxy(29,1);
     PAP=toupper(getche());
     }
   while (PAP!='S'&&PAP!='N');
   cprintf("\r\nTama¤o de la pila ? ");
   scanf("%u",&stacksize);
   rgs->PC = 0;                      /* Inicializar registros y banderas. */
   rgs->SW = 0;
   rgs->SP = rgs->BP = stackend-stacksize;
   rgs->X = rgs->Y = rgs->A = 0;
   for (i=vramstart;i<=MEM_MAX;i++)       /* Inicializo VRAM (todo en 0). */
      M[i] = 0;
   pintacrt();
   mask();
   pintapant(rgs);
   return (char)PAP;
}

/*--------------------------------------------------------------------------
Funci¢n    : carga
Objetivo   : carga instrucciones a la memoria.
Par metros : RGS *rgs - puntero a los registros.
Salida     : ninguna.
--------------------------------------------------------------------------*/
void carga(RGS *rgs)
{
   struct code text;
   struct head header;
   struct fin end;
   struct modify           /* Estructura de los registros de modificaci¢n */
      {
      char M;
      unsigned int tam_reg, reg_mod,col;
      }modifica;
   FILE *archie2;
   char vari,name[13];
   int i,j,PC;
   hola:
   window(42,5,77,7);
   textcolor(WHITE);
   textbackground(LIGHTMAGENTA);
   clrscr();
   gotoxy(4,2);
   cprintf("Nombre del archivo: ");chtgets(name,13);
   if( (archie2=fopen(name,"rb"))==NULL)
      {
      gotoxy(4,2);clreol();
      cprintf("Error al abrir archivo %s !!!",name);
      getch();
      gotoxy(4,2);clreol();
      cprintf("Desea terminar (S/N) ? [ ]");
      do {gotoxy(28,2);vari=toupper(getche());}
      while(vari!='S'&&vari!='N');
      if(vari=='S') exit(0);
      goto hola;
      }
   textcolor(WHITE);
   textbackground(BLACK);
   clrscr();
   window(1,1,80,25);
   fread(&header,sizeof(struct head),1,archie2);   /* Leo cabecera */
   rgs->PC=PC=header.dir_in;         /* Direccion inicial del programa */
   for(i=0;i<header.reg_t;i++)
      {
      fread(&text,sizeof(struct code),1,archie2);  /* Leo cabecera T */
      for(j=0;j<text.longi;j++,PC++)
	{
	fscanf(archie2,"%02x",&vari);
	M[PC]=vari;
	}
      }
 /* Lee el registro de modificacion para el c¢digo relocalizable */
 /*  fread(&modifica,sizeof(modifica),1,archie2);
   while(!feof(archie2))
   {
     fread(&modifica,sizeof(modifica),1,archie2);
     modifica->reg_mod+=PC;
   }  */
}



/*--------------------------------------------------------------------------
Funci¢n    : simula
Objetivo   : ejecuta instrucciones en la memoria.
Par metros : RGS *rgs  -  puntero a registros,
	     char p    -  para ver si lo corro paso a paso.
Salida     : ninguna.
--------------------------------------------------------------------------*/
void si_mula(RGS *rgs)
{
   unsigned int *opcito,op;
   unsigned long aux;
   op = M[rgs->PC+2];                     /* ... instrucci¢n y lo pongo en op. */
   if (M[rgs->PC] & 1)                                 /* Si es inmediato (#). */
      op |= ((int) M[rgs->PC+1]) << 8;
   else                                           /* Si es por direcci¢n. */
      {
      op |= ((int) M[rgs->PC+1]&0x7F) << 8;
      if (M[rgs->PC+1] & 0x80)                          /* Si es relativo a X. */
	 {
	 aux=op;
	 op = M[aux+rgs->X+1];
	 op |= ((int) M[aux+rgs->X]) << 8;
	 }
      else                                             /* Si es absoluto. */
	 {
	 aux=op;
	 op = M[aux+1];
	 op |= ((int) M[aux]) << 8;
	 }
      }
   switch(M[rgs->PC]&0xFE)                   /* Identificaci¢n de instrucci¢n. */
      {
      case ADD:
	 CF(C);
	 CF(S);
	 CF(Z);
	 aux = rgs->A+op;
	 if (aux>=0)
	    {
	    if (aux > 65535)
	       SF(C);
	    if (!aux)
	       SF(Z);
	    }
	 rgs->A += op;
	 if (rgs->A&128)
	    SF(S);
	 rgs->PC += 3;
	 break;
      case SUB:
	 CF(C);
	 CF(S);
	 CF(Z);
	 aux = rgs->A-op;
	 if (aux > 65535)
	    SF(C);
	 if (rgs->A==op)
	    SF(Z)
	 rgs->A -= op;
	 if (rgs->A&128)
	    SF(S);
	 rgs->PC += 3;
	 break;
      case MUL:
	 CF(O);
	 CF(S);
	 CF(Z);
	 aux = rgs->A*op;
	 if (aux > 65535)
	    SF(O);
	 if (rgs->A==0 || op==0)
	    SF(Z);
	 if ((rgs->A<0&&op>0) || (rgs->A>0&&op<0))
	    SF(S);
	 rgs->A *= op;
	 rgs->PC += 3;
	 break;
      case DIV:
	 CF(U);
	 CF(S);
	 CF(Z);
	 if (op==0)
	    {
	    SF(U);
	    rgs->PC += 3;
	    break;
	    }
         aux = rgs->A/op;
	 if ((rgs->A<0&&op>0) || (rgs->A>0&&op<0))
	    SF(S);
	 if (rgs->A==0)
	    SF(Z);
	 rgs->A /= op;
	 rgs->PC += 3;
	 break;
      case ADDX:
	 CF(C);
	 CF(S);
	 CF(Z);
	 aux = rgs->A+rgs->X;
	 if (aux>=0)
	    {
	    if (aux > 65535)
	       SF(C)
	    else
	       if (!aux)
		  SF(Z);
	    }
	 else
	    SF(S);
	 rgs->A += rgs->X;
	 rgs->PC++;
	 break;
      case ADDY:
	 CF(C);
	 CF(S);
	 CF(Z);
	 aux = rgs->A+rgs->Y;
	 if (aux>=0)            /* Altero bandera de signo (+ 1, - 0). */
	    {
	    if (aux > 65535)               /* Altero bandera de CARRY. */
	       SF(C)
	    else
	       if (!aux)           /* Altero banda si es igual a zero. */
		  SF(Z);
	    }
	 else
	    SF(S);
	 rgs->A += rgs->Y;
	 rgs->PC++;
	 break;
      case SUBX:
	 CF(C);
	 CF(S);
	 CF(Z);
	 aux = rgs->A-rgs->X;
	 if (aux > 65535)
	    SF(C);
	 if (rgs->A==rgs->X)
	    SF(Z)
	 else
	    if (rgs->X>rgs->A)
	       SF(S);
	 rgs->A -= rgs->X;
	 rgs->PC++;
	 break;
      case SUBY:
	 CF(C);
	 CF(S);
	 CF(Z);
	 aux = rgs->A-rgs->Y;
	 if (aux > 65535)
	    SF(C);
	 if (rgs->A==rgs->Y)
	    SF(Z)
	 else
	    if (rgs->Y>rgs->A)
	       SF(S);
	 rgs->A -= rgs->Y;
	 rgs->PC++;
	 break;
      case MULX:
	 CF(O);
	 CF(S);
	 CF(Z);
	 aux = rgs->A*rgs->X;
	 if (aux > 65535)
	    SF(O);
	 if (rgs->A==0 || rgs->X==0)
	    SF(Z);
	 if ((rgs->A<0&&rgs->X>0) || (rgs->A>0&&rgs->X<0))
	    SF(S);
	 rgs->A *= rgs->X;
	 rgs->PC++;
	 break;
      case MULY:
	 CF(O);
	 CF(S);
	 CF(Z);
	 aux = rgs->A*rgs->Y;
	 if (aux > 65535)
	    SF(O);
	 if (rgs->A==0 || rgs->Y==0)
	    SF(Z);
	 if ((rgs->A<0&&rgs->Y>0) || (rgs->A>0&&rgs->Y<0))
	    SF(S);
	 rgs->Y *= rgs->A;
	 rgs->PC++;
	 break;
      case DIVX:
	 CF(U);
	 CF(S);
	 CF(Z);
	 aux = rgs->A/rgs->X;
	 if (rgs->X==0)
	    SF(U);
	 if ((rgs->A<0&&rgs->X>0) || (rgs->A>0&&rgs->X<0))
	    SF(S);
	 if (rgs->A==0)
	    SF(Z);
	 rgs->A /= rgs->X;
	 rgs->PC++;
	 break;
      case DIVY:
	 CF(U);
	 CF(S);
	 CF(Z);
	 aux = rgs->A/rgs->Y;
	 if (rgs->Y==0)
	    SF(U);
	 if ((rgs->A<0&&rgs->Y>0) || (rgs->A>0&&rgs->Y<0))
	    SF(S);
	 if (rgs->A==0)
	    SF(Z);
	 rgs->A /= rgs->Y;
	 rgs->PC++;
	 break;
      case INCX:
	 CF(C);
	 aux = rgs->X+1;
	 if (aux>65535)
	    SF(C);
	 rgs->X++;
	 rgs->PC++;
	 break;
      case INCY:
	 CF(C);
	 aux = rgs->Y+1;
	 if (aux>65535)
	    SF(C);
	 rgs->Y++;
	 rgs->PC++;
	 break;
      case DECX:
	 CF(S);
	 CF(Z);
	 if (rgs->X==1)
	    SF(Z)
	 else
	    if (rgs->X<=0)
	       SF(S);
	 rgs->X--;
	 rgs->PC++;
	 break;
      case DECY:
	 CF(S);
	 CF(Z);
	 if (rgs->Y==1)
	    SF(Z)
	 else
	    if (rgs->Y<=0)
	       SF(S);
	 rgs->Y--;
	 rgs->PC++;
	 break;
      case INCA:
	 CF(C);
	 aux = rgs->A+1;
	 if (aux>65535)
	    SF(C);
	 rgs->A++;
	 rgs->PC++;
	 break;
      case DECA:
	 CF(S);
	 CF(Z);
	 if (rgs->A==1)
	    SF(Z)
	 else
	    if (rgs->A<=0)
	       SF(S);
	 rgs->A--;
	 rgs->PC++;
	 break;
      case AND:
	 CF(Z);
	 rgs->A&=op;
	 if (!rgs->A)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case ANDX:
	 CF(Z);
	 rgs->X&=op;
	 if (!rgs->X)
	    SF(Z);
	 rgs->PC+=3;
	 break;
      case ANDY:
	 CF(Z);
	 rgs->Y&=op;
	 if (!rgs->Y)
	    SF(Z);
	 rgs->PC+=3;
	 break;
      case OR:
	 CF(Z);
	 rgs->A|=op;
	 if (!rgs->A)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case ORX:
	 CF(Z);
	 rgs->X|=op;
	 if (!rgs->X)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case ORY:
	 CF(Z);
	 rgs->Y|=op;
	 if (!rgs->Y)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case SHL:
	 CF(Z);
	 rgs->A<<=op;
	 if (!rgs->A)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case SHR:
	 CF(Z);
	 rgs->A>>=op;
	 if (!rgs->A)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case SHLC:
	 CF(Z);
	 CF(C);
	 aux=32768&rgs->A;
	 rgs->A<<=op;
	 if (aux)
	    {
	    SF(C);
	    rgs->A+=1;
	    }
	 if (!rgs->A)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case SHRC:
	 CF(Z);
	 CF(C);
	 if (op%2)
	    aux=32768;
	 else
	    aux=0;
	 rgs->A>>=op;
	 rgs->A+=aux;
	 if (aux)
	    SF(C);
	 if (!rgs->A)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case XOR:
	 CF(Z);
	 rgs->A^=op;
	 if (!rgs->A)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case XORX:
	 CF(Z);
	 rgs->X^=op;
	 if (!rgs->X)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case XORY:
	 CF(Z);
	 rgs->Y^=op;
	 if (!rgs->Y)
	    SF(Z);
	 rgs->PC += 3;
	 break;
      case CS:
	 SF(C);
	 rgs->PC ++;
	 break;
      case CC:
	 CF(C);
	 rgs->PC ++;
	 break;
      case ZS:
	 SF(Z);
	 rgs->PC ++;
	 break;
      case ZC:
	 CF(Z);
	 rgs->PC ++;
	 break;
      case IS:
	 SF(I);
	 rgs->PC ++;
	 break;
      case IC:
	 CF(I);
	 rgs->PC ++;
	 break;
      case SS:
	 SF(S);
	 rgs->PC ++;
	 break;
      case SC:
	 CF(S);
	 rgs->PC ++;
	 break;
      case CMPA:
	 CF(Z);
	 CF(S);
	 if (rgs->A==op)
	    SF(Z)
	 else
	    if (rgs->A < op)
	       SF(S);
	 rgs->PC += 3;
	 break;
      case CMPX:
	 CF(Z);
	 CF(S);
	 if (rgs->X==op)
	    SF(Z)
         else
	    if (rgs->X < op)
	       SF(S);
	 rgs->PC += 3;
	 break;
      case CMPY:
	 CF(Z);
	 CF(S);
	 if (rgs->Y==op)
	    SF(Z)
         else
	    if (rgs->Y < op)
	       SF(S);
	 rgs->PC += 3;
	 break;
      case J:
	 rgs->PC = op;
	 break;
      case JZ:
      case JE:
	 if (edoband(Z))
	    rgs->PC = op;
	 else
	    rgs->PC += 3;
	 CF(Z);
	 CF(S);
	 break;
      case JL:
	 if (edoband(S)&&(!edoband(Z)))
	    rgs->PC = op;
	 else
	    rgs->PC += 3;
	 CF(Z);
	 CF(S);
	 break;
      case JG:
	 if ((!edoband(S))&&(!edoband(Z)))
	    rgs->PC = op;
	 else
	    rgs->PC += 3;
	 CF(Z);
	 CF(S);
	 break;
      case JNL:
	 if (((!edoband(S))&&(!edoband(Z)))||((!edoband(S))&&edoband(Z)))
	    rgs->PC = op;
	 else
	    rgs->PC += 3;
	 CF(Z);
	 CF(S);
	 break;
      case JNE:
	 if ((((!edoband(S))&&(!edoband(Z)))||((edoband(S))&&(!edoband(Z)))))
	    rgs->PC = op;
	 else
	    rgs->PC += 3;
	 CF(Z);
	 CF(S);
	 break;
      case JNG:
	 if (((!edoband(S))&&edoband(Z))||(edoband(S)&&(!edoband(Z))))
	    rgs->PC = op;
	 else
	    rgs->PC += 3;
	 CF(Z);
	 CF(S);
	 break;
      case JS:
	 push(rgs,(long)rgs->PC);
	 rgs->PC = op;
	 break;
      case LOOP:
	 CF(S);
	 CF(Z);
	 rgs->Y--;
	 if (rgs->Y==0)
	    SF(Z);
	 if (edoband(Z))
	    rgs->PC += 3;
	 else
	    rgs->PC = op;
	 break;
      case INT:
	 break;
      case RET:
	 rgs->PC = (int) pop(rgs);
	 break;
      case RTI:
	 break;
      case PUSH:
	 push(rgs,(long)op);
	 rgs->PC += 3;
	 break;
      case PUSHA:
	 push(rgs,rgs->A);
	 rgs->PC++;
	 break;
      case PUSHX:
	 push(rgs,(long)rgs->X);
	 rgs->PC++;
	 break;
      case PUSHY:
	 push(rgs,(long)rgs->Y);
	 rgs->PC++;
	 break;
      case POP:
	M[op] = (int) pop(rgs);
	 rgs->PC += 3;
	 break;
      case POPA:
	 rgs->A = pop(rgs);
	 rgs->PC++;
	 break;
      case POPX:
	 rgs->X = (int) pop(rgs);
	 rgs->PC++;
	 break;
      case POPY:
	 rgs->Y = (int) pop(rgs);
	 rgs->PC++;
	 break;
      case LDA:
         CF(C);
	 CF(S);
	 CF(Z);
	 if (op>=0)
	    {
	    if (op > 65535)
	       SF(C)
	    else
	       if (!op)
		  SF(Z);
	    }
	 else
	    SF(S);
	 rgs->A = op;
	 rgs->PC += 3;
	 break;
      case LDX:
         CF(C);
	 CF(S);
	 CF(Z);
	 if (op>=0)
	    {
	    if (op > 65535)
	       SF(C)
	    else
	       if (!op)
		  SF(Z);
	    }
	 else
	    SF(S);
	 rgs->X = op;
	 rgs->PC += 3;
	 break;
      case LDY:
         CF(C);
	 CF(S);
	 CF(Z);
	 if (op>=0)
	    {
	    if (op > 65535)
	       SF(C)
	    else
	       if (!op)
		  SF(Z);
	    }
	 else
	    SF(S);
	 rgs->Y = op;
	 rgs->PC += 3;
	 break;
      case LDPC:
         CF(C);
	 CF(S);
	 CF(Z);
	 if (op>=0)
	    {
	    if (op > 65535)
	       SF(C)
	    else
	       if (!op)
		  SF(Z);
	    }
	 else
	    SF(S);
	 rgs->PC = op;
	 break;
      case LDBP:
         CF(C);
	 CF(S);
	 CF(Z);
	 if (op>=0)
	    {
	    if (op > 65535)
	       SF(C)
	    else
	       if (!op)
		  SF(Z);
	    }
	 else
	    SF(S);
	 rgs->BP = op;
	 rgs->PC += 3;
	 break;
      case LDSP:
         CF(C);
	 CF(S);
	 CF(Z);
	 if (op>=0)
	    {
	    if (op > 65535)
	       SF(C)
	    else
	       if (!op)
		  SF(Z);
	    }
	 else
	    SF(S);
	 rgs->SP = op;
	 rgs->PC += 3;
	 break;
      case STA:
	 opcito = M+op;
	 *opcito = rgs->A;
	 rgs->PC += 3;
	 break;
      case STX:
	 opcito = M+op;
	 *opcito = rgs->X;
	 rgs->PC += 3;
	 break;
      case STY:
	 opcito = M+op;
	 *opcito = rgs->Y;
	 rgs->PC += 3;
	 break;
      case STPC:
	 opcito = M+op;
	 *opcito = rgs->PC;
	 rgs->PC += 3;
	 break;
      case STBP:
	 opcito = M+op;
	 *opcito = rgs->BP;
	 rgs->PC += 3;
	 break;
      case STSP:
	 opcito = M+op;
	 *opcito = rgs->SP;
	 rgs->PC += 3;
	 break;
      case STSW:
	 opcito = M+op;
	 *opcito = rgs->SW;
	 rgs->PC += 3;
	 break;
      case LDAL:break;
      case STAL:break;
      case LDXL:break;
      case STXL:break;
      case LDYL:break;
      case STYL:break;
      }
   pintacrt();
}

/*--------------------------------------------------------------------------
Funci¢n    : mask
Objetivo   : imprime como va a quedar la pantalla para la simulaci¢n.
Par metros : ninguno.
Salida     : ninguna.
--------------------------------------------------------------------------*/
void mask()
{
   int x,y,i;
   textcolor(LIGHTMAGENTA);
   clrscr();
   for(y=1;y<12;y++)
      {
      gotoxy(39,y);
      cprintf("³");
      }
   gotoxy(1,12);
   for(y=1;y<81;y++)
      cprintf("Ä");
   gotoxy(39,12);
   cprintf("Á");
   gotoxy(2,2);   cprintf("Registros:      Banderas:");
   gotoxy(2,4);   cprintf("        A              Z");
   gotoxy(2,5);   cprintf("        X              S");
   gotoxy(2,6);   cprintf("        Y              C");
   gotoxy(2,7);   cprintf("       BP              O");
   gotoxy(2,8);   cprintf("       SP              U");
   gotoxy(2,9);   cprintf("       PC              I");
   gotoxy(2,10);  cprintf("       SW");
   i=0;
   for(x=2;x<75;x+=8)
      for(y=15;y<25;y++)
	 {
	 gotoxy(x,y);
	 cprintf("%2d",i++);
	 }
   gotoxy(2,13);cprintf("MEMORIA (hexadecimal)");
   gotoxy(65,13);cprintf("Segmento %3d",segmento);
   gotoxy(1,25);cprintf("  F2 Correr   F3 Fin   F4 Principio    ESC Salir del programa        PgDn/PgUp");
   textcolor(WHITE);
}

/*--------------------------------------------------------------------------
Funci¢n    : push
Objetivo   : "apusha" un dato al stack.
Par metros : RGS *rgs - puntero a los registros,
	     long x - dato a apushar
Salida     : ninguna.
--------------------------------------------------------------------------*/
void push(RGS *rgs,long x)
{
   if (rgs->SP == stackend)
      error1("Fatal error: Stack overflow.");
   *(M+rgs->SP)=x;
   rgs->SP += 4;
}

/*--------------------------------------------------------------------------
Funci¢n    : pop
Objetivo   : "popea" un dato del stack.
Par metros : RGS *rgs - puntero a los registros.
Salida     : ninguna.
--------------------------------------------------------------------------*/
long pop(RGS *rgs)
{
   if (rgs->SP == rgs->BP)
      error1 ("Fatal error: Stack underflow.");
   rgs->SP -= 4;
   return *(M+rgs->SP);
}

/*--------------------------------------------------------------------------
Funci¢n    : pintacrt
Objetivo   : pinta pantalla de PCsita.
Par metros : ninguno.
Salida     : ninguna.
--------------------------------------------------------------------------*/
void pintacrt()
{
   unsigned int i,j;
   int far *ptr;
   ptr = (int far*)CGA;
   for (i=vramstart,j=39;i<=MEM_MAX;i+=2,j++)
      {
      if (!(j%80))
	 j+=40;
      (char)*(ptr+j) = (char)*(M+i);
      }
}

/*--------------------------------------------------------------------------
Funci¢n    : pintapant
Objetivo   : pinta pantalla de PC (las banderas y los registros).
Par metros : RGS *rgs - puntero a los registros.
Salida     : ninguna.
--------------------------------------------------------------------------*/
void pintapant(RGS *rgs)
{
   textcolor(WHITE);
   textbackground(BLACK);
   gotoxy(14,4); cprintf("%7ld",rgs->A);
   gotoxy(14,5); cprintf("%7u",rgs->X);
   gotoxy(14,6); cprintf("%7u",rgs->Y);
   gotoxy(14,7); cprintf("%7u",rgs->BP);
   gotoxy(14,8); cprintf("%7u",rgs->SP);
   gotoxy(14,9); cprintf("%7u",rgs->PC);
   gotoxy(14,10);cprintf("%7u",rgs->SW);
   gotoxy(28,4); cprintf("%7u",edoband(Z));
   gotoxy(28,5); cprintf("%7u",edoband(S)?1:0);
   gotoxy(28,6); cprintf("%7u",edoband(C)?1:0);
   gotoxy(28,7); cprintf("%7u",edoband(O)?1:0);
   gotoxy(28,8); cprintf("%7u",edoband(U)?1:0);
   gotoxy(28,9); cprintf("%7u",edoband(I)?1:0);
}

/*--------------------------------------------------------------------------
Funci¢n    : teclazo
Objetivo   : espera un teclazo; si es PgUp o PgDn, cambia pantalla de la
	     memoria.
Par metros : char *PAP - puntero a un char que se va a modificar.
Salida     : el valor modificado de PAP.
--------------------------------------------------------------------------*/
void teclazo(char *PAP)
{
   int aux,x,y,i;
   do
     {
      aux=getch();
      if (aux==0)
	 aux=getch();
      switch(aux)
	 {
	 case 27:
	    exit(0);
	 case 81:
	    if(segmento<328)
	       segmento++;
	    break;
	 case 73:
	    if(segmento)
	       segmento--;
	    break;
	 case 60:
	    *PAP='N';
	    return;
	 case 61:
	    segmento=328;
	    break;
	 case 62:
	    segmento=0;
	    break;
	 default:
	    return;
	 }
      i=0;
      gotoxy(65,13);cprintf("Segmento %3d",segmento);
      for(x=5;x<78;x+=8)
	 for(y=15;y<25;y++)
	    {
	    gotoxy(x,y);
	    cprintf("%2X",*(M+(segmento*100+i)));
	    i++;
	    }
      }
   while(1);
}

/*--------------------------------------------------------------------------
Funci¢n    : error1
Objetivo   : manda mensaje de error fatal y sale del programa.
Par metros : char *str - cadena a imprimir.
Salida     : ninguna.
--------------------------------------------------------------------------*/
void error1(str)
char *str;
{
   int len = strlen(str);
   window(20,10,60,14);
   textbackground(RED);
   textcolor(YELLOW);
   clrscr();
   gotoxy((40-len)/2,3);
   cprintf("%s",str);
   getch();
   gotoxy(1,1);
   textbackground(BLACK);
   textcolor(WHITE);
   window(1,1,80,25);
   clrscr();
   exit(1);
}
