#include "list.h"

//��ʼ������
void InitProcessList(ProcessList * list){
	list->ListItemIndex = list->lastItem;
	list->numberOfProcesses = 0;
	list->lastItem->previous = list->lastItem;
	list->lastItem->next = list->lastItem;
	list->listType = LISTonINIT;
	list->ListItemIndex = list->lastItem;
	list->lastItem->priorityValue = MAX_subordinateListItemValue;
}
//��ʼ��������
void InitListItem(ListItem * item){
	item->hostList = NULL;
}

//����һ�����������
void InsertItemIntoProcessList(ListItem * item, ProcessList * list){
	ListItem* itemIterator;
	volatile unsigned newPriorityValue = item->priorityValue;
	//printf("%d\n",newPriorityValue);
	if (newPriorityValue == MAX_subordinateListItemValue) {
		itemIterator = list->lastItem->previous;
	}
	else {
		for (itemIterator = list->lastItem; itemIterator->next->priorityValue <= newPriorityValue;
			itemIterator = itemIterator->next);
	}
	item->next = itemIterator->next;
	item->previous = itemIterator;
	item->next->previous = item;
	itemIterator->next = item;
	item->hostList = list;
	list->numberOfProcesses += 1;
}

//����һ�����������ĩβ
void InsertItemToListEnd(ListItem * item, ProcessList * list){
	if (list->numberOfProcesses==0) {
		list->lastItem->next = item;
		list->lastItem->previous = item;
		item->next = (ListItem*)&(list->lastItem);
		item->previous = (ListItem*)&(list->lastItem);
		item->hostList = list;
	}
	else {
		ListItem * itemIterator = list->lastItem->previous;
		itemIterator->next = item;
		item->previous = itemIterator;
		item->next = (ListItem*)&(list->lastItem);
		list->lastItem->previous = item;
	}
	list->numberOfProcesses += 1;

}

//�Ƴ�һ��������
int DeleteFromList(ListItem * item){
	ProcessList* hostList = item->hostList;
	//printf("%d",hostList->numberOfProcesses);
	item->next->previous = item->previous;
	item->previous->next = item->next;
	if (hostList->ListItemIndex == item) {
		hostList->ListItemIndex = item->previous;
	}
	item->hostList = NULL;
	(hostList->numberOfProcesses) --;
	//free(item);
	return hostList->numberOfProcesses;
}

void SET_LIST_STATE(ProcessList * list,LIST_STATUS status){
	list->listType = status;
}
