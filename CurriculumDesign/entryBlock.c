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
	int count = a;
	while (count++) {
		clock_t start = clock();
		Sleep(200);
		//WaitForSingleObject(toKillProcessThread, INFINITE);
		OSstackSimulatorItem_t*placeOfValue = findRunningItem();
		//printf("%s\n",placeOfValue->pcb->PCBname);
		printf("%d\n",count);
		
		ENTER_CRITICAL();
		{
			placeOfValue->functionValue = (int*)count;
			
			//printf("�������\n");
			//printf("%d\n",(*STATIC_OS_STACK)->startSimulatorItem->next->functionValue);
		}
		EXIT_CRITICAL();
		//ReleaseSemaphore(toKillProcessThread,1,NULL);
		clock_t end = clock()-start;
		placeOfValue->pcb->runTime -= end;
		if (placeOfValue->pcb->runTime < 0) {
			
			(*processExitBuf)->pcb = placeOfValue->pcb;
			
			exit_signal = TRUE;
			ExitThread(0);
		}
		printf("ʣ��ʱ��:%d\n", placeOfValue->pcb->runTime);
	}
}

void test1(void*a) {

	while (1) {
		Sleep(200);
		printf("process2 is running...\n");
	}
}

void test0(void*a) {
	while (1) {
		Sleep(200);
		printf("process3 is running...\n");
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
	//		printf("�����ջʧ��");
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


	//���һЩ��ʼ��
	initOSstackSimulator();
	initStaticLists();
	initSemphores();
	exit_signal = 0;
	processExitBuf =malloc(sizeof(EXIT_PROCESS));

	(*processExitBuf) = (EXIT_PROCESS*)malloc(sizeof(EXIT_PROCESS));
	CurrentProcessNumer = 0;
	TopPriorityReadyProcess = 0;

	PCB_t **pcb2 = malloc(sizeof(PCB));
	char name2[MAX_NAME_LENGTH] = "P3";

	CreateNewProcess(test1, name2, 1, (int*)12, 6, pcb2,2*CLOCKS_PER_SEC);

	PCB_t **pcb1=malloc(sizeof(PCB));
	char name1[MAX_NAME_LENGTH] = "P2";

	CreateNewProcess(test0, name1, 11, (int*)19, 6, pcb1, 2 * CLOCKS_PER_SEC);


	PCB_t **pcb = malloc(sizeof(PCB));
	char name[MAX_NAME_LENGTH] = "P1";

	CreateNewProcess(test, name, 1, (int*)4, 6, pcb, 2 * CLOCKS_PER_SEC);

	//OSstackSimulatorItem*iter = (*STATIC_OS_STACK)->startSimulatorItem->next;
	/*ListItem*item = ProcessReadyList[6]->lastItem->next;
	for (int i = 0; i < 3; i++) {
		
		printf("1����id:%d\n",((PCB*)item->PCB_block)->IDofPCB);
		item = item->next;
	}*/
	
	CreateTimer();
	startScheduler();
	free(*processExitBuf);
	system("pause");
	return 0;
}