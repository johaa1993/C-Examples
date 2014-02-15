#include <stdio.h>

#include "SingleLinkedList.h"

typedef struct Planet Planet;
struct Planet{
	char name[20];
	int mass;
};

void printer(void* item)
{
	Planet* planet = (Planet*)item;
	printf("name: %s ", planet->name);
	printf("mass: %i\n", planet->mass);
}



void main()
{
	Planet planet1 = { "Pluto", 200 };
	Planet planet2 = { "Earth", 10 };
	Planet planet3 = { "Jupiter", 123 };
	List list = LIST_EMPTY;
	List_insert(&list, &planet1);
	List_insert(&list, &planet2);
	List_insert(&list, &planet3);
	List_foreach(&list, printer);
	List_remove(&list, &planet2);
	printf("\n\n");
	List_foreach(&list, printer);
	printf("\n\nPROGRAM END");
	while (1);
}

