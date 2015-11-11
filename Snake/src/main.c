#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "Snake.h"

#define Map_Width 30
#define Map_Height 10
#define Map_Count (Map_Width * Map_Height)
#define Map_Index(X,Y) ((X) + (Map_Width * (Y)))
#define Map_Element(X,Y) Map[Map_Index(X,Y)]
char Map[Map_Count];

void Spawn_Apple ()
{
	Map_Element (rand () % Map_Width, rand () % Map_Height) = 'A';
}

int main ()
{
	int X;
	int Y;
	char C;
	struct Snake * Snake_0;
	Snake_0 = Snake_malloc ();
	Spawn_Apple ();
	Spawn_Apple ();
	Spawn_Apple ();
	while (1)
	{
		C = getch ();
		system ("cls");
		switch (C)
		{
			case 'a':
			Snake_Set_Direction (Snake_0, Snake_Direction_Left);
			break;
			case 'd':
			Snake_Set_Direction (Snake_0, Snake_Direction_Right);
			break;
			case 'w':
			Snake_Set_Direction (Snake_0, Snake_Direction_Up);
			break;
			case 's':
			Snake_Set_Direction (Snake_0, Snake_Direction_Down);
			break;
		}
		Snake_Head_Update (Snake_0);
		Snake_Closed_Space (Snake_0, Map_Width, Map_Height);
		if (Snake_Collision_Self (Snake_0))
		{
			printf("%s\n", "Snake_Collision_Self");
		}
		C = Map_Element (Snake_Head_X (Snake_0), Snake_Head_Y (Snake_0));
		if (C == 'A')
		{
			Spawn_Apple ();
		}
		else
		{
			Map_Element (Snake_Tail_X (Snake_0), Snake_Tail_Y (Snake_0)) = ' ';
			Snake_Tail_Update (Snake_0);
		}
		Map_Element (Snake_Head_X (Snake_0), Snake_Head_Y (Snake_0)) = 'S';
		for (Y = 0; Y < Map_Height; Y = Y + 1)
		{
			for (X = 0; X < Map_Width; X = X + 1)
			{
				printf ("%c", Map_Element (X, Y));
			}
			printf ("\n");
		}
	}
}
