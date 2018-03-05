#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include "ProgramSnapshot.h"
#include "test.h"

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

std::vector<ProgramSnapshot> snapshots;

BOOL CALLBACK getWindowDetails(HWND hwnd, LPARAM lParam) {


	// Get process Id.
	DWORD processId;
	GetWindowThreadProcessId(hwnd, &processId);

	// Get handle.
	HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processId);

	// Get  the ful path to the executable.
	TCHAR filename[MAX_PATH];
	if (processHandle != NULL) {
		GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH);
		CloseHandle(processHandle);
	}


	// Get window rect.
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);

	// Get window title text.
	const DWORD TITLE_SIZE = 1024;
	WCHAR windowTitle[TITLE_SIZE];
	GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

	int length = ::GetWindowTextLength(hwnd);
	std::wstring title(&windowTitle[0]);
	if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager") {
		return TRUE;
	}

	wstring wideFilename(&filename[0]); 
	string nameString(wideFilename.begin(), wideFilename.end());

	ProgramSnapshot ps(nameString, windowRect.top, windowRect.bottom, windowRect.left, windowRect.right);

	snapshots.push_back(ps);

	return TRUE;
}

int main(void)
{
	EnumWindows(getWindowDetails, NULL);

	for (ProgramSnapshot s : snapshots) {
		cout << s.getName() << std::endl;
	}

	return 0;
}