/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
						Librer¡a para hacer botones gr ficos en 3-D

Archivo    : BUTTON.C
Objetivo   : Una serie de rutinas para el manejo de botones gr ficos en 3D.
Dise¤o     : Jos‚ Antonio Valderrama Ramos
Implemento : El mismo.
Versi¢n    : 1.00 (Marzo 1994).
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

#include <graphics.h>
#include "button.h"

/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
Funci¢n    : _draw_button.
Objetivo   : Dibuja un bot¢n en la pantalla.
Par metros : int x1,y1,x2,y2 = Coordenadas del bot¢n (Rect ngulo externo).
				 int thick       = Grosor de las sombras.
				 int color       = Color del bot¢n.
				 int shadow      = Color de la Sombra.
				 int upshadow    = Color de la Sombra superior (resalte).
				 int accion      = 0 - el bot¢n est  presionado y
										 1 - no est  presionado.
Salida     : Ninguna.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */
void _draw_button(x1,y1,x2,y2,thick,color,shadow,upshadow,accion)
int x1,y1,x2,y2,thick,color,shadow,upshadow,accion;
{
	int x3,y3,x4,y4;
	int colortemp;

	x3 = x1+thick;
	y3 = y1+thick;
	x4 = x2-thick;
	y4 = y2-thick;

	setcolor(color);
	setfillstyle(SOLID_FILL,color);
	setlinestyle(SOLID_LINE,1,1);
	rectangle(x3--,y3--,x4++,y4++);
	floodfill(x3+2,y3+2,color);

	colortemp = (accion)?upshadow:shadow;              /* Sombra Superior. */
	setcolor(colortemp);
	setfillstyle(SOLID_FILL,colortemp);
	line(x1,y1,x2,y1);
	line(x2,y1,x4,y3);
	line(x4,y3,x3,y3);
	line(x3,y3,x3,y4);
	line(x3,y4,x1,y2);
	line(x1,y2,x1,y1);
	floodfill(x1+1,y1+1,colortemp);

	colortemp = (accion)?shadow:upshadow;              /* Sombra Inferior. */
	setcolor(colortemp);
	setfillstyle(SOLID_FILL,colortemp);
	line(x2,y1,x2,y2);
	line(x2,y2,x1,y2);
	line(x1,y2,x3,y4);
	line(x3,y4,x4,y4);
	line(x4,y4,x4,y3);
	line(x4,y3,x2,y1);
	floodfill(x4+1,y4+1,colortemp);
}

