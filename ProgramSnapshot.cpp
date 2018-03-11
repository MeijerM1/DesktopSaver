#include "stdafx.h"
#include "ProgramSnapshot.h"
#include "WindowsUtility.h"

BOOL CALLBACK moveWindow(HWND hwnd, LPARAM LParam)
{
	if (!WindowsUtility::IsAltTabWindow(hwnd)) {
		return TRUE;
	}

	std::vector<double>& params =
		*reinterpret_cast<std::vector<double>*>(LParam);

	// Get process id.
	DWORD processId;
	GetWindowThreadProcessId(hwnd, &processId);

	// Check if the process is the process we just created..
	if (processId == (DWORD)params[0]) {

		int x = params[2];
		int y = params[1];

		int width = params[3] - params[2];
		int height = params[4] - params[1];

		MoveWindow(hwnd, x, y, width, height, FALSE);
		return TRUE;
	}

	return TRUE;
}

ProgramSnapshot::ProgramSnapshot(wstring name, double top, double bottom, double left, double right)
{
	executableName = name;
	this->top = top;
	this->bottom = bottom;
	this->left = left;
	this->right = right;
}

double ProgramSnapshot::getTop() {
	return top;
}

double ProgramSnapshot::getBot() {
	return bottom;
}

double ProgramSnapshot::getLeft() {
	return left;
}

double ProgramSnapshot::getRight() {
	return right;
}

wstring ProgramSnapshot::getName() {
	return executableName;
}

bool ProgramSnapshot::start() {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// Give the stored position as start-up parameter.
	// Won't work in some cases because Microsoft doesn't know how to make working software.
	si.dwX = left;
	si.dwY = right;

	si.dwXSize = (right - left);
	si.dwYSize = (bottom - top);

	si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the process. 
	if (!CreateProcess(executableName.c_str(),
		NULL,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi)
		)
	{

		printf("CreateProcess failed (%d).\n", GetLastError());
		return false;
	}

	std::vector<double> params;

	params.push_back(pi.dwProcessId);
	params.push_back(top);
	params.push_back(left);
	params.push_back(right);
	params.push_back(bottom);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	// Sleep to wait for program window to appear.
	// TODO find more elegant solution.
	Sleep(3000);

	// Get the newly created window and move it.
	EnumWindows(moveWindow, reinterpret_cast<LPARAM>(&params));

	return true;
}