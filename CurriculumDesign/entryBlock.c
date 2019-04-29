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
		//printf("���̵����н��(����д���ڴ�):%d\n",count);
		
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
			return;
		}
		//printf("ʣ��ʱ��:%d\n", placeOfValue->pcb->runTime);
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
void printInformation();
DWORD WINAPI displayFun(LPVOID param) {
	int choice;
	char name[MAX_NAME_LENGTH];
	int paramter;
	int prority;
	time_t runTime;
	PCB_t**pcbToCreate;
	int pcbIdToBlock = -1;
	int pcbIdToWakeUp = -1;
	int pcbIdToDelete = -1;
	PCB*pcbTemp;
	for (;;) {
		printf("|--------------------------------------------------------------------------------------------------|\n");
		printf("|��ѡ����Ҫ���еĲ�����(1)�������� (2)�������� (3)���ѽ��� (4)��ֹ���� (5)��ʾ������Ϣ (0)�˳�����|\n");
		scanf_s("%d", &choice);
		switch (choice) {
		case 0:
			return;
		case 1:
			printf("|-------------------------------------------��������------------------------------------------------|\n");
			printf("|�������ƣ�");
			scanf_s("%s", name, MAX_NAME_LENGTH);
			printf("\n|���̺�����ʼ������");
			scanf_s("%d", &paramter);
			printf("\n|�������ȼ���");
			scanf_s("%d", &prority);
			printf("\n|��������ʱ��:");
			scanf_s("%lld", &runTime);
			pcbToCreate = (PCB_t**)malloc(sizeof(PCB_t));
			int result = CreateNewProcess(test, name, 1, (int*)paramter, prority, pcbToCreate, runTime);
			printf("|�����ɹ�!\n");

			break;
		case 2:
			printf("|-------------------------------------------��������----------------------------------------------|\n");
			printf("|�����Ľ���ID��");
			scanf_s("%d", pcbIdToBlock);
			BlockedProcess(pcbIdToBlock);
			printf("|�����ɹ���\n");
			break;
		case 3:
			printf("|-------------------------------------------���ѽ���----------------------------------------------|\n");
			printf("|Ҫ���ѵĽ���ID:");
			scanf_s("%d", pcbIdToWakeUp);
			WakeupProcess(pcbIdToWakeUp);
			printf("|���ѳɹ���\n");
			break;
		case 4:
			printf("|-------------------------------------------��ֹ����-----------------------------------------------|\n");
			printf("|Ҫ��ֹ�Ľ���ID:");
			scanf_s("%d", pcbIdToDelete);
			pcbTemp = findPCB_ById(pcbIdToDelete);
			DeleteProcess(pcbTemp);
			printf("|��ֹ�ɹ���\n");
			break;
		case 5:
			printf("|------------------------------------------��ʾ������Ϣ--------------------------------------------|\n");
			printf("��������\t����ID\t����״̬\t�������ȼ�\t��������ʣ��ʱ��\n");
			printInformation();
			break;
		default:
			printf("������������������!\n");
		}
	}
}
void printInformation() {

	OSstackSimulatorItem*iter = (*STATIC_OS_STACK)->startSimulatorItem->next;

	for (int i = 0; ; i++) {
			if (iter == iter->next) break;
			if (iter->pcb!= NULL) {
					printf("  %s  \t\t  %d  \t %d \t\t   %d   \t\t      %d   \n",
						iter->pcb->PCBname, iter->pcb->IDofPCB, iter->pcb->status,
						iter->pcb->processPriority, iter->pcb->runTime);
			
				iter = iter->next;
			}
	}
	/*for (int i = 0;; i++) {
		item = ProcessBlockingList->lastItem->next;
		if (item == ProcessBlockingList->lastItem) break;
		if (item != RunningItem) {
			printf("  %s  \t  %d  \t Ready \t   %d   \t      %d   \n",
				((PCB*)item->PCB_block)->PCBname, ((PCB*)item->PCB_block)->IDofPCB,
				((PCB*)item->PCB_block)->processPriority, ((PCB*)item->PCB_block)->runTime);
		}
	}*/
	printf("\n");
	

}

int main() {
	

	//���һЩ��ʼ��
	initOSstackSimulator();
	initStaticLists();
	initSemphores();
	exit_signal = 0;
	processExitBuf =malloc(sizeof(EXIT_PROCESS));

	(*processExitBuf) = (EXIT_PROCESS*)malloc(sizeof(EXIT_PROCESS));
	CurrentProcessNumer = 0;
	TopPriorityReadyProcess = 30;


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


	//BlockedProcess((*pcb)->IDofPCB);
	//printf("���ȼ���������3�Ƿ�Ϊ�գ�%d\n",LIST_IS_EMPTY(ProcessReadyList[3]));
	//WakeupProcess((*pcb)->IDofPCB);
	//printf("���ȼ���������3�Ƿ�Ϊ�գ�%d\n", LIST_IS_EMPTY(ProcessReadyList[3]));
	//printf("%s\n",((PCB_t*)(ProcessReadyList[4]->lastItem->PCB_block))->PCBname);
	
	CreateTimer();
	HANDLE display = CreateThread(NULL, 0, displayFun, NULL, 0, NULL);
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
	freeStaticLists();
	system("pause");
	return 0;
}