#include "Vector2D.h"


const Vector2D VECTOR2D_ZERO = { 0, 0 };


void Vector2D_add(Vector2D* result, const Vector2D* a, const Vector2D* b)
{
	result->x = a->x + b->x;
	result->y = a->y + b->y;
}


void Vector2D_sub(Vector2D* result, const Vector2D* a, const Vector2D* b)
{
	result->x = a->x - b->x;
	result->y = a->y - b->y;
}


//Zero vector is forbidden
void Vector2D_norm(Vector2D* result, const Vector2D* a){
	double r = sqrt(a->x * a->x + a->y*a->y);
	if ( r == 0 )
	{
		printf("Vector2D.h: Zero vector cannot be normilized.");
		while (1);
	}
	result->x = a->x / r;
	result->y = a->y / r;
}


void Vector2D_print(const Vector2D* a)
{
	printf("( %f, %f )\n",a->x,a->y);
}