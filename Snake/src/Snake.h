#ifndef SNAKE_H_
#define SNAKE_H_


#define Snake_Body_Count 40
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
int Snake_Closed_Space (struct Snake * S, int Width, int Height);

void Snake_Set_Direction (struct Snake * S, int Direction);

int Snake_Head_X (struct Snake * S);
int Snake_Head_Y (struct Snake * S);
int Snake_Tail_X (struct Snake * S);
int Snake_Tail_Y (struct Snake * S);

#endif
