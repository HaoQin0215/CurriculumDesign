#include "list.h"
#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
int main() {
	ListItem* item=(ListItem*)malloc(sizeof(ListItem));
	ProcessList* list=(ProcessList*)malloc(sizeof(ProcessList));
	ListItem* listEnditem = (ListItem*)malloc(sizeof(ListItem));
	listEnditem->priorityValue = 27;
	
	list->lastItem = listEnditem;
	InitListItem(item);
	InitProcessList(list);
	SET_priorityValue(item,29);
	printf("%d\n", listEnditem==list->lastItem);
	InsertItemIntoProcessList(item, list);
	printf("%d\n", list->lastItem->priorityValue);
	printf("%d\n", GET_priorityValue(list->lastItem->next));
	DeleteFromList(item);

	
	free(item);
	free(list);
	free(listEnditem);
	system("pause");
	return 0;
}