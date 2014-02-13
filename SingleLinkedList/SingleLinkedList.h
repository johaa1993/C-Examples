#ifndef SingleLinkedList_H
#define SingleLinkedList_H

#include <stdlib.h>


//private:
typedef struct _List_Node _List_Node;
struct _List_Node
{
	void* item;
	_List_Node* next;
};



//public:
typedef struct List List;
struct List
{
	_List_Node* first;
	int size;
};

extern const List LIST_EMPTY;

void List_insert(List* list, const void* item);

void List_remove(List* list, const void* item);

void List_foreach(const List* list, void(*callback)(void*));

#endif