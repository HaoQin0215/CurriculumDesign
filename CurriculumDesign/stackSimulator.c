#include "stackSimulator.h"

int initOSstackSimulator()
{  
	STATIC_OS_STACK =(OSstack*)malloc(sizeof(OSstack));

	OSstackSimulatorItem* newItem = (OSstackSimulatorItem*)malloc(sizeof(OSstackSimulatorItem));

	OSstackSimulatorItem* newItem2 = (OSstackSimulatorItem*)malloc(sizeof(OSstackSimulatorItem));

	//OSstack* mystack = STATIC_OS_STACK;

	if (STATIC_OS_STACK == NULL||newItem==NULL||newItem2==NULL ) return 0;

	else {
		STATIC_OS_STACK->currentDeepth = 0;

		STATIC_OS_STACK->totalLength = MAX_STACK_LENGTH;

		STATIC_OS_STACK->startSimulatorItem = newItem;

		STATIC_OS_STACK->lastItem = newItem2;

		STATIC_OS_STACK->startSimulatorItem->pcb = NULL;

		STATIC_OS_STACK->lastItem->pcb = NULL;

		STATIC_OS_STACK->startSimulatorItem->next = STATIC_OS_STACK->startSimulatorItem;

		STATIC_OS_STACK->lastItem->next = STATIC_OS_STACK->lastItem;

		STATIC_OS_STACK->startSimulatorItem->index = 0;

		return 1;
	}
}

int addPcbToStack(PCB_t * newPcb)
{
	if (STATIC_OS_STACK->currentDeepth == MAX_STACK_LENGTH) return 0;

	OSstackSimulatorItem_t *item = (OSstackSimulatorItem*)malloc(sizeof(OSstackSimulatorItem));

	if (item == NULL)return 0;

	item->pcb = newPcb;

	item->functionValue = newPcb->function;

	item->next = item;

	OSstackSimulatorItem_t*interator;

	for (interator = STATIC_OS_STACK->startSimulatorItem; ; interator = interator->next)
	{
		//interator->next == interator
		if (interator == interator) {
			break;
		}
	}
	interator->next = item;
	
	item->index = interator->index + 1;

	newPcb->stackPosition = item->index;

	newPcb->stackAddress.start = item->index;

	STATIC_OS_STACK->currentDeepth+=1;

	//printf("%d\n", STATIC_OS_STACK->currentDeepth);

	return 1;
}

int deletePcbFromStack(int idOfPcb)
{
	int result;
	OSstackSimulatorItem* iterator;
	iterator = STATIC_OS_STACK->startSimulatorItem;
	if (STATIC_OS_STACK->currentDeepth == 0) {
		result = 0;
	}
	else {
		PCB_t*pcb=findPCB_ById(idOfPcb);
		if (pcb == NULL) {
			result = 0;
		}
		else {
			for (;; iterator = iterator->next) {
				if (iterator->next->index == idOfPcb) {
					iterator->next = iterator->next->next;
					free(iterator->next->pcb);
					free(iterator->next);
					
				}
			}
			result = 1;
		}
	}
	return result;
}

PCB_t* findPCB_ById(int id)
{
	OSstackSimulatorItem_t*iterator;
	for (iterator = STATIC_OS_STACK->startSimulatorItem;; iterator = iterator->next) {
		if (iterator->index = id) {
			return iterator->pcb;
		}
	}
	return NULL;
}


