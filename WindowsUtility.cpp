#include "stdafx.h"
#include "WindowsUtility.h"
#include "ProgramSnapshot.h"
#include <psapi.h>

// Check if a window is a taskbar program.
// Used to make sure we're not saving overlay applications like screen recorders etc.
// Source: http://www.dfcd.net/projects/switcher/switcher.c
BOOL WindowsUtility::IsAltTabWindow(HWND hwnd) {
	TITLEBARINFO ti;
	HWND hwndTry, hwndWalk = NULL;

	if (!IsWindowVisible(hwnd))
		return FALSE;

	hwndTry = GetAncestor(hwnd, GA_ROOTOWNER);
	while (hwndTry != hwndWalk)
	{
		hwndWalk = hwndTry;
		hwndTry = GetLastActivePopup(hwndWalk);
		if (IsWindowVisible(hwndTry))
			break;
	}
	if (hwndWalk != hwnd)
		return FALSE;

	// the following removes some task tray programs and "Program Manager"
	ti.cbSize = sizeof(ti);
	GetTitleBarInfo(hwnd, &ti);
	if (ti.rgstate[0] & STATE_SYSTEM_INVISIBLE)
		return FALSE;

	// Tool windows should not be displayed either, these do not appear in the
	// task bar.
	if (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
		return FALSE;

	return TRUE;
}

BOOL CALLBACK getWindowDetails(HWND hwnd, LPARAM lParam) {

	if (!WindowsUtility::IsAltTabWindow(hwnd)) {
		return TRUE;
	}

	std::vector<ProgramSnapshot>& programs =
		*reinterpret_cast<std::vector<ProgramSnapshot>*>(lParam);

	// Get process id.
	DWORD processId;
	GetWindowThreadProcessId(hwnd, &processId);

	// Get handle.
	HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processId);

	// Get  the full path to the executable.
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

	std::wstring wideFilename(&filename[0]);

	ProgramSnapshot ps(wideFilename, windowRect.top, windowRect.bottom, windowRect.left, windowRect.right);

	programs.push_back(ps);

	return TRUE;
}

std::vector<ProgramSnapshot> WindowsUtility::getOpenPrograms() {
	std::vector<ProgramSnapshot> programs;

	EnumWindows(getWindowDetails, reinterpret_cast<LPARAM>(&programs));

	return programs;
}