#include<Windows.h>
#include<signal.h>
#include<stdint.h>

#define tickTime 500
#define ENTER_CRITICAL() enter_list_critical() 
#define EXIT_CRITICAL() exit_list_critical()

//ʱ�Ӽ�����
long volatile tickCount;
//��������
typedef long BaseType_t;
//ʱ�ӽ�������
typedef uint32_t TickType_t;
//��ջ����
typedef unsigned int StackType_t;
//�����ж��ź���
HANDLE timeInterruptMutex;
//���Ľ����б�����ȫ�����ݽṹ���ź���
HANDLE modifyListMutex;
//ʱ�Ӽ������ź���
HANDLE tickCountMutex;
//�ж��ź���
HANDLE INTERRUPTION;

//ɱ���̵߳��ź���
HANDLE toKillProcessThread;

//�����������ź���
HANDLE schedulerMutex;
//��ʼ���ź���
void initSemphores();
//����ģ�⹤���߳�
HANDLE processThread;
//ʱ�ӹ����߳�
HANDLE timerThread;

//�������߳�ִ�к���
DWORD WINAPI startTimer(LPVOID param);
//���������Ƿ����
BOOL checkTickCountOverflow();
//����������
void CreateTimer();
//�����޸��б����ݽṹ�ٽ���
void enter_list_critical();
//�˳��޸����ݽṹ�ٽ���
void exit_list_critical();




