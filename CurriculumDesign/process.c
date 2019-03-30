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