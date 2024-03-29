#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <fstream>
#include <ctime>
using namespace std;

int main() {
	HANDLE eventP = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("eventP"));
	HANDLE eventP2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("eventP2"));

	SYSTEMTIME st;
	ifstream bufferIS;
	ofstream p2OS;
	p2OS.open("buf/bufProcessP2.txt", ios::out);
	ofstream logP2;
	logP2.open("log/logProcessP2.log", ios::out);

	while (true) {
		WaitForSingleObject(eventP2, INFINITE);
		ResetEvent(eventP2);

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
		logP2 << b << " INFO processP2 - read " << num << " from buffer.txt" << endl;

		p2OS << num << endl;
		GetLocalTime(&st);
		sprintf_s(b, "%02d:%02d:%02d.%03d",
			st.wHour,
			st.wMinute,
			st.wSecond,
			st.wMilliseconds);
		logP2 << b << " INFO processP2 - write " << num << " to p2.txt" << endl;

		if (num == -1) {
			GetLocalTime(&st);
			sprintf_s(b, "%02d:%02d:%02d.%03d",
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds);
			logP2 << b << " INFO processP2 - closed";
			logP2.close();
			p2OS.close();
			SetEvent(eventP2);
			break;
		}
		SetEvent(eventP);
	}
	return 0;
}