#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include"stackSimulator.h"
#include"process.h"
#include<mmsystem.h>


void test(void*a) {
	/*int* number = &(int*)a;
	int number1 = &number;*/
	int count = &a;
	while (count++) {
		Sleep(50);
		printf("%d\n", count);
		OSstackSimulatorItem_t*placeOfValue = findRunningItem();
		ENTER_CRITICAL();
		{
			placeOfValue->functionValue = count;
		}
		EXIT_CRITICAL();

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

	
	//initOSstackSimulator();
	//CurrentProcessNumer = 0;
	//PCB_t **pcb=malloc(sizeof(PCB));
	//char name[MAX_NAME_LENGTH] = "this is a process";

	//CreateNewProcess(test, name, 1, NULL, 0, pcb); 
	////printf("%d\n", (*STATIC_OS_STACK)->currentDeepth);//1
	//char a[20] = "process is running";
	//(*pcb)->function(a);
	////printf("%d\n",(*pcb)->hostItem->hostList->numberOfProcesses);
	//printf("%d\n",DeleteProcess(*pcb));
	//

	//printf("%d\n%d\n", CurrentProcessNumer,ProcessReadyList[(*pcb)->processPriority]->numberOfProcesses);
	//printf("%d\n", (*STATIC_OS_STACK)->currentDeepth);
	//
	//free(STATIC_OS_STACK);
	//system("pause");



	PCB_t **pcb=malloc(sizeof(PCB));
	char name[MAX_NAME_LENGTH] = "P1";

	CreateNewProcess(test, name, 1, (int*)1, 0, pcb);

	PCB_t **pcb1=malloc(sizeof(PCB));
	char name1[MAX_NAME_LENGTH] = "P2";

	CreateNewProcess(test, name1, 1, (int*)2, 0, pcb1); 

	startScheduler();
	system("pause");
	return 0;
}