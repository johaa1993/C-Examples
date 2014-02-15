#include <stdio.h>

#include "Vector2D.h"




void main()
{

	Vector2D a = { 0.0, 0.0 };
	Vector2D_print(&a);
	Vector2D_norm(&a, &a);
	Vector2D_print(&a);

	printf("\n\nPROGRAM END");
	while (1);
}

