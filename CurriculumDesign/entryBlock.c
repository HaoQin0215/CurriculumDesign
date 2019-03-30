#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include"stackSimulator.h"
#include"process.h"
#include"pcbUtil.h"



void test(void*a) {
	/*int* number = &(int*)a;
	int number1 = &number;*/
	int number1 = 0;
	while (number1<=3) {
		Sleep(1000);
		printf("%d\n", number1++);
	}
}
int main() {
	//ListItem* item=(ListItem*)malloc(sizeof(ListItem));
	//ProcessList* list=(ProcessList*)malloc(sizeof(ProcessList));
	//ListItem* listEnditem = (ListItem*)malloc(sizeof(ListItem));
	//listEnditem->priorityValue = 27;
	//
	//list->lastItem = listEnditem;
	//InitListItem(item);
	//InitProcessList(list);
	//SET_priorityValue(item,29);
	//printf("%d\n", listEnditem==list->lastItem);
	//InsertItemIntoProcessList(item, list);
	//printf("%d\n", list->lastItem->priorityValue);
	//printf("%d\n", GET_priorityValue(list->lastItem->next));
	//DeleteFromList(item);

	//
	//free(item);
	//free(list);
	//free(listEnditem);

	
	//initOSstackSimulator();
	//PCB_t *pcb[30];
	//for (int i = 0; i < 30; i++) {
	//	pcb[i] = (PCB_t*)malloc(sizeof(PCB_t));
	//	if (0 == addPcbToStack(pcb[i])) {
	//		printf("¼ÓÈë¶ÑÕ»Ê§°Ü");
	//	};
	//	printf("%d", pcb[i]->stackPosition);
	//}
	//for (int j = 0; j < 30; j++) {
	//	free(pcb[j]);
	//}
	initOSstackSimulator();
	
	PCB_t **pcb=malloc(sizeof(PCB));
	char name[MAX_NAME_LENGTH] = "this is a process";

	CreateNewProcess(test, name, 1, NULL, 0, pcb); 
	printf("%u\n%u\n", (*pcb)->IDofPCB, (*pcb)->processPriority);
	system("pause");
	return 0;
}