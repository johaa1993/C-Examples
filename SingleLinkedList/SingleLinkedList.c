#include "SingleLinkedList.h"

const List LIST_EMPTY = { NULL, 0 };

void List_insert(List* list, const void* item)
{
	_List_Node* node = (_List_Node*)malloc(sizeof(_List_Node));
	node->item = item;
	if (list->first)
	{
		node->next = list->first;
	}
	else
	{
		node->next = NULL;
	}
	list->first = node;
	list->size++;
}

void List_remove(List* list, const void* item){
	if (list->first == NULL)
	{
		list->size--;
		return;
	}
	if (list->first->item == item)
	{
		list->first = list->first->next;
		list->size--;
		return;
	}
	_List_Node* iterator = list->first;
	while (iterator->next)
	{
		if (iterator->next->item == item)
		{
			iterator->next = iterator->next->next;
			list->size--;
			return;
		}
		iterator = iterator->next;
	}
}

void List_foreach(const List* list, void(*callback)(void*))
{
	if (list->first == NULL)
	{
		return;
	}
	_List_Node* node = list->first;
	while (node)
	{
		callback(node->item);
		node = node->next;
	}
}