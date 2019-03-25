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

//定义一个返回值为空的函数指针
typedef void(*ProcessFunction_t)(void*);

typedef struct PCB_ADDRESSofMEMORY{
	void* start;
	unsigned int length;

}PCB_ADDRESSofMEMORY;

typedef struct ProcessControllBlock {
	volatile PCB_ADDRESSofMEMORY stackAddress;
	//指向的父列表项
	ListItem hostItem;
	//进程运行时间
	clock_t runTime;
	//进程名称
	char* PCBname[MAX_NAME_LENGTH];
	//进程优先级
	unsigned int processPriority;
}PCB;

typedef PCB PCB_t;
//正在处理的进程
PCB_t* volatile CurrentPCB_pointer;

//系统关键记录
typedef struct OSrecording {
	//运行的进程数量
	volatile unsigned int CurrentProcessNumer;
	//最高优先级的进程
	volatile unsigned int TopPriorityProcess;
	//调度器的运状态
	SCHEDULER_STATUS schdulerStatus;
}OSrecording;



