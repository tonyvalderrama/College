/*                       Programa de las ocho reinas                     */
/*    Encuentra todas las maneras que se pueden posicionar ocho reinas   */
/*  en un tablero de ajedrez, sin que ninguna raina se "coma" entre s¡.  */


/*  Jos‚ Antonio Valderrama Ramos     -          No. Control 90120509-9  */


/*  Inicializaci¢n de vectores para controlar posiciones posibles de las */
/*                  reinas.  El 1 significa posici¢n libre.              */

int fila[9]={0,1,1,1,1,1,1,1,1};                /* Posiciones en la fila */
int izq[15]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};      /* Posiciones diag izq */
int der[17]={0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};  /* Posiciones diag der */
int col[9];                     /* Numero de columna donde est  colocada */
int i,lra=0;

main()
{
   void prueba(),impsol();
   clrscr();
   printf("\n\n\t\t***PROBLEMA DE LAS OCHO REINAS***");
   printf("\n\tLas posibles soluciones son como sigue:\n\n");
   prueba(1);
}

/* prueba() */
/* Funci¢n recursiva que va probando todos los lugares hasta colocar */
/* cada uno de las ocho reinas */

void prueba(i)
int i;
{
   int j;
   for (j=1;j<=8;j++)
      {
      if ( (fila[j]==1) && (der[i+j]==1) && (izq[7+i-j]==1) )
	 {
	 col[i]=j;
	 fila[j]=0;
	 der[i+j]=0;
	 izq[7+i-j]=0;
	 if (i<8)
	    prueba(i+1);
	 else
	    impsol();
	 fila[j]=1;
	 der[i+j]=1;
	 izq[7+i-j]=1;
	 }
      }
}

/* impsol() */
/* Funci¢n que imprime las posiciones de cada uno de las ocho reinas en */
/* el tablero */

void impsol()
{
   int chp=0;
   lra++;
   for (chp=1;chp<=8;chp++)
      printf("Soluci¢n #%d es (%d,%d)\n",lra,col[chp],chp);
   printf("\nOprime <ENTER> para continuar");
   printf("\n\n");
   getch();
}
