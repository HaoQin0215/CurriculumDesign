
#include"pcbUtil.h"

int CreateNewProcess(ProcessFunction_t function,
	const char* const name, const unsigned int stackLength,
	void* const parameters, unsigned int prority,
	PCB* pcb);

void InitialNewProcess(ProcessFunction_t function, const char* const name, const unsigned int stackLength,
	void* const parameters, unsigned int prority, PCB* pcb);

void addProcessToReadyList(PCB_t*newPcb);

int DeleteProcess();
int BlockedProcess();
int WakeupProcess();

void schedulerStopAll(void);
void schedulerResume(void);

void* myMalloc(size_t newSize);

void myFree(void*pointer);
