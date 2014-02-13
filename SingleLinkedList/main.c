#include <stdio.h>
#include <windows.h>
#include "SingleLinkedList.h"

void printer(void* item)
{
	char* cString = (char*)item;
	printf("item: %s\n", cString);
	Sleep(100);
}

void main()
{
	List list = LIST_EMPTY;
	char* e1 = "e1";
	char* e2 = "e2";
	char* e3 = "e3";
	char* e4 = "e4";
	List_insert(&list, e1);
	List_insert(&list, e2);
	List_insert(&list, e3);
	List_insert(&list, e4);
	List_foreach(&list, printer);
	List_remove(&list, e4);
	List_remove(&list, e2);
	List_remove(&list, e2);
	List_remove(&list, e4);
	printf("\n\n");
	List_foreach(&list, printer);
	printf("\n\nPROGRAM END");
	while (1);
}