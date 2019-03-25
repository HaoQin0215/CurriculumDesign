#include "list.h"

#define MAX_NAME_LENGTH 20

typedef enum {
	SCHEDULER_RUNNING = 1,
	SCHEDULER_STOP = 0
}SCHEDULER_STATUS;
typedef enum { 
	BLOCKING=0,
	SUSPEND,
	RUNNING, 
	READY ,
	DELETE
}PCB_STATUS;

//����һ������ֵΪ�յĺ���ָ��
typedef void(*ProcessFunction_t)(void*);

typedef struct PCB_ADDRESSofMEMORY{
	void* start;
	unsigned int length;

}PCB_ADDRESSofMEMORY;

typedef struct ProcessControllBlock {
	volatile PCB_ADDRESSofMEMORY stackAddress;
	//ָ��ĸ��б���
	ListItem hostItem;
	//��������ʱ��
	clock_t runTime;
	//��������
	char* PCBname[MAX_NAME_LENGTH];
	//�������ȼ�
	unsigned int processPriority;
}PCB;

typedef PCB PCB_t;
//���ڴ���Ľ���
PCB_t* volatile CurrentPCB_pointer;

//ϵͳ�ؼ���¼
typedef struct OSrecording {
	//���еĽ�������
	volatile unsigned int CurrentProcessNumer;
	//������ȼ��Ľ���
	volatile unsigned int TopPriorityProcess;
	//����������״̬
	SCHEDULER_STATUS schdulerStatus;
}OSrecording;



