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

//������������
#define MAX_PORCESS_NUMBER 20

//����������ȼ�
#define MAX_subordinateListItemValue 30

//��õ�ǰ���������
#define GET_LIST_NUMBER(List) ((List)->numberOfProcesses)
//��õ�ǰ�б������ȼ�
#define GET_priorityValue(Item) ((Item)->priorityValue)
//���õ�ǰ�б������ȼ�
#define SET_priorityValue(Item,value) ((Item)->priorityValue=value)
//�жϵ�ǰ�б��Ƿ�Ϊ��
#define LIST_IS_EMPTY(list) ((list->numberOfProcesses==0)?1:0)

//ʱ��Ƭ��ʱ�л��б���
#define listChangeListItemWithTime(pcb,list) {\
	ProcessList*const ConstList = (list);\
	ConstList->ListItemIndex = ConstList->ListItemIndex->next;\
	if ((void*)ConstList->ListItemIndex == (void*)ConstList->lastItem) {\
		ConstList->ListItemIndex = ConstList->ListItemIndex->next;\
	}\
	pcb = ConstList->ListItemIndex->PCB_block;\
}\
//����б��Ƿ񱻳�ʼ��
#define listIS_INITIAL(list) (list->lastItem->priorityValue==MAX_subordinateListItemValue)

//�����б���ֵ
#define listSetListItemValue(listItem,value) ((listItem)->runTime=value)

void InitProcessList(ProcessList* list);

void InitListItem(ListItem* item);

void InsertItemIntoProcessList(ListItem* item, ProcessList* list);

void InsertItemToListEnd(ListItem*item, ProcessList* list);

int DeleteFromList(ListItem*item);






