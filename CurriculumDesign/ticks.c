#include"ticks.h"
#include<stdio.h>

void initSemphores()
{
	timeInterruptMutex = CreateMutex(NULL, TRUE, NULL);

	modifyListMutex = CreateMutex(NULL, TRUE, NULL);

	tickCountMutex = CreateMutex(NULL, TRUE, NULL);

	INTERRUPTION = CreateSemaphore(NULL,0,1,NULL);

	schedulerMutex= CreateMutex(NULL, TRUE, NULL);
}


DWORD WINAPI startTimer(LPVOID param) {
	tickCount = 0;

	initSemphores();

	while (1) {
		

		Sleep(1000);
		printf("\nʱ����:%d\n",tickCount);
		checkTickCountOverflow();
		WaitForSingleObject(tickCountMutex, INFINITE);
		//ʱ�Ӽ�һ
		tickCount++;

		ReleaseMutex(tickCountMutex);
		//����޸�ʱ���ж��źŵ��ź����������ڽ��̽����ٽ���������¹��ж�
		WaitForSingleObject(timeInterruptMutex,INFINITE);

		ReleaseSemaphore(INTERRUPTION, 1, 0);

		ReleaseMutex(timeInterruptMutex);

		

	}

}
//�ж�ʱ�Ӽ����Ƿ����
BOOL checkTickCountOverflow() {

	BaseType_t currentTickCount = 0;

	BOOL result;
	//��÷���ʱ�Ӽ������ź���
	WaitForSingleObject(tickCountMutex,INFINITE);

		currentTickCount = tickCount;
		//����������
		if ((currentTickCount++) < currentTickCount) {

			result = FALSE;

			tickCount = 0;
		}else{
			result = TRUE;
		}

	ReleaseMutex(tickCountMutex);

		return result;

}

void CreateTimer() {

	timerThread = CreateThread(NULL, 0, startTimer, NULL, 0, NULL);
	
}

void enter_list_critical()
{
	//ͨ�����ʱ���źŻ������ﵽ���жϵ�Ŀ��
	WaitForSingleObject(timeInterruptMutex,INFINITE);
	//�޸��б�Ļ�����
	WaitForSingleObject(modifyListMutex, INFINITE);


}

void exit_list_critical()
{
	ReleaseMutex(modifyListMutex);

	ReleaseMutex(timeInterruptMutex);
}
