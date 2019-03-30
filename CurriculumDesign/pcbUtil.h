#pragma once
#define MAX_NAME_LENGTH 20
#define MAX_PROCESS_PRIORITY 30
#include"list.h"
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

//定义一个返回值为空的函数指针 
//用于描述进程函数
typedef void(*ProcessFunction_t)(void*);

typedef struct PCB_ADDRESSofMEMORY {
	void* start;
	unsigned int length;

}PCB_ADDRESSofMEMORY;

typedef struct ProcessControllBlock {
	volatile PCB_ADDRESSofMEMORY stackAddress;
	//指向的父列表项
	ListItem *hostItem;
	//进程运行时间
	clock_t runTime;
	//进程名称
	char* PCBname[MAX_NAME_LENGTH];
	//进程优先级
	unsigned int processPriority;
	//进程状态
	PCB_STATUS status;
	//进程id
	unsigned int IDofPCB;
	//进程函数
	ProcessFunction_t function;
	//进程在堆栈中的位置
	int stackPosition;
}PCB;


typedef PCB PCB_t;
//正在处理的进程
PCB_t* volatile CurrentPCB_pointer;

//全局就绪列表
ProcessList* ProcessReadyList[MAX_PROCESS_PRIORITY];
//全局阻塞列表
ProcessList* ProcessBlockingList;
//全局删除列表
ProcessList* ProcessDeleteList;


//系统关键记录
//运行的进程数量
//!!!!!!一定要记得初始化!!!!!
volatile unsigned int CurrentProcessNumer;
//最高优先级的进程
volatile unsigned int TopPriorityReadyProcess;
//调度器的运状态
SCHEDULER_STATUS schdulerStatus;



