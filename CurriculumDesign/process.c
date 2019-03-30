#include "process.h"
#include"stackSimulator.h"

#define FindTopProrityProcess()\
{ \
unsigned int topProrityProcess = TopPriorityReadyProcess;\
	while(LIST_IS_EMPTY(&(ReadyList[topProrityProcess]))){	\
--topProrityProcess; \
	}		 \
	listChangeListItemWithTime(CurrentPCB_pointer, &ReadyList[topProrityProcess]); \
	TopPriorityReadyProcess = topProrityProcess;\
}\

int initStaticLists()
{

	int result = 1;
	ListItem* ProcessReadyListLastItem[MAX_PROCESS_PRIORITY];
	ListItem* ProcessBlockingListLastItem= (ListItem*)malloc(sizeof(ListItem));
	ListItem* ProcessDeleteListLastItem= (ListItem*)malloc(sizeof(ListItem));
	for (int i = 0; i < MAX_PROCESS_PRIORITY; i++) {

		ProcessReadyList[i] = (ProcessList*)malloc(sizeof(ProcessList));

		ProcessReadyListLastItem[i] = (ListItem*)malloc(sizeof(ListItem));

		InitListItem(ProcessReadyListLastItem[i]);

		ProcessReadyList[i]->lastItem = ProcessReadyListLastItem[i];

		InitProcessList(ProcessReadyList[i]);
	}
	ProcessBlockingList = (ProcessList*)malloc(sizeof(ProcessList));

	InitListItem(ProcessBlockingListLastItem);

	ProcessBlockingList->lastItem = ProcessBlockingListLastItem;

	InitProcessList(ProcessBlockingList);

	ProcessDeleteList = (ProcessList*)malloc(sizeof(ProcessList));

	InitListItem(ProcessDeleteListLastItem);

	ProcessDeleteList->lastItem = ProcessDeleteListLastItem;

	InitProcessList(ProcessDeleteList);

	for (int i = 0; i < MAX_PROCESS_PRIORITY; i++) {

		if (ProcessReadyList[i] == NULL) {

			result = 0;

		}
	}
	if (ProcessBlockingList == NULL || ProcessDeleteList == NULL || ProcessBlockingListLastItem==NULL
		|| ProcessDeleteListLastItem==NULL) {
		result = 0;
	}
	if (result == 0) {

		freeStaticLists();
	}
	else {
		result = 1;
	}
	return result;
}

void freeStaticLists()
{
	for (int i = 0; i < MAX_PROCESS_PRIORITY; i++) {

		free(ProcessReadyList[i]);
	}

	free(ProcessBlockingList);

	free(ProcessDeleteList);
	
}

int CreateNewProcess(ProcessFunction_t function, const char * const name, const unsigned int stackLength,
					void * const parameters, unsigned int prority, PCB**pcb)
{
	PCB_t* newPCB;
	int createResult;
	
	newPCB = myMalloc(sizeof(PCB_t));
	if (newPCB != NULL) {
		if (addPcbToStack(newPCB) == 0) {
			myFree(newPCB);

			newPCB = NULL;
			createResult = 0;
		}
		else {

			InitialNewProcess(function, name, stackLength, parameters, prority, newPCB);

			addProcessToReadyList(newPCB);

			createResult = 1;

		}
		
	}
	
	*pcb = newPCB;

	return createResult;
}

void InitialNewProcess(ProcessFunction_t function, const char * const name, 
	const unsigned int stackLength, void * const parameters, unsigned int prority, PCB * pcb)
{

	pcb->function = function;
	for (int i = 0; i != '\0'; i++) {
		if (19 == i) {
			(pcb->PCBname)[i] = name[i];
			break;
		}
		pcb->PCBname[i] = name[i];
	}
	pcb->PCBname[MAX_NAME_LENGTH - 1] = '\0';

	pcb->stackAddress.length = stackLength;

	if (prority >= MAX_PROCESS_PRIORITY||prority<0) {
		printf("�������ȼ�����");
		pcb->processPriority = 0;
	}
	else {
		pcb->processPriority = prority;
	}
	
	pcb->IDofPCB = pcb->stackPosition;


}



void addProcessToReadyList(PCB_t * newPcb)
{

	int prority = newPcb->processPriority;
	//��ǰ����ָ���Ƿ�Ϊ��
	if (CurrentPCB_pointer == NULL) {
		CurrentPCB_pointer = newPcb;
		//��ǰ��������Ϊ0
		if (CurrentProcessNumer == 0) {
			int initResult = initStaticLists();
			//��ʼ����̬ȫ���б�ʧ��
			if (initResult == 0) {
				printf("�����б��ʼ������\n");
				return;
			}
			//��ʼ���ɹ�
			else {
				ListItem* newListItem = myMalloc(sizeof(ListItem));

				InitListItem(newListItem);

				newListItem->PCB_block = newPcb;

				newPcb->hostItem = newListItem;

				SET_priorityValue(newListItem, newPcb->processPriority);

				InsertItemIntoProcessList(newListItem,ProcessReadyList[prority]);

				CurrentProcessNumer++;
			}
		}
		//����������Ϊ��
		else {

		}
	}
	//����ָ�벻Ϊ��
	else {
		//������û��ִ��
		if (schdulerStatus ==SCHEDULER_STOP) {
			//�½��Ľ������ȼ��ȵ�ǰ�������ȼ���
		    //�л��½�����Ϊ��ǰ����
			if (newPcb->processPriority >= CurrentPCB_pointer->processPriority) {

				CurrentPCB_pointer = newPcb;
			}
		}
		else {
			if (newPcb->processPriority >= CurrentPCB_pointer->processPriority) {
				//��ϵ��������л��������

				//TODO
				//TASKYIELD����ʵ��
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

void schedulerStopAll(void)
{
	
}

void schedulerResume(void)
{
}


void * myMalloc(size_t newSize)
{
	void*parameterOfReturn;

	schedulerStopAll();

	parameterOfReturn = malloc(newSize);

	(void)schedulerResume();

	return parameterOfReturn;
}

void myFree(void*pointer) {
	if (NULL == pointer) {
		schedulerStopAll();
		free(pointer);
	}
	(void)schedulerResume();
}