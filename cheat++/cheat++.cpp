// cheatengine++.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h> // CreateToolHelp32Snapshot, Process32First, Process32Next
using namespace std;
DWORD findProcessIdByProcessName(LPCSTR processName)
{
	// Declare variables for enumerated processes
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	DWORD PiD = NULL;
	DWORD _err;

	//set all members of the process32 structure to zero
	ZeroMemory(&pe32, sizeof(PROCESSENTRY32));

	//set the size of the structure
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// take a snapshot of all processes in the operating system
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_err = GetLastError();
		printf("error: OpenProcess failed! | Code :%d (%x)\n", _err, _err);
		return NULL;
	}

	//Retrieve information about the first process,
	//and return zero if unnsucessfull

	if (Process32First(hProcessSnap, &pe32) == FALSE)
	{
		_err = GetLastError();
		printf("error: OpenProcess failed! | Code :%d (%x)\n", _err, _err);
		CloseHandle(hProcessSnap);
		return NULL;
	}

	do
	{
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
		if (hProcess != NULL)
		{
			if (strcmp(pe32.szExeFile, processName) == 0)
			{
				PiD = pe32.th32ProcessID;

				CloseHandle(hProcessSnap);

				CloseHandle(hProcess);

				return PiD;
			}
			CloseHandle(hProcess);
		}
	} while (Process32Next(hProcessSnap, &pe32) != FALSE);
	//Free snapshot handle
	CloseHandle(hProcessSnap);

	return PiD;
}

int main()
{
	DWORD PiD = 0;
	DWORD _err;

	HWND okno = FindWindow(0, "Step2");

	if (okno != NULL)
	{
		GetWindowThreadProcessId(okno, &PiD);
		printf("okno wykryte!");

		if (PiD != NULL)
		{
			HANDLE hProcess;

			hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, PiD);

			if (hProcess != NULL)
			{
				printf("Opened process Id %d (%x) with Read/Write permissions\n", PiD, PiD);
				CloseHandle(hProcess);
			}
			else
			{
				_err = GetLastError();
				printf("error: OpenProcess failed! | Code :%d (%x)\n", _err, _err);
			}
		}
	}
	else
	{
		printf("okno niewykryte!\n");
		printf("Szukanie okna przez process\n");
		PiD = findProcessIdByProcessName("Tutorial-i386.exe");
		printf("Opened process Id %d (%x)\n", PiD, PiD);

		if (PiD != NULL)
		{

			printf("Opened process Id %d (%x)\n", PiD, PiD);
		}
		else {
			printf("Error: Unable to find process!\n");
		}

	}

	system("pause");
}