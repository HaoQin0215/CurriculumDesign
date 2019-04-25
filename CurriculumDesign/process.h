#include<stdint.h>

#include"pcbUtil.h"
#include"ticks.h"

#define GET_PCB_STATUS(pcb) (pcb->status)

//进程工作函数
DWORD WINAPI processThreadFun(LPVOID param);

int initStaticLists();

void freeStaticLists();

int CreateNewProcess(ProcessFunction_t function,
	const char* const name, const unsigned int stackLength,
	void* const parameters, unsigned int prority,
	PCB** pcb, time_t runTime);

void InitialNewProcess(ProcessFunction_t function, const char* const name, const unsigned int stackLength,
	void* const parameters, unsigned int prority, PCB*pcb, time_t runTime);

void addProcessToReadyList(PCB_t*newPcb);

int DeleteProcess(PCB* pcb);
int BlockedProcess();
int WakeupProcess();

void schedulerStopAll(void);
void schedulerResume(void);

void* myMalloc(size_t newSize);

void myFree(void*pointer);


void processSwitchContext();

void startScheduler();

void listChangeListItemWithTime(ProcessList*list);

void  FindTopProrityProcess();

void proSELECT_HIGHEST_PRIORITY_PROCESS();

//void changeProcessUnderInterruption(PCB_t*pcb);

void runInFreeTime(void*a);
