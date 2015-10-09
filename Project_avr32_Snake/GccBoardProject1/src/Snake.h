/*
Light weight snake package.
Snake environment is not implemented only pure snake logic is used.
Works for all platforms.
*/

#ifndef SNAKE_H_
#define SNAKE_H_

#define Snake_Count 1
#define Snake_Body_Max_Count 40
#define Snake_Direction_Left 0
#define Snake_Direction_Right 1
#define Snake_Direction_Up 2
#define Snake_Direction_Down 3
#define Snake_Direction_None 4


struct Snake;

struct Snake * Snake_malloc ();
void Snake_Init (struct Snake * S);

//Returns the snake direction it took.
int Snake_Head_Update (struct Snake * S);

void Snake_Tail_Update (struct Snake * S);

//Check if the snake head is colliding with any of it's own body.
int Snake_Collision_Self (struct Snake * S);

int Snake_Collision_Wall (struct Snake * S, int Width, int Height);

//The edges become connected.
//https://www.youtube.com/watch?v=5qu3WETuf6c
int Snake_Flat_Torus_Logic (struct Snake * S, int Width, int Height);

void Snake_Set_Direction (struct Snake * S, int Direction);

int Snake_Head_X (struct Snake * S);
int Snake_Head_Y (struct Snake * S);
int Snake_Tail_X (struct Snake * S);
int Snake_Tail_Y (struct Snake * S);

#endif