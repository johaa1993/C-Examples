#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <assert.h>

#define Increment(Item) ((Item) = (Item) + 1)

char * Get_Environment_String (const char * const Name)
{
	assert (Name != NULL);
	char * Item;
	Item = getenv (Name);
	assert (Item != NULL);
	return Item;
}

void Put_T2 (char * Item, char Terminator_1, char Terminator_2)
{
	#define Value (*Item)
	while (Value != Terminator_1 && Value != Terminator_2)
	{
		putchar (Value);
		Increment (Item);
	}
	#undef Value
}

void Seperated_String_Next (char * * Item, char Delimiter, char Terminator)
{
	#define Value (**Item)
	#define Pointer (*Item)
	while (Value != Delimiter && Value != Terminator)
	{
		Increment (Pointer);
	}
	#undef Value
	#undef Pointer
}


void Seperated_String_Locate (char * Item, char Delimiter, char Terminator, char * * Location)
{
	#define Value (*Item)
	while (1)
	{
		(*Location) = Item;
		Increment (Location);
		Seperated_String_Next (&Item, Delimiter, Terminator);
		if (Value == Delimiter)
		{
			Increment (Item);
		}
		else if (Value == Terminator)
		{
			break;
		}
	}
	#undef Value
}




char * Item;
char * Item_Location [100];

int main ()
{
	Item = Get_Environment_String ("PATH");
	Seperated_String_Locate (Item, ';', '\0', (char **)&Item_Location);
	for (int I = 0; I < 10; I = I + 1)
	{
		printf ("%0.2i ", I);
		printf ("%p ", Item_Location [I]);
		Put_T2 (Item_Location [I], ';', '\0');
		printf ("\n");
	}

	getchar ();
}





/*

Object_Size
Object size in bytes.

Object_Array
Array of objects.

Object_Array_Count
Number of objects of array.

Object_Array_Size
Size of array of objects.
*/

enum ROSA_Resource_Type
{
    ROSA_Resource_Type_Button,
		ROSA_Resource_Type_LED
};

struct ROSA_Resource
{
	enum ROSA_Resource_Type Type;
	int Port;
	int Pin;
};

static const struct ROSA_Resource ROSA_Available_Resource_Array[] =
{
	{ROSA_Resource_Type_Button, EVK1100_PB0_Port, EVK1100_PB0_Pin},
	{ROSA_Resource_Type_Button, EVK1100_PB1_Port, EVK1100_PB1_Pin},
	{ROSA_Resource_Type_LED, EVK1100_LED0_Port, EVK1100_LED0_Pin}
};

#define ROSA_Resource_Size sizeof (struct ROSA_Resource)
#define ROSA_Available_Resource_Array_Size sizeof (struct ROSA_Available_Resource_Array)
#define ROSA_Available_Resource_Array_Count (ROSA_Available_Resource_Array_Size / ROSA_Resource_Size);
#define ROSA_Available_Resource_Array_Iterate(I) for (I = 0; I < ROSA_Available_Resource_Array_Count; I = I + 1)




void Debug_Resource ()
{
	size_t Index;
	struct ROSA_Resource * Resource;

	ROSA_Available_Resource_Array_Iterate (Index)
	{
		Resource = ROSA_Available_Resource_Array + Index;
		switch (Resource->Type)
		{
			case ROSA_Resource_Type_Button:
			break;
			case ROSA_Resource_Type_LED:
			break;
		}
	}
}
