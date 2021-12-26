#include "process.h"
#include"stackSimulator.h"
#include<stddef.h>
#include<string.h>
//���ڹ���������ı���
static volatile long SchedulerSuspended = (long)FALSE;

void  FindTopProrityProcess(){ 
	unsigned int topProrityProcess = MAX_PROCESS_PRIORITY - 1;
	while (listIsEmpty(ProcessReadyList[topProrityProcess])) {
			--topProrityProcess;
		}
	//printf("��ǰ������ȼ���%d\n", topProrityProcess);
	listChangeListItemWithTime(ProcessReadyList[topProrityProcess]);
	TopPriorityReadyProcess = topProrityProcess;
}

void proSELECT_HIGHEST_PRIORITY_PROCESS(){
	unsigned int uxTopPriority=TopPriorityReadyProcess;
	FindTopProrityProcess();
	listChangeListItemWithTime(ProcessReadyList[ uxTopPriority ] );
}

//ʱ��Ƭ��ʱ�л��б���
void listChangeListItemWithTime(ProcessList * list){
	ProcessList* ConstList = (list);
	//printf("�л�֮ǰ�Ľ���id��%d\n", CurrentPCB_pointer->IDofPCB);
	ConstList->ListItemIndex = ConstList->ListItemIndex->next;
	if (ConstList->ListItemIndex == ConstList->lastItem) {
		ConstList->ListItemIndex = ConstList->ListItemIndex->next;
	}
	CurrentPCB_pointer = ConstList->ListItemIndex->PCB_block;
	//printf("����ִ�еĽ���id��%d\n", CurrentPCB_pointer->IDofPCB);
}

int initStaticLists(){

	int result = 1;
	ListItem* ProcessReadyListLastItem[MAX_PROCESS_PRIORITY];
	ListItem* ProcessBlockingListLastItem= (ListItem*)malloc(sizeof(ListItem));
	ListItem* ProcessDeleteListLastItem= (ListItem*)malloc(sizeof(ListItem));
	//��ʼ�����ȼ�����
	for (int i = 0; i < MAX_PROCESS_PRIORITY; i++) {
		ProcessReadyList[i] = (ProcessList*)malloc(sizeof(ProcessList));
		ProcessReadyListLastItem[i] = (ListItem*)malloc(sizeof(ListItem));
		InitListItem(ProcessReadyListLastItem[i]);
		ProcessReadyList[i]->lastItem = ProcessReadyListLastItem[i];
		InitProcessList(ProcessReadyList[i]);
		SET_LIST_STATE(ProcessReadyList[i], LISTReady);
	}
	//��ʼ����������
	ProcessBlockingList = (ProcessList*)malloc(sizeof(ProcessList));
	InitListItem(ProcessBlockingListLastItem);
	ProcessBlockingList->lastItem = ProcessBlockingListLastItem;
	InitProcessList(ProcessBlockingList);
	SET_LIST_STATE(ProcessBlockingList,LISTBlocking);
	ProcessDeleteList = (ProcessList*)malloc(sizeof(ProcessList));
	InitListItem(ProcessDeleteListLastItem);
	ProcessDeleteList->lastItem = ProcessDeleteListLastItem;
	InitProcessList(ProcessDeleteList);
	SET_LIST_STATE(ProcessDeleteList,LISTDelete);
	for (int i = 0; i < MAX_PROCESS_PRIORITY; i++) {
		if (ProcessReadyList[i] == NULL) {
			result = 0;
		}
	}
	if (ProcessBlockingList == NULL || ProcessDeleteList == NULL || ProcessBlockingListLastItem==NULL
		|| ProcessDeleteListLastItem==NULL) {
		result = 0;
	}
	/*if (result == 0) {

		freeStaticLists();
	}
	else {
		result = 1;
	}*/
	return result;
}

void freeStaticLists(){
	for (int i = 0; i < MAX_PROCESS_PRIORITY; i++) {
		free(ProcessReadyList[i]);
	}
	free(ProcessBlockingList);
	free(ProcessDeleteList);
}

int CreateNewProcess(ProcessFunction_t function, 
					const char * const name, 
					const unsigned int stackLength,
					void * const parameters,	 
					unsigned int prority, 
					PCB**pcb,time_t runTime){
	PCB_t* newPCB;
	int createResult;
	newPCB = myMalloc(sizeof(PCB_t));
	if (newPCB != NULL) {
		if (addPcbToStack(newPCB,parameters) == 0) {
			myFree(newPCB);
			newPCB = NULL;
			createResult = 0;
		}
		else {
			InitialNewProcess(function, name, stackLength, parameters, prority, newPCB,runTime);
			addProcessToReadyList(newPCB);
			newPCB->status = READY;
			createResult = 1;
		}
	}
	*pcb = newPCB;
	return createResult;
}

void InitialNewProcess(ProcessFunction_t function, 
						const char * const name, 
						const unsigned int stackLength, 
						void * const parameters, 
						unsigned int prority, 
						PCB * pcb,
						time_t runTime){

	pcb->function = function;
	/*for (int i = 0; i != '\0'; i++) {
		if (19 == i) {
			(pcb->PCBname)[i] = name[i];
			break;
		}
		pcb->PCBname[i] = name[i];
	}
	pcb->PCBname[MAX_NAME_LENGTH - 1] = '\0';*/
	strcpy_s(pcb->PCBname, MAX_NAME_LENGTH,name);
	pcb->stackAddress.length = stackLength;

	if (prority >= MAX_PROCESS_PRIORITY||prority<0) {
		printf("�������ȼ�����");
		pcb->processPriority = 0;
	}
	else {
		pcb->processPriority = prority;
	}
	
	pcb->IDofPCB = pcb->stackPosition;
	//printf("����id��%d\n", pcb->IDofPCB);
	//��ʼ����ջ�еĽ��������ʼ����
	pcb->runTime = runTime;

}

void addProcessToReadyList(PCB_t * newPcb){
	int prority = newPcb->processPriority;
	//��ǰ����ָ���Ƿ�Ϊ��
	if (CurrentPCB_pointer == NULL) {
		CurrentPCB_pointer = newPcb;
		//��ǰ��������Ϊ0
		if (CurrentProcessNumer == 0 && ProcessReadyList==NULL) {
			int initResult = initStaticLists();
			//��ʼ����̬ȫ���б�ʧ��
			if (initResult == 0) {
				printf("�����б���ʼ������\n");
				return;
			}
			//��ʼ���ɹ�
			else {
				ListItem* newListItem = myMalloc(sizeof(ListItem));
				//��ʼ��������
				InitListItem(newListItem);
				newListItem->PCB_block = newPcb;
				newPcb->hostItem = newListItem;

				SET_priorityValue(newListItem, newPcb->processPriority);
				InsertItemIntoProcessList(newListItem,ProcessReadyList[prority]);
				//newListItem->PCB_block.status = READY;
				if (newPcb->processPriority >= TopPriorityReadyProcess) {
					TopPriorityReadyProcess = newPcb->processPriority;
				}
				//printf("%d %d", newPcb->processPriority, TopPriorityReadyProcess);
				CurrentProcessNumer++;
			}
		}
		//����������Ϊ��
		else {
			ListItem* newListItem = myMalloc(sizeof(ListItem));
			InitListItem(newListItem);
			newListItem->PCB_block = newPcb;
			newPcb->hostItem = newListItem;

			SET_priorityValue(newListItem, newPcb->processPriority);
			InsertItemIntoProcessList(newListItem, ProcessReadyList[prority]);
			//newListItem->PCB_block.status = READY;
			if ((newPcb->processPriority) >= TopPriorityReadyProcess) {
				TopPriorityReadyProcess = newPcb->processPriority;
			}
			//printf("%d %d", newPcb->processPriority, TopPriorityReadyProcess);
			CurrentProcessNumer++;
		}
	}
	//����ָ�벻Ϊ��
	else {
		//������û��ִ��
		if (schdulerStatus ==SCHEDULER_STOP) {
			//�½��Ľ������ȼ��ȵ�ǰ�������ȼ���
		    //�л��½�����Ϊ��ǰ����
			if (newPcb->processPriority >= CurrentPCB_pointer->processPriority) {
				TopPriorityReadyProcess = newPcb->processPriority;
				CurrentPCB_pointer->status = READY;
				CurrentPCB_pointer = newPcb;
			}
			//printf("%d %d", newPcb->processPriority, TopPriorityReadyProcess);
		}
		else {
			if (newPcb->processPriority >= CurrentPCB_pointer->processPriority) {
				//��ϵ��������л��������
				schedulerStopAll();
				TopPriorityReadyProcess = newPcb->processPriority;
				CurrentPCB_pointer->status = READY;
				CurrentPCB_pointer = newPcb;
				schedulerResume();
			}
		}

		ListItem* newListItem = myMalloc(sizeof(ListItem));
		InitListItem(newListItem);
		newListItem->PCB_block = newPcb;
		newPcb->hostItem = newListItem;

		SET_priorityValue(newListItem, newPcb->processPriority);
		InsertItemIntoProcessList(newListItem, ProcessReadyList[prority]);
		CurrentProcessNumer++;
	}
}

int DeleteProcess(PCB * pcb){
	PCB* pcbToDelete=pcb;
	ListItem* hostItemOfpcbToDelete = pcbToDelete->hostItem;
	int preNumber = hostItemOfpcbToDelete->hostList->numberOfProcesses;
	int result = 0;
	//���Ҫɾ���Ľ����ǵ�ǰ����
	//����ǰ�б�ָ��ָ���б������һ��
	if (pcbToDelete == CurrentPCB_pointer) {
		if (pcbToDelete->hostItem->hostList->numberOfProcesses == 1) {
			CurrentPCB_pointer = NULL;
		}
		else{
			CurrentPCB_pointer = pcbToDelete->hostItem->hostList->lastItem->next->PCB_block;
		}
		CurrentProcessNumer--;
	}
	else{
		CurrentProcessNumer--;
	}

	if ( DeleteFromList(hostItemOfpcbToDelete)!=(preNumber-1)) {
		printf("����ɾ��ʧ��\n");
		result = 0;
	}
	else {
		if (0 == deletePcbFromStack(pcb->IDofPCB)) {
			printf("��ջ���ʧ��\n");
			return 0;
		}
		result = 1;
		myFree(pcb);
	}
	return result;
}

int BlockedProcess(int pcbID){
	//�ҵ�Ҫ������PCB
	PCB_t*pcbToBlock = findPCB_ById(pcbID);
	//��Ӧ�����ȼ�
	int prority = pcbToBlock->processPriority;
	if (0 == ProcessReadyList[prority]->numberOfProcesses) {
		//printf("�����Ѿ��˳���\n");
		return 0;
	}
	//ListItem* ItemOfRemoveToBlockList = pcbToBlock->hostItem;
	//printf("���ƿ����ȼ���%d,�б������ȼ�:%d",pcbToBlock->processPriority,ItemOfRemoveToBlockList->priorityValue);
	/*printf("����������%d",ItemOfRemoveToBlockList->hostList->numberOfProcesses);*/
	//ENTER_CRITICAL();
	else{
		DeleteFromList(pcbToBlock->hostItem);
		InsertItemIntoProcessList(pcbToBlock->hostItem,ProcessBlockingList);
		blocking_signal = 1;
		pcbToBlock->status = BLOCKING;
		return 1;
	}
	//EXIT_CRITICAL();
	
}

int WakeupProcess(int pcbID){
	//�ҵ�Ҫ���ѵ�PCB
	PCB_t*pcbToReady = findPCB_ById(pcbID);
	int prority = pcbToReady->processPriority;
	//�ҵ���Ӧ���б���
	//ListItem* ItemOfRemoveToReadList = pcbToReady->hostItem;
	//ENTER_CRITICAL();
	{
		DeleteFromList(pcbToReady->hostItem);
		InsertItemIntoProcessList(pcbToReady->hostItem, ProcessReadyList[prority]);
		pcbToReady->status = READY;
	}
	//EXIT_CRITICAL();
	return 1;
}

void schedulerStopAll(void){
	//�ȴ��ж��ź������൱�ڹ��ж�
	//WaitForSingleObject(timeInterruptMutex, INFINITE);
	WaitForSingleObject(modifyListMutex,INFINITE);
	
}

void schedulerResume(void){
	ReleaseMutex(modifyListMutex);
}


void * myMalloc(size_t newSize){
	void*parameterOfReturn;
	schedulerStopAll();
	parameterOfReturn = malloc(newSize);
	(void)schedulerResume();
	return parameterOfReturn;
}

void myFree(void*pointer) {
	if (NULL != pointer) {
		schedulerStopAll();
		free(pointer);
		(void)schedulerResume();
	}
}


void processSwitchContext(){
	if (schdulerStatus != SCHEDULER_STOP) {
		xYieldPending = TRUE;
	}
	else {
		xYieldPending = FALSE;
		proSELECT_HIGHEST_PRIORITY_PROCESS();
	}
}

//���̴�������(ִ�н��̺���)
DWORD WINAPI processThreadFun(LPVOID param){
		FindTopProrityProcess();
		//printf("������ߵ����ȼ���%d\n",TopPriorityReadyProcess);
		//printf("����ִ�еĽ��̣�%s\n", CurrentPCB_pointer->PCBname);
		int*value=NULL;
		//PCB_t*current = CurrentPCB_pointer;
		value = (int*)findFunValueByPcbID(CurrentPCB_pointer->IDofPCB);
		//printf("�ҵ���ֵ��%d\n",value);
		//printf("���̣�%d\n",CurrentPCB_pointer->IDofPCB);
		//ִ�н��̵ĺ���
		//��������Ҫ��¼���̺�����ֵ ��Ҫ�ڽ��̺������ڲ���ģ��Ķ�ջ�м�¼(��Ϊ�ڽ��̹���������û�з���ֵ��)
		CurrentPCB_pointer->status = RUNNING;
	    (CurrentPCB_pointer->function)(value);
		
	
}

void startScheduler(){
	//����һ������ ģ�������
	processThread = CreateThread(NULL, 0, processThreadFun, NULL, 0, NULL);
	while(1) {
		WaitForSingleObject(INTERRUPTION,INFINITE);
		//WaitForSingleObject(toKillProcessThread,INFINITE);
		if (exit_signal == TRUE) {
			//printf("Ҫɾ���Ľ��̣�%s\n",(*processExitBuf)->pcb->PCBname);
			DeleteProcess((*processExitBuf)->pcb);
			//OSstackSimulatorItem*iter = (*STATIC_OS_STACK)->startSimulatorItem->next;
			//ListItem*item = ProcessReadyList[6]->lastItem->next;
			//for (int i = 0;; i++) {
			//	if (iter == ProcessReadyList[6]->lastItem) {
			//		break;
			//	}
			//	//printf("���б��еĽ�������:%s\n", ((PCB*)item->PCB_block)->PCBname);
			//	printf("���̺���ֵ�Ƿ�Ϊ��:%s\n",iter->functionValue==NULL);
			//	iter = iter->next;
			//	
			//FindTopProrityProcess();
			exit_signal = FALSE;
		}
		TerminateThread(processThread, 0);
		//ReleaseMutex(modifyListMutex);
		//ReleaseMutex(timeInterruptMutex);
		if(CurrentPCB_pointer != NULL) {
			CurrentPCB_pointer->status = READY;
		}
		if (blocking_signal = 1&&CurrentPCB_pointer!=NULL) {
			CurrentPCB_pointer->status = BLOCKING;
			blocking_signal = 0;
		}
		processThread=CreateThread(NULL, 0, processThreadFun, NULL, 0, NULL);
	}
}

void runInFreeTime(void*a) {
	while (1) {
		Sleep(tickTime / 2);
		//printf("ϵͳ��ǰ����\n");
	}
}

int listIsEmpty(ProcessList * list){
	if (list->numberOfProcesses == 0){
		return 1;
	}
	else{
		return 0;
	}
}



