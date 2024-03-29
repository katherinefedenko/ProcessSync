#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <fstream>
#include <ctime>

using namespace std;


int main() {
	HANDLE eventP1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("eventP1"));
	HANDLE eventP2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("eventP2"));

	SYSTEMTIME st;
	ifstream bufferIS;
	ofstream logP1;
	logP1.open("log/logProcessP1.log", ios::out);

	while (true) {
		WaitForSingleObject(eventP1, INFINITE);
		ResetEvent(eventP1);

		bufferIS.open("buf/buf.txt");
		int num;
		bufferIS >> num;
		bufferIS.close();

		char b[100];
		GetLocalTime(&st);
		sprintf_s(b, "%02d:%02d:%02d.%03d",
			st.wHour,
			st.wMinute,
			st.wSecond,
			st.wMilliseconds);
		logP1 << b << " INFO processP1 - read " << num << " from buffer.txt" << endl;

		cout << num << endl;
		GetLocalTime(&st);
		sprintf_s(b, "%02d:%02d:%02d.%03d",
			st.wHour,
			st.wMinute,
			st.wSecond,
			st.wMilliseconds);
		logP1 << b << " INFO processP1 - write " << num << " to console" << endl;

		if (num == -1) {
			GetLocalTime(&st);
			sprintf_s(b, "%02d:%02d:%02d.%03d",
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds);
			logP1 << b << " INFO processP1 - closed";
			logP1.close();
			SetEvent(eventP2);
			break;
		}
		SetEvent(eventP2);
	}
	return 0;
}