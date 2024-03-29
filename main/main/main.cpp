// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <fstream>
#include <csignal>


using namespace std;

SYSTEMTIME st;
HANDLE eventP;
HANDLE eventP1;
HANDLE eventP2;

ofstream bufferOS;
ofstream logOS;


int main() {
	int num_amount = 0;
	eventP = CreateEvent(NULL, TRUE, TRUE, TEXT("eventP"));
	eventP1 = CreateEvent(NULL, FALSE, FALSE, TEXT("eventP1"));
	eventP2 = CreateEvent(NULL, FALSE, FALSE, TEXT("eventP2"));
	
	STARTUPINFO startUpInfo;
	ZeroMemory(&startUpInfo, sizeof(STARTUPINFO));
	PROCESS_INFORMATION processInformation[2];
	CreateProcess(TEXT("G:\\Study\\labs\\3 курс\\ОС\\labs\\ProcessSync\\main\\Debug\\p1.exe"), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startUpInfo, &processInformation[0]);
	CreateProcess(TEXT("G:\\Study\\labs\\3 курс\\ОС\\labs\\ProcessSync\\main\\Debug\\p2.exe"), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startUpInfo, &processInformation[1]);
	
	
	logOS.open("log/logProcessP.log", ios::out);
	while (num_amount != 10) {
		WaitForSingleObject(eventP, INFINITE);
		ResetEvent(eventP);

		Sleep(1);
		
		int num = rand() % 2000;
		num_amount++;
		bufferOS.open("buf/buf.txt", ios::out);
		bufferOS << num << endl;
		bufferOS.close();

		char b[100];
		GetLocalTime(&st);
		sprintf_s(b, "%02d:%02d:%02d.%03d",
			st.wHour,
			st.wMinute,
			st.wSecond,
			st.wMilliseconds);
		logOS << b << " INFO processP - write " << num << " to buffer.txt" << endl;
		SetEvent(eventP1);
	}

	if (bufferOS.is_open()) {
		bufferOS.close();
	}
	bufferOS.open("buf/buf.txt", ios::out);
	bufferOS << "-1" << endl;
	bufferOS.close();

	char b[100];
	GetLocalTime(&st);
	sprintf_s(b, "%02d:%02d:%02d.%03d",
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);

	logOS << b << " INFO processP - write -1 to buffer.txt" << endl;
	logOS << b << " INFO processP - closed" << endl;

	if (logOS.is_open()) {
		logOS.close();
	}

	SetEvent(eventP1);
	exit(0);
	
	return 0;
}
