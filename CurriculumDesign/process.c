#include "process.h"
#include"stackSimulator.h"
#include<stddef.h>
//用于挂起调度器的变量
static volatile long SchedulerSuspended = (long)FALSE;

#define FindTopProrityProcess()\
{ \
unsigned int topProrityProcess = TopPriorityReadyProcess;\
	while(LIST_IS_EMPTY(&(ProcessReadyList[topProrityProcess]))){	\
--topProrityProcess; \
	}		 \
	listChangeListItemWithTime(CurrentPCB_pointer, &ProcessReadyList[topProrityProcess]); \
	TopPriorityReadyProcess = topProrityProcess;\
}\

#define proSELECT_HIGHEST_PRIORITY_PROCESS()\
{\
unsigned int uxTopPriority=TopPriorityReadyProcess;\
FindTopProrityProcess();\
listGET_OWNER_OF_NEXT_ENTRY( CurrentPCB_pointer, &( ProcessReadyList[ uxTopPriority ] ) );\
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

		SET_LIST_STATE(ProcessReadyList[i], LISTReady);
	}
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

			newPCB->status = READY;

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
		printf("进程优先级错误");
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
	//当前进程指针是否为空
	if (CurrentPCB_pointer == NULL) {
		CurrentPCB_pointer = newPcb;
		//当前进程数量为0
		if (CurrentProcessNumer == 0) {
			int initResult = initStaticLists();
			//初始化静态全局列表失败
			if (initResult == 0) {
				printf("就绪列表初始化错误\n");
				return;
			}
			//初始化成功
			else {
				ListItem* newListItem = myMalloc(sizeof(ListItem));

				InitListItem(newListItem);

				newListItem->PCB_block = newPcb;

				newPcb->hostItem = newListItem;

				SET_priorityValue(newListItem, newPcb->processPriority);

				InsertItemIntoProcessList(newListItem,ProcessReadyList[prority]);

				/*newListItem->PCB_block.status = READY;*/
				

				CurrentProcessNumer++;
			}
		}
		//进程数量不为零
		else {

		}
	}
	//进程指针不为空
	else {
		//调度器没有执行
		if (schdulerStatus ==SCHEDULER_STOP) {
			//新建的进程优先级比当前进程优先级高
		    //切换新建进程为当前进程
			if (newPcb->processPriority >= CurrentPCB_pointer->processPriority) {

				CurrentPCB_pointer = newPcb;
			}
		}
		else {
			if (newPcb->processPriority >= CurrentPCB_pointer->processPriority) {
				//打断调度器，切换任务进程

				//TODO
				//TASKYIELD函数实现
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

int DeleteProcess(PCB * pcb)
{
	PCB* pcbToDelete=pcb;
	
	ListItem* hostItemOfpcbToDelete = pcbToDelete->hostItem;
	int preNumber = hostItemOfpcbToDelete->hostList->numberOfProcesses;
	int result = 0;
	//如果要删除的进程是当前进程
	//将当前列表指针指向列表项的下一个
	if (pcbToDelete == CurrentPCB_pointer) {
		if (pcbToDelete->hostItem->hostList->numberOfProcesses == 1) {
			CurrentPCB_pointer = NULL;
		}
		else {
			CurrentPCB_pointer = pcbToDelete->hostItem->hostList->lastItem->next->PCB_block;
		}
		CurrentProcessNumer--;
	}
	else {
		CurrentProcessNumer--;
	}

	if ( DeleteFromList(hostItemOfpcbToDelete)!=(preNumber-1)) {
		//printf("进程删除失败\n");
		result = 0;
	}
	else {
		if (0 == deletePcbFromStack(pcb->IDofPCB)) {
			//printf("堆栈清除失败\n");
			return 0;
		}
		
		
		result = 1;
		myFree(pcb);
	}

	return result;
}

void schedulerStopAll(void)
{
	++SchedulerSuspended;
	
}

void schedulerResume(void)
{
	PCB_t*pcb;
	long alreadyYielded = FALSE;
	En
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

long increaseTicks()
{
	PCB_t pcb;
	uint32_t itemValue;

	return 0;
}

void processSwitchContext()
{
	if (schdulerStatus != SCHEDULER_STOP) {
		xYieldPending = TRUE;
	}
	else {
		xYieldPending = FALSE;
		proSELECT_HIGHEST_PRIORITY_PROCESS();
	}

}
