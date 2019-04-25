#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include"stackSimulator.h"
#include"process.h"
#include<mmsystem.h>


void test(void*a) {
	srand(time(NULL));
	/*int* number = &(int*)a;
	int number1 = &number;*/
	int count = a;
	while (count++) {
		clock_t start = clock();
		Sleep(rand()%100+50);
		//WaitForSingleObject(toKillProcessThread, INFINITE);
		OSstackSimulatorItem_t*placeOfValue = findRunningItem();
		//printf("%s\n",placeOfValue->pcb->PCBname);
		printf("进程的运行结果(将被写会内存):%d\n",count);
		
		ENTER_CRITICAL();
		{
			placeOfValue->functionValue = (int*)count;
			
			//printf("更新完成\n");
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
		printf("剩余时间:%d\n", placeOfValue->pcb->runTime);
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
void pr(int id) {
	OSstackSimulatorItem*iter = (*STATIC_OS_STACK)->startSimulatorItem->next;
	ListItem*item = ProcessReadyList[id]->lastItem->next;

	for (int i = 0; i < 3; i++) {
		if (item == ProcessReadyList[id]->lastItem) break;
		/*int value = findFunValueByPcbID(iter->pcb->IDofPCB);*/
		printf("1进程id:%s\n", iter->pcb->PCBname);
		item = item->next;
	}
	printf("\n");
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
	//		printf("加入堆栈失败");
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


	//完成一些初始化
	initOSstackSimulator();
	initStaticLists();
	initSemphores();
	exit_signal = 0;
	processExitBuf =malloc(sizeof(EXIT_PROCESS));

	(*processExitBuf) = (EXIT_PROCESS*)malloc(sizeof(EXIT_PROCESS));
	CurrentProcessNumer = 0;
	TopPriorityReadyProcess = 0;


	PCB_t **freeProcess = malloc(sizeof(PCB));
	char name3[MAX_NAME_LENGTH] = "FreeProcess";

	CreateNewProcess(runInFreeTime, name3, 1, NULL, 0, freeProcess, INFINITE * CLOCKS_PER_SEC);

	PCB_t **pcb2 = malloc(sizeof(PCB));
	char name2[MAX_NAME_LENGTH] = "P3";

	CreateNewProcess(test, name2, 1, (int*)12, 1, pcb2,4*CLOCKS_PER_SEC);

	PCB_t **pcb1=malloc(sizeof(PCB));
	char name1[MAX_NAME_LENGTH] = "P2";

	CreateNewProcess(test, name1, 11, (int*)19, 2, pcb1, 1* CLOCKS_PER_SEC);


	PCB_t **pcb = malloc(sizeof(PCB));
	char name[MAX_NAME_LENGTH] = "P1";

	CreateNewProcess(test, name, 1, (int*)4, 3, pcb, 2 * CLOCKS_PER_SEC);

	
	//printf("%s\n",((PCB_t*)(ProcessReadyList[4]->lastItem->PCB_block))->PCBname);
	CreateTimer();
	startScheduler();
	/*pr(1);
	DeleteProcess(*pcb);
	pr(1);

	DeleteProcess(*pcb1);
	pr(1);

	DeleteProcess(*pcb2);
	pr(1);
*/
	free(*processExitBuf);
	system("pause");
	return 0;
}