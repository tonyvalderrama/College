/*--------------------------------------------------------------------------
M¢dulo     : Funciones para el Mouse.
Archivo    : MOUSE.C.
Objetivo   : Funciones que permiten el uso del Mouse.
Dise§o     : JosÇ Antonio Valderrama Ramos,
				 Jorge Eduardo Aparicio Mayorga,
				 HÇctor Villal¢n PÇrez.
Implemento : El mismo.
Versi¢n    : Noviembre 1993.
--------------------------------------------------------------------------*/

#include <dos.h>
#include "mouse.h"

int _star[] =
		  { /* M†scara de la pantalla. */
		  0xFE7F,0xFE7F,0xFE7F,0xFC3F,0xFC3F,0x0,0x8001,0xE007,
		  0xF00F,0xF81F,0xF00F,0xF18F,0xF3CF,0xE7E7,0xEFF7,0xDFFB,
		  /* M†scara del Cursor.  */
		  0x0180, /*       €€       */
		  0x0180, /*       €€       */
		  0x0180, /*       €€       */
		  0x03C0, /*      €€€€      */
		  0x03C0, /*      €€€€      */
		  0xFFFF, /*€€€€€€€€€€€€€€€€*/
		  0x7FFE, /* €€€€€€€€€€€€€€ */
		  0x1FF8, /*   €€€€€€€€€€   */
		  0x0FF0, /*    €€€€€€€€    */
		  0x07E0, /*     €€€€€€     */
		  0x0FF0, /*    €€€€€€€€    */
		  0x0E70, /*    €€€  €€€    */
		  0x0C30, /*    €€    €€    */
		  0x1818, /*   €€      €€   */
		  0x1008, /*   €        €   */
		  0x2004, /*  €          €  */ };
		int _smily[]= { /* M†scara de la Pantalla. */
		  0xFFFF, /*€€€€€€€€€€€€€€€€*/
		  0xF81F, /*€€€€€      €€€€€*/
		  0xE7E7, /*€€€  €€€€€€  €€€*/
		  0xDDBB, /*€€ €€€ €€ €€€ €€*/
		  0xBDBD, /*€ €€€€ €€ €€€€ €*/
		  0xBDBD, /*€ €€€€ €€ €€€€ €*/
		  0x7DBE, /* €€€€€ €€ €€€€€ */
		  0x7FFE, /* €€€€€€€€€€€€€€ */
		  0x77EE, /* €€€ €€€€€€ €€€ */
		  0x77EE, /* €€€ €€€€€€ €€€ */
		  0xBBDD, /*€ €€€ €€€€ €€€ €*/
		  0xBC3D, /*€ €€€€    €€€€ €*/
		  0xDFFB, /*€ €€€€€€€€€€€ €€*/
		  0xE7E7, /*€€€  €€€€€€  €€€*/
		  0xF81F, /*€€€€€      €€€€€*/
		  0xFFFF, /*€€€€€€€€€€€€€€€€*/
		  /* M†scara del Cursor.  */
		  0x0,0x07E0,0x1818,0x2244,0x4242,0x4242,0x8241,0x8001,
		  0x8811,0x8811,0x4422,0x43C2,0x2004,0x1818,0x07E0,0x0 };
		int _watch[]= { /* M†scara de la Pantalla. */
		  0xFFFF,0xF87F,0xE79F,0xDDEF,0xBDF7,0xBDF7,0x7DFB,0x7DF8,
		  0x7C38,0x7FFB,0xBFF7,0xBFF7,0xDFEF,0xE79F,0xF87F,0xFFFF,
		  /* M†scara del Cursor. */
		  0x0   , /*                */
		  0x0780, /*     €€€€       */
		  0x1860, /*   €€    €€     */
		  0x2210, /*  €   €    €    */
		  0x4208, /* €    €     €   */
		  0x4208, /* €    €     €   */
		  0x8204, /*€     €      €  */
		  0x8207, /*€     €      €€€*/
		  0x83C7, /*€     €€€€   €€€*/
		  0x8004, /*€            €  */
		  0x4008, /* €          €   */
		  0x4008, /* €          €   */
		  0x2010, /*  €        €    */
		  0x1860, /*   €€    €€     */
		  0x0780, /*     €€€€       */
		  0x0     /*                */ };
		int _finger[] = { /* M†scara de la pantalla. */
		  0xFBFF,0xF5FF,0xF5FF,0xF5FF,0xF5FF,0xE4FF,0xD50F,0xD557,
		  0xD40F,0xDFDF,0xDFDF,0xEFDF,0xF7DF,0xF7BF,0xF87F,0xFFFF,
		  /* M†scara del Cursor. */
		  0x0400, /*     €          */
		  0x0A00, /*    € €         */
		  0x0A00, /*    € €         */
		  0x0A00, /*    € €         */
		  0x0A00, /*    € €         */
		  0x1B00, /*   €€ €€        */
		  0x2AF0, /*  € € € €€€€    */
		  0x2AA8, /*  € € € € € €   */
		  0x2BF0, /*  € € €€€€€€    */
		  0x2020, /*  €       €     */
		  0x2020, /*  €       €     */
		  0x1020, /*   €      €     */
		  0x0820, /*    €     €     */
		  0x0840, /*    €    €      */
		  0x0780, /*     €€€€       */
		  0x0 };
		int _intsign[] =
		  { /* M†scara para Pantalla. */
		  0xFC3F,0xF00F,0xE3C7,0xE7E7,0xFFE7,0xFFC7,0xFE0F,0xFE3F,
		  0xFE7F,0xFE7F,0xFE7F,0xFE7F,0xFFFF,0xFFFF,0xFE7F,0xFE7F,
		  /* M†scara para el Cursor. */
		  0x03C0, /*      €€€€       */
		  0x0FF0, /*    €€€€€€€€     */
		  0x1C38, /*   €€€    €€€    */
		  0x1818, /*   €€      €€    */
		  0x0018, /*           €€    */
		  0x0038, /*          €€€    */
		  0x01F0, /*       €€€€€     */
		  0x01C0, /*       €€€       */
		  0x0180, /*       €€        */
		  0x0180, /*       €€        */
		  0x0180, /*       €€        */
		  0x0180, /*       €€        */
		  0x0,    /*                 */
		  0x0,    /*                 */
		  0x0180, /*       €€        */
		  0x0180  /*       €€        */ };
		int _disk[] = { /* M†scara para la Pantalla. */
		  0x0,0x0,0x0003,0x0003,0x0,0x03C0,0x03C0,0x03D0,
		  0x03C0,0x0,0x0,0x0180,0x0180,0x180,0x0180,0x0,
		  /* M†scara para el Cursor. */
		  0xFFFF, /*€€€€€€€€€€€€€€€€€*/
		  0xFFFF, /*€€€€€€€€€€€€€€€€€*/
		  0xFFFC, /*€€€€€€€€€€€€€€€  */
		  0xFFFC, /*€€€€€€€€€€€€€€€  */
		  0xFFFF, /*€€€€€€€€€€€€€€€€€*/
		  0xFC3F, /*€€€€€€    €€€€€€€*/
		  0xFC3F, /*€€€€€€    €€€€€€€*/
		  0xFC27, /*€€€€€€    €  €€€€*/
		  0xFC3F, /*€€€€€€    €€€€€€€*/
		  0xFFFF, /*€€€€€€€€€€€€€€€€€*/
		  0xFE7F, /*€€€€€€€  €€€€€€€€*/
		  0xFE7F, /*€€€€€€€  €€€€€€€€*/
		  0xFE7F, /*€€€€€€€  €€€€€€€€*/
		  0xFE7F, /*€€€€€€€  €€€€€€€€*/
		  0xFFFF  /*€€€€€€€€€€€€€€€€€*/ };
		int _heart[]={ /* M†scara para la Pantalla. */
		  0xFFFF,0xC183,0x8181,0x0180,0x0,0x0,0x0,0x0,
		  0x8001,0x8001,0xC003,0xF00F,0xF81F,0xFC3F,0xFE7F,0xFFFF,
		  /* M†scara del Cursor. */
		  0x0,    /*                */
		  0x3E7C, /*  €€€€€  €€€€€  */
		  0x7E7E, /* €€€€€€  €€€€€€ */
		  0xFE7F, /*€€€€€€€  €€€€€€€*/
		  0xFFFF, /*€€€€€€€€€€€€€€€€*/
		  0xFFFF, /*€€€€€€€€€€€€€€€€*/
		  0xFFFF, /*€€€€€€€€€€€€€€€€*/
		  0xFFFF, /*€€€€€€€€€€€€€€€€*/
		  0x7FFE, /* €€€€€€€€€€€€€€ */
		  0x7FFE, /* €€€€€€€€€€€€€€ */
		  0x3FFC, /*  €€€€€€€€€€€€  */
		  0x0FF0, /*    €€€€€€€€    */
		  0x07E0, /*     €€€€€€     */
		  0x03C0, /*      €€€€      */
		  0x0180, /*       €€       */
		  0x0     /*                */ };
		int _patyyyo[] = { /* M†scara para la Patalla. */
		  0x1115,0x55B5,0x31BB,0x75BB,0xFFFF,0xFFFF,0xFEBF,0xFEBF,
		  0xFF7F,0xFF7F,0xFFFF,0xFFFF,0x116A,0xB52A,0xB54D,0xB16D,
		  /* M†scara para el Cursor.*/
		  0xEEEA, /*€€€ €€€ €€€ € € */
		  0xAA4A, /*€ € € €  €  € € */
		  0xCE44, /*€€  €€€  €   €  */
		  0x8A44, /*€   € €  €   €  */
		  0x0,    /*                */
		  0x0,    /*                */
		  0x0140, /*       € €      */
		  0x0140, /*       € €      */
		  0x0080, /*        €       */
		  0x0080, /*        €       */
		  0x0,    /*                */
		  0x0,    /*                */
		  0xEE95, /*€€€ €€€ €  € € €*/
		  0x4AD5, /* €  € € €€ € € €*/
		  0x4AB2, /* €  € € € €€  € */
		  0x4E92  /* €  €€€ €  €  € */
		  };
                int _flecha[]=
                  {
                  0x8FFF,0x07FF,0x03FF,0x018F,0x808F,0xC007,0xE007,0xF007,
                  0xF803,0xC003,0xC001,0xC001,0xF800,0xFF00,0xFFC0,0xFFF0,

                  0x0000,0x2000,0x7000,0x3800,0x1C20,0x0E20,0x0730,0x03B0,
                  0x01F0,0x00F8,0x1FF8,0x03FC,0x007C,0x001E,0x0006,0x0000
                  };

/*--------------------------------------------------------------------------
Funci¢n    : _inic_mouse.
Objetivo   : Verifica que existe el driver, el mouse y lo inicializa.
Par†metros : Ninguno.
Salida     : 0 en caso de operaci¢n infructuosa,
				 0xFFFF en caso de operaci¢n exitosa.
--------------------------------------------------------------------------*/
int _inic_mouse()
{
	struct REGPACK reg;
	reg.r_ax=0;
	intr(0x33,&reg);
	return reg.r_ax;
}

/*--------------------------------------------------------------------------
Funci¢n    : _def_cur_graph.
Objetivo   : Establece un cursor definido en MOUSE.H.
Par†metros : int form = N£mero que define el n£mero de cursor a usar.
				 Valores v†lidos:
					 1 - Una estrella,
					 2 - Una carita sonriente,
					 3 - Un reloj,
					 4 - Un dedo,
					 5 - Un signo de interrogaci¢n,
                                         6 - Un diskette de 5.25",
                                         7 - Un coraz¢n,
                                         8 - Un mensaje amoroso, y
                                         9 - Flecha.
Salida     : Devuelve un 0 en caso de error, 1 en caso contrario.
--------------------------------------------------------------------------*/
int _def_cur_graph(int form)
{
	struct REGPACK reg;
	reg.r_ax = 0x9;
	switch (form)
		{
		case _STAR:
			reg.r_bx = 8;
			reg.r_cx = -1;
			reg.r_es = FP_SEG(_star);
			reg.r_dx = FP_OFF(_star);
			break;
		case _SMILY:
			reg.r_bx = 8;
			reg.r_cx = 8;
			reg.r_es = FP_SEG(_smily);
			reg.r_dx = FP_OFF(_smily);
			break;
		case _WATCH:
			reg.r_bx = 8;
			reg.r_cx = 8;
			reg.r_es = FP_SEG(_watch);
			reg.r_dx = FP_OFF(_watch);
			break;
		case _FINGER:
			reg.r_bx = 6;

			reg.r_cx = -1;
			reg.r_es = FP_SEG(_finger);
			reg.r_dx = FP_OFF(_finger);
			break;
		case _INTSIGN:
			reg.r_bx = 8;
			reg.r_cx = 8;
			reg.r_es = FP_SEG(_intsign);
			reg.r_dx = FP_OFF(_intsign);
			break;
		case _DISK:
			reg.r_bx = 8;
			reg.r_cx = 8;
			reg.r_es = FP_SEG(_disk);
			reg.r_dx = FP_OFF(_disk);
			break;
		case _HEART:
			reg.r_bx = 8;
			reg.r_cx = 8;
			reg.r_es = FP_SEG(_heart);
			reg.r_dx = FP_OFF(_heart);
			break;
		case _PATYYYO:
                        reg.r_bx = 8;
			reg.r_cx = 8;
			reg.r_es = FP_SEG(_patyyyo);
			reg.r_dx = FP_OFF(_patyyyo);
			break;
                case _FLECHA:
                        reg.r_bx = 16;
                        reg.r_cx = 16;
                        reg.r_es = FP_SEG(_flecha);
                        reg.r_dx = FP_OFF(_flecha);
                        break;
		default:
			return 0;
		}
	intr(0x33,&reg);
	return 1;
}

/*--------------------------------------------------------------------------
Funci¢n    : _show_hide_cur.
Objetivo   : Pone en la pantalla el cursro del mouse, o lo quita.
Par†metros : int accion = Si es 1, prendo el cursor; 2 lo apaga.
Salida     : Ninguna.
--------------------------------------------------------------------------*/
void _show_hide_cur(int accion)
{
	struct REGPACK regs;
	regs.r_ax=accion;
	intr(0x33,&regs);
}

/*--------------------------------------------------------------------------
Funci¢n    : _pos_status.
Objetivo   : Determinar en d¢nde se encuentra el rat¢n en la pantalla y el
				 estado actual de los botones.
Par†metros : int *x,*y = Punteros a donde se va a dejar la posici¢n.
Salida     : 1 si est† presionado el bot¢n izquierdo,
				 2 si est† presionado el derecho,
				 3 si est† presionado el central y
				 0 si no est† presionado ninguno.
--------------------------------------------------------------------------*/
int _pos_status(int *x,int *y)
{
	struct REGPACK regs;
	int boton;
	regs.r_ax = 0x3;
	intr(0x33,&regs);
	*x = regs.r_cx;
	*y = regs.r_dx;
	boton = regs.r_bx;
	boton &= 1;
	if (boton)                             /* Revisi¢n de bot¢n izquierdo. */
		return 1;
	boton = regs.r_bx;
	boton &= 2;
	if (boton)                               /* Revisi¢n de bot¢n derecho. */
		return 2;
	boton = regs.r_bx;
	boton &= 4;
	if (boton)                   /* Revisi¢n de bot¢n central (si existe). */
		return 3;
	return 0;                        /* Si no fue ning£n bot¢n, regresa 0. */
}

/*--------------------------------------------------------------------------
Funci¢n    : _pos_cur.
Objetivo   : Posicionar el cursor del rat¢n en una coordenada indicada.
Par†metros : int col,row = Coordenadas en donde se va a posicionar.
Salida     : Ninguna.
--------------------------------------------------------------------------*/
void _pos_cur(int col,int row)
{
	struct REGPACK reg;
	reg.r_ax = 0x4;
	reg.r_cx = col;
	reg.r_dx = row;
	intr(0x33,&reg);
}

/*--------------------------------------------------------------------------
Funci¢n    : _cur_range.
Objetivo   : Definir el †rea de movimiento del mouse.
Par†metros : int x1,y1 = Coordenadas de esquina superior izquierda,
				 int x2,y2 = Coordenadas de esquina inferior derecha.
Salida     : Ninguna.
--------------------------------------------------------------------------*/
void _cur_range(int x1,int y1,int x2,int y2)
{
	struct REGPACK reg;
	reg.r_ax = 0x7;
	reg.r_cx = x1;
	reg.r_dx = x2;
	intr(0x33,&reg);
	reg.r_ax = 0x8;
	reg.r_cx = y1;
	reg.r_dx = y2;
	intr(0x33,&reg);
}

/*--------------------------------------------------------------------------
Funci¢n    : _check_button.
Objetivo   : Revisa un bot¢n del mouse.
Par†metros : int *x,*y = Aqu° regreso coordenadas de 'on 'ta el mouse,
				 int boton = Indica el bot¢n que quiero revisar:
											0  -  bot¢n izquierdo,
											1  -  derecho y
											2  -  central.
Salida     : El n£mero de veces que se presion¢ el bot¢n.
--------------------------------------------------------------------------*/
int _check_button(int *x,int *y,int boton)
{
	struct REGPACK regs;
	regs.r_ax = 0x5;
	regs.r_bx = boton;
	intr(0x33,&regs);
	*x = regs.r_cx;
	*y = regs.r_dx;
	return (regs.r_bx);
}

/*--------------------------------------------------------------------------
Funci¢n    : _clean_mouse.
Objetivo   : Dejar el estado del rat¢n de tal modo que indique que
				 no est† presionado y listo para una revisi¢n.
Par†metros : Ninguno.
Salida     : Ninguna.
--------------------------------------------------------------------------*/
void _clean_mouse()
{	int x, y;
	while(_pos_status(&x,&y));
}