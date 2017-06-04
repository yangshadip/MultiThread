#include <windows.h>
#include <process.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

using namespace std;

#define  THREAD_NUM 5

DWORD WINAPI ThreadFunc(LPVOID);
DWORD WINAPI MonitorFunc(LPVOID);

void ExitFunc(void);

int main()
{
	HANDLE hThread[THREAD_NUM];

	for (int i = 0; i < THREAD_NUM; ++i)
	{
		hThread[i] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)i, 0, NULL);
	}

	HANDLE monitorThread;
	monitorThread = CreateThread(NULL, 0, MonitorFunc, (LPVOID)hThread, 0, NULL);
	if (WaitForSingleObject(monitorThread, INFINITE))
		CloseHandle(monitorThread);


	for (int i = 0; i < THREAD_NUM; ++i)
	{
		if (WaitForSingleObject(hThread[i], INFINITE))
		{
			CloseHandle(hThread[i]);
		}
	}

	cout << "�����߳��Ѿ����н�����" << endl;

	system("pause");

	return EXIT_SUCCESS;

}

DWORD WINAPI ThreadFunc(LPVOID lpPara)
{
	cout << "�߳�" << (int)(lpPara) << "��������" << endl;
	Sleep(10);
	return 0;
}

DWORD WINAPI MonitorFunc(LPVOID hThread)
{
	DWORD exitCoed;
	int exitThreadCount = 0;
	int threadStatus[THREAD_NUM];
	memset(threadStatus, 0, THREAD_NUM);

	while (TRUE)
	{
		for (int i = 0; i < THREAD_NUM; ++i)
		{
			GetExitCodeThread(((HANDLE*)hThread)[i], &exitCoed);
			if (exitCoed == STILL_ACTIVE)
				cout << "����ExitCode�жϣ��߳�" << i << "��Ȼ�����У�" << endl;
			else
			{
				cout << "����ExitCode�жϣ��߳�" << i << "�Ѿ���������" << endl;
				threadStatus[i] = 1;
			}
		}

		exitThreadCount = 0;
		for (int i = 0; i < THREAD_NUM; ++i)
			exitThreadCount += threadStatus[i];

		if (exitThreadCount == THREAD_NUM)
			break;
	}

	return 0;

}