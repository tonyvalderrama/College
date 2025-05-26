/* -------------------------------------------------------------------------
	Funciones para el manejo de Memoria de Video
	Archivo de definiciones  .................................... VIDEO.H
Version: Septiembre de 1993.
------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

typedef char *MENOP[12];

typedef struct {char x1,x2,y1,y2,*buffer;} PANT;
typedef struct {char atr, somb, atropc, atrbar;} MENATR;
typedef struct {char row, col, stline, endline;} CURSOR;

/* -------------------------------------------------------------------------
	Definiciones
------------------------------------------------------------------------- */
#define INTVID			0x10
#define MAXSCREEN		4000
#define SIG_REN		160
#define _NORMAL		0x0F
#define T_ARR        72
#define T_ABA        80
#define ENTER        13
#define ESC          27


/* -------------------------------------------------------------------------
	Macros
------------------------------------------------------------------------- */
#define POSXY(dir, x,y) (dir)+(x)*2+160*(y)
#define un_car(x,y,c,a) {_VIDEO[(x)*2+160*(y)]=(c); _VIDEO[(x)*2+160*(y)+1]=(a);}
#define PIDE_BUFFER(address) (address)=(char*)malloc(4000)
#define FREE_BUFFER(address)	free((address))

/* -------------------------------------------------------------------------
	Funciones
------------------------------------------------------------------------- */
void inic_vid();

void escribe_car(char, char, char, int, char);
void borra_screen();
void dis_cursor(CURSOR*);
void app_cursor(CURSOR*);

void ventana(char, char, char, char, char, char, char);
void v_explode(char, char, char, char, char, char, char);
void fondo_vent(char, char, char, char, char);
void shadow(char, char, char, char, char);
void change_atr(char, char, char, char, char);

void copia_screen(char*);
void rest_screen(char*);
void copia_area(char, char, char, char, PANT*);
void rest_area(PANT*);

int menu(char, char, char, char, char*[], MENATR*, PANT*);
void close_menu(PANT *);
int getcode();

void desp_cad(char, char, char*, char);
void pide_cad(char, char, char*, char);

