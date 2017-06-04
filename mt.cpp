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

	cout << "所有线程已经运行结束！" << endl;

	system("pause");

	return EXIT_SUCCESS;

}

DWORD WINAPI ThreadFunc(LPVOID lpPara)
{
	cout << "线程" << (int)(lpPara) << "正在运行" << endl;
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
				cout << "根据ExitCode判断：线程" << i << "仍然在运行！" << endl;
			else
			{
				cout << "根据ExitCode判断：线程" << i << "已经结束运行" << endl;
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