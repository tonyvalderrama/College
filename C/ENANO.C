/*                                  enano.c                               */
/*    Programa que implementa un algoritmo para solucionar el problema    */
/*  del enano de las escaleras del examen de la 1a. unidad de Algoritmos  */
/*                              Computacionales.                          */

/*               Jos‚ Antonio Valderrama Ramos  -  90120509-9             */

#include <stdio.h>
#include <string.h>

#define N 10

int numper[N];
char xxx[89][N+1],x1[89][N+1],x2[89][N+1];    /* El 1er. par metro se ... */
			  /* saca por [1,2,3,5,8,13,21,34,55,89,144,...]. */

void main()
{
   int i,j;
   void enano();
   clrscr();
   numper[0] = 1;
   numper[1] = 2;
   for (i=2;i<N;i++)    /* numper tiene los pasos nec. para cada escal¢n. */
      numper[i] = numper[i-1]+numper[i-2];
   printf("\n\tEsta es la matriz resultante para subir %d escalones ...",N);
   printf("\n\n");
   enano();
   j = N-1;
   for (i=0;i<numper[j];i++)
      {
      printf("No. %d -> %s",i+1,&x2[i][0]);
      printf("\n");
      }
   printf("\n\n");
}

void enano()      /* Funci¢n que construye la matriz de pasos necesarios. */
{
   int k,i,j=0;
   for (k=1;k<=N;k++)
      {
      switch (k)
	 {
	 case 1:
	    strcpy(&x1[0][0],"1\0");
	    break;
	 case 2:
	    strcpy(&x2[0][0],"11\0");
	    strcpy(&x2[1][0],"2\0");
	    break;
	 default:
	    i = 0;
	    j = 0;
	    do
	       {
	       strcpy(&xxx[j][0],&x2[i][0]);
	       strcat(&xxx[j][0],"1\0");
	       i++;
	       j++;
	       }
	    while (i < numper[k-2]);
	    i = 0;
	    do
	       {
	       strcpy(&xxx[j][0],&x1[i][0]);
	       strcat(&xxx[j][0],"2\0");
	       j++;
	       i++;
	       }
	    while (i < numper[k-3]);
	    for (i=0;i<numper[N-1];i++)
	       {
	       strcpy(&x1[i][0],&x2[i][0]);
	       strcpy(&x2[i][0],&xxx[i][0]);
	       strcpy(&xxx[i][0],"\0");
	       }
	 }
      }
}