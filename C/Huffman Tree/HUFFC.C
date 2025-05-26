/* --------------- huffc.c ------------ */
#include <stdio.h>
#include <stdlib.h>
#include "htree.h"
#include "htree.c"

static void compress (FILE *fo, struct htree *h, struct htree *child );
static void outbit (FILE *fo, int bit);
void main(int argc, char *argv[])
{
 FILE *fi, *fo;
 int c;
 BYTECOUNTER bytectr = 0;
 int freqctr = 0;

 if ( argc < 3 ){
    printf("\nusage: huffc infile outfile");
    exit(1);
 }
 if (( fi = fopen( argv[1], "rb"))==NULL){
    printf("\ncannot open %s",argv[1]);
    exit(1);
 }
 if ((fo = fopen( argv[2], "wb"))==NULL){
    printf("\ncannot open %s",argv[2]);
    fclose(fi);
    exit(1);
 }
 /* ----- read input file and count character frequency ------- */
 while (( c=getc(fi)) != EOF ){
    c &= 255;
    if ( ht[c].cnt == 0 ){
       freqctr++;
       ht[c].ch = c;
    }
    ht[c].cnt++;
    bytectr++;
 }
 /* ------------ write the bytew count to the output file --------- */
 fwrite( &bytectr, sizeof bytectr, 1, fo);
 /* ------------ write the frequency count to the output file ----- */
 fwrite( &freqctr, sizeof freqctr, 1, fo);
 /* ------------ write the frequency array to the output file ------ */
 for ( c=0; c<256 ; c++){
   if ( ht[c].cnt > 0 ){
      fwrite( &ht[c].ch, sizeof (char), 1 , fo);
      fwrite( &ht[c].cnt, sizeof (BYTECOUNTER), 1, fo);
      }
   }
 /* -------- build the Huffman tree  ------------- */
 buildtree();

 /* -------- compress the file --------------- */
 fseek(fi, 0L, 0);
 while (( c=fgetc(fi)) != EOF )
       compress(fo, ht+(c & 255),NULL);
       outbit(fo, -1);
       fclose ( fi );
       fclose ( fo );
 }

 /* ------compress a character value into a bit stream ------ */
 static void compress (FILE *fo,struct htree *h, struct htree *child)
 {
  if ( h->parent != NULL)
     compress(fo , h->parent , h);
  if ( child ){
     if ( child == h->right )
	outbit ( fo , 0 );
     else if (child == h->left)
	  outbit ( fo , 1 );
     }
  }

 static char out8;
 static int ct8;
 /* -- collect and write bits to the compressed output file --- */
 static void outbit(FILE *fo, int bit)
 {
    if ( ct8 == 8 || bit == -1 ){
	while(ct8 <8) {
	      out8<<=1;
	      ct8++;
	}
	fputc(out8, fo);
	ct8 = 0;
    }
    out8 = (out8 << 1 ) | bit;
    ct8++;
 }






