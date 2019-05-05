#pragma once
#define MAX_NAME_LENGTH 20
#define MAX_PROCESS_PRIORITY 35
#include"list.h"

#define FALSE ((long)0)
#define TRUE ((long)1)

#define listSELECT_HIGHEST_PRIORITY_TACK() {\
	listGET_OWNER_OF_NEXT_ENTRY(CurrentPCB_pointer, &ProcessReadyList[TopPriorityReadyProcess]);\
}\

typedef enum {
	SCHEDULER_RUNNING = 1,
	SCHEDULER_STOP = 0
}SCHEDULER_STATUS;
typedef enum {
	BLOCKING = 0,
	SUSPEND,
	RUNNING,
	READY,
	DELETED
}PCB_STATUS;

//����һ������ֵΪ�յĺ���ָ�� 
//�����������̺���
typedef void(*ProcessFunction_t)(void*);

typedef struct PCB_ADDRESSofMEMORY {
	void* start;
	unsigned int length;

}PCB_ADDRESSofMEMORY;

typedef struct ProcessControllBlock {
	volatile PCB_ADDRESSofMEMORY stackAddress;
	//ָ��ĸ��б���
	ListItem *hostItem;
	//��������ʱ��
	clock_t runTime;
	//��������
	char PCBname[MAX_NAME_LENGTH];
	//�������ȼ�
	unsigned int processPriority;
	//����״̬
	PCB_STATUS status;
	//����id
	unsigned int IDofPCB;
	//���̺���
	ProcessFunction_t function;
	//�����ڶ�ջ�е�λ��
	int stackPosition;
}PCB;
typedef PCB PCB_t;
//���̽����Ĵ洢��
typedef struct EXIT_PROCESS {
	PCB_t*pcb;
}EXIT_PROCESS;




//���ڴ���Ľ���
PCB_t* volatile CurrentPCB_pointer;

//ȫ�־����б�
ProcessList* ProcessReadyList[MAX_PROCESS_PRIORITY];
//ȫ�������б�
ProcessList* ProcessBlockingList;
//ȫ��ɾ���б�
ProcessList* ProcessDeleteList;
//��չ��������
ProcessList* ProcessPendingList;
//��ʱ�б�1
ProcessList* DelayedList1;
//��ʱ�б�2
ProcessList* DelayedList2;
//��ʱ�б�
ProcessList* volatile DelayedList;
//�����ʱ�б����������ʱ����������ʱ�б��������
ProcessList* volatile OverFlowDelayedList;


//ϵͳ�ؼ���¼
//���еĽ�������
//!!!!!!һ��Ҫ�ǵó�ʼ��!!!!!
int CurrentProcessNumer;
//������ȼ��Ľ���
unsigned int TopPriorityReadyProcess;
//����������״̬
 SCHEDULER_STATUS schdulerStatus;
//�Ƿ��ܹ����������л�
static long xYieldPending  = 0;
//�����˳����ź�
int exit_signal ;

EXIT_PROCESS **processExitBuf;




