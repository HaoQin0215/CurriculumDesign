#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<time.h>



typedef struct ProcessList ProcessList;
typedef struct ListItem ListItem;
//typedef struct subordinateListItem subordinateListItem;
typedef struct ListItem {
	clock_t runTime;
	ListItem* next;
	ListItem* previous;
	void* PCB_block;
	ProcessList* hostList;
	unsigned priorityValue;

}ListItem;

//typedef struct subordinateListItem {
//	ListItem* next;
//	ListItem* previous;
//	unsigned priorityValue;
//}subordinateListItem;

typedef struct ProcessList {
	volatile unsigned numberOfProcesses;
	void* ListItemIndex;
	ListItem* lastItem;
	void * listType;
	
}ProcessList;

#define setListPCB_Pointer(ListItem,PCBpointer) ((ListItem)->PCB_block=(void*)PCBpointer)

//定义最多进程数
#define MAX_PORCESS_NUMBER 20

//定义最低优先级
#define MAX_subordinateListItemValue 30

//获得当前链表进程数
#define GET_LIST_NUMBER(List) ((List)->numberOfProcesses)
//获得当前列表项优先级
#define GET_priorityValue(Item) ((Item)->priorityValue)
//设置当前列表项优先级
#define SET_priorityValue(Item,value) ((Item)->priorityValue=value)


void InitProcessList(ProcessList* list);

void InitListItem(ListItem* item);

void InsertItemIntoProcessList(ListItem* item, ProcessList* list);

void InsertItemToListEnd(ListItem*item, ProcessList* list);

int DeleteFromList(ListItem*item);






