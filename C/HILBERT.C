/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
											 Fractales
M¢dulo     : Funciones para curvas de Hilbert.
Archivo    : HILBERT.C.
Objetivo   : Funciones para el dibujado de curvas de Hilbert.
Dise¤o     : Jos‚ Antonio Valderrama Ramos  -  90120509-9.
Implemento : El mismo.
Versi¢n    : 1.00 (Mayo 1994).
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <graphics.h>
#include <math.h>

const int SquareSize = 512;
int i,x0,y0,u;
char ch;

void ini_gui(),a(int),b(int),c(int),d(int);

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : main.
Objetivo   : Arrancar el sistema.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void main()
{
	ini_gui();
	x0 = getmaxx() / 2;
	y0 = getmaxy() / 2;
	u = SquareSize;
	i = 0;
	do
		{
		i++;
		u = u / 2;
		x0 += (u / 2);
		y0 += (u / 2);
		moveto(x0,y0);
		a(i);
		if (kbhit())
		  ch = getch();
		}
	while ((ch != 13) && (i != 6));
	getch();
	closegraph();
}

/* -------------------------------------------------------------------------
FUNCION   :	_line
OBJETIVO  : Mover el cursor en direcci¢n 45 * i grados, d pixeles;
PARAMETROS: int i, d;
SALIDA    : Ninguna
-------------------------------------------------------------------------- */
void _line(int i, int d)
{	float direcc;
	int x = getx(), y = gety();

	switch(i)
	{	case 0:
			x += d;
			break;
		case 2:
			y += d;
			break;
		case 4:
			x -= d;
			break;
		case 6:
			y -= d;
	}
	lineto(x,y);
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : ini_gui.
Objetivo   : Inicializa gr ficos.
Par metros : Ninguno.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void ini_gui()
{
	int driver=VGA,mode=VGAHI;

	initgraph(&driver,&mode,"");           /* Inicializo modo gr fico VGA. */
	if (graphresult())
		{
		printf("\nNo puedo inicializar modo gr fico, perdone.\n");
		exit(0);
		}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : a.
Objetivo   : Primera funci¢n recursiva de Hilbert.
Par metros : int i = un valor necesario.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void a(int x)
{	if(x > 0)
	{	d(x-1); _line(4, u); a(x - 1); _line(6, u);
		a(x-1); _line(0, u); b(x - 1);
	}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : b.
Objetivo   : Segunda funci¢n recursiva de Hilbert.
Par metros : int i = un valor necesario.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void b(int x)
{  if(x > 0)
	{	c(x-1); _line(2, u); b(x - 1); _line(0, u);
		b(x-1); _line(6, u); a(x - 1);
	}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : c.
Objetivo   : Tercera funci¢n recursiva de Hilbert.
Par metros : int i = un valor necesario.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void c(int x)
{   if(x > 0)
	{	b(x-1); _line(0, u); c(x - 1); _line(2, u);
		c(x-1); _line(4, u); d(x - 1);
	}
}

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : d.
Objetivo   : Cuarta funci¢n recursiva de Hilbert.
Par metros : int i = un valor necesario.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void d(int x)
{	if(x > 0)
	{	a(x-1); _line(6, u); d(x - 1); _line(4, u);
		d(x-1); _line(2, u); c(x - 1);
	}
}
