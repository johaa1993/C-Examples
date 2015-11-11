#include <string.h>
#include <stdlib.h>
#include "Snake.h"


void Snake_Ring_Pointer_Forward (int * I)
{
	#define Index (*I)
	Index++;
	if (Index == Snake_Body_Count)
	{
		Index = 0;
	}
	#undef Index
}


struct Segment
{
	int X;
	int Y;
};

struct Snake
{
	//The snake is defined between Head_Pointer and Tail_Pointer.
	int Head_Pointer;
	int Tail_Pointer;
	//This will decide where the snake goes when Snake_Head_Update is called.
	int Direction;
	//Ring buffer for the snake's body segment position.
	//Only one body segment is used initially.
	//Number of body segment will grow if calling Snake_Head_Update without Snake_Tail_Update.
	struct Segment Body[Snake_Body_Count];
};

void Snake_Init (struct Snake * S)
{
	S->Head_Pointer = 0;
	S->Tail_Pointer = 0;
	S->Direction = Snake_Direction_None;
	S->Body[S->Head_Pointer].X = 0;
	S->Body[S->Head_Pointer].Y = 0;
}

//As far as I know opaque struct cannot be statically declared.
struct Snake * Snake_malloc ()
{
	struct Snake * S;
	S = (struct Snake *)malloc(sizeof (struct Snake));
	Snake_Init (S);
	return S;
}

int Snake_Head_Update (struct Snake * S)
{
	int Head_Pointer_Old;
	if (S->Direction != Snake_Direction_None)
	{
		//Copy the old body segment and put it to
		Head_Pointer_Old = S->Head_Pointer;
		Snake_Ring_Pointer_Forward (&S->Head_Pointer);
		S->Body[S->Head_Pointer] = S->Body[Head_Pointer_Old];
		switch (S->Direction)
		{
			case Snake_Direction_Down:
			S->Body[S->Head_Pointer].Y += 1;
			break;

			case Snake_Direction_Up:
			S->Body[S->Head_Pointer].Y -= 1;
			break;

			case Snake_Direction_Left:
			S->Body[S->Head_Pointer].X -= 1;
			break;

			case Snake_Direction_Right:
			S->Body[S->Head_Pointer].X += 1;
			break;
		}
	}
	return S->Direction;
}

void Snake_Tail_Update (struct Snake * S)
{
	if (S->Direction != Snake_Direction_None)
	{
		Snake_Ring_Pointer_Forward (&S->Tail_Pointer);
	}
}


int Snake_X (struct Snake * S, int Index)
{
	return S->Body[Index].X;
}

int Snake_Y (struct Snake * S, int Index)
{
	return S->Body[Index].Y;
}

int Snake_Head_X (struct Snake * S)
{
	return Snake_X (S, S->Head_Pointer);
}

int Snake_Head_Y (struct Snake * S)
{
	return Snake_Y (S, S->Head_Pointer);
}

int Snake_Tail_X (struct Snake * S)
{
	return Snake_X (S, S->Tail_Pointer);
}

int Snake_Tail_Y (struct Snake * S)
{
	return Snake_Y (S, S->Tail_Pointer);
}


int Snake_Collision_Body (struct Snake * S, int Body_Index)
{
	return (Snake_Head_X (S) == Snake_X (S, Body_Index)) && (Snake_Head_Y (S) == Snake_Y (S, Body_Index));
}

int Snake_Collision_Self (struct Snake * S)
{
	//Begin to search collision from the tail.
	int Pointer = S->Tail_Pointer;
	while (1)
	{
		if (Pointer == S->Head_Pointer)
		{
			//Snake is length one or no collision was found after the entire snake was checked for collision.
			return 0;
		}
		if (Snake_Collision_Body(S, Pointer))
		{
			return 1;
		}
		//No collision was found then proceed to next body.
		Snake_Ring_Pointer_Forward (&Pointer);
	}
}


int Snake_Collision_Wall (struct Snake * S, int Width, int Height)
{
	return (Snake_Head_X (S) >= Width) || (Snake_Head_X (S) < 0) || (Snake_Head_Y (S) >= Height) || (Snake_Head_Y (S) < 0);
}


int Snake_Closed_Space (struct Snake * S, int Width, int Height)
{
	if (Snake_Head_X (S) >= Width)
	{
		S->Body[S->Head_Pointer].X = 0;
	}
	else if (Snake_Head_X (S) < 0)
	{
		S->Body[S->Head_Pointer].X = Width - 1;
	}
	else if (Snake_Head_Y (S) >= Height)
	{
		S->Body[S->Head_Pointer].Y = 0;
	}
	else if (Snake_Head_Y (S) < 0)
	{
		S->Body[S->Head_Pointer].Y = Height - 1;
	}
}


void Snake_Set_Direction (struct Snake * S, int Direction)
{
	S->Direction = Direction;
}
