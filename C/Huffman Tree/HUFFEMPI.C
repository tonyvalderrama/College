#include <stdlib.h>
#include <stdio.h>
#include "htree.h"
#include "htree.c"

static void compressmod(struct htree *h,struct htree *child);
int con;
FILE *result;

void main()
{
   FILE *fptr;
   BYTECOUNTER bytectr=0;
   char aux1[10],aux2[10];
   double aux;
   int freqctr=0,c,i;
   fptr = fopen("texto.dat","rb");
   result = fopen("result.dat","w");
   fseek(fptr,0,SEEK_SET);
   c = fgetc(fptr);
   while (!feof(fptr))
      {
      c &= 255;
      if (ht[c].cnt==0)
	 {
	 freqctr++;
	 ht[c].ch = c;
	 }
      ht[c].cnt++;
      bytectr++;
      c = fgetc(fptr);
      }
   fclose(fptr);
   fptr = fopen("chares.pba","wb");
   fprintf(result,"\nN£mero de bytes = %d",bytectr);
   fprintf(result,"\nN£mero de caracteres diferentes = %d\n",freqctr);
   fprintf(result,"xi\t\tai\t\tfrec.\t\tprob.\t\tn\n\n");
   for (i=0;i<256;i++)
      if (ht[i].cnt != 0)
	 fputc(ht[i].ch,fptr);
   buildtree();
   fseek(fptr,0,SEEK_SET);
   c = fgetc(fptr);
   while (!feof(fptr))
      {
      con = 0;
      fprintf(result,"%c\t\t",c);
      i = c&255;
      compressmod(ht+(c&255),NULL);
      fprintf(result,"\t\t%d",ht[i].cnt);
      itoa(ht[i].cnt,aux1,10);
      itoa(bytectr,aux2,10);
      aux = atof(aux1);
      aux /= atof(aux2);
      fprintf(result,"\t\t%f\t%d\n",aux,con);
      c = fgetc(fptr);
      }
   fclose(fptr);
   fclose(result);
}

static void compressmod(struct htree *h,struct htree *child)
{
   if (h->parent != NULL)
      compressmod(h->parent,h);
   if (child)
      {
      con++;
      if (child == h->right)
	 fprintf(result,"0");
      else if (child == h->left)
	 fprintf(result,"1");
      }
}