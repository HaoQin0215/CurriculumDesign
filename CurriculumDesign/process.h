
#include"pcbUtil.h"

#define GET_PCB_STATUS(pcb) (pcb->status)


	int initStaticLists();

	void freeStaticLists();

	int CreateNewProcess(ProcessFunction_t function,
		const char* const name, const unsigned int stackLength,
		void* const parameters, unsigned int prority,
		PCB** pcb);

	void InitialNewProcess(ProcessFunction_t function, const char* const name, const unsigned int stackLength,
		void* const parameters, unsigned int prority, PCB*pcb);

	void addProcessToReadyList(PCB_t*newPcb);

	int DeleteProcess(PCB* pcb);
	int BlockedProcess();
	int WakeupProcess();

	void schedulerStopAll(void);
	void schedulerResume(void);

	void* myMalloc(size_t newSize);

	void myFree(void*pointer);