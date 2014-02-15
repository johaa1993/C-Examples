#ifndef Vector2D_H
#define Vector2D_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Vector2D Vector2D;
struct Vector2D{
	double x;
	double y;
};

extern const Vector2D VECTOR2D_ZERO;

void Vector2D_add(Vector2D* result, const Vector2D* a, const Vector2D* b);
void Vector2D_sub(Vector2D* result, const Vector2D* a, const Vector2D* b);
void Vector2D_norm(Vector2D* result, const Vector2D* a);
void Vector2D_print(const Vector2D* a);

#endif