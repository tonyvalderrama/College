/*------------------huffd.c--------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "htree.h"
#include "htree.c"

static int decompress(FILE *fi, struct htree *root);

void main(int argc,char *argv[])
{
   FILE *fi, *fo;
   unsigned char c;
   BYTECOUNTER bytectr;
   int freqctr;

   if (argc < 3)   {
      printf("\nusage: huffd infile outfile");
      exit(1);
   }

   if ((fi = fopen(argv[1], "rb")) == NULL)   {
      printf("\nCannot open %s", argv[1]);
      exit(1);
   }

   if ((fo = fopen(argv[2], "wb")) == NULL)   {
      printf("\nCannot open %s", argv[2]);
      fclose(fi);
      exit(1);
   }

   /* ----- read the byte count -----*/
   fread(&bytectr, sizeof(bytectr), 1, fi);

   /* ----- read the frequency count _____*/
   fread(&freqctr, sizeof(freqctr), 1, fi);

   while (freqctr--)    {
      fread(&c, sizeof(char), 1, fi);
      ht[c].ch = c;
      fread(&ht[c].cnt, sizeof(BYTECOUNTER), 1, fi);
   }

   /* ----- build the huffman tree ----- */
   buildtree();

   /* ----- decompress the file ----- */
   while (bytectr--)
      fputc(decompress(fi, root), fo);
   fclose(fo);
   fclose(fi);
}

static int in8;
static int ct8 = 8;

/* ----- read a bit at a time from a file ----- */
static int inbit(FILE *fi)
{
   int obit;
   if (ct8 == 8)   {
      in8 = fgetc(fi);
      ct8 = 0;
   }
   obit = in8 & 0x80;
   in8 <<= 1;
   ct8++;
   return obit;
}

/* ----- decompress file bits into characters ----- */
static int decompress(FILE *fi, struct htree *h)
{
   while (h->right != NULL)
      if (inbit(fi))
	 h = h->left;
      else
	 h = h->right;
   return h->ch;
}
