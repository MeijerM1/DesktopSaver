#pragma once
#include "stdafx.h"
#include <windows.h>
#include <vector>
#include "ProgramSnapshot.h"

class WindowsUtility {
public:
	static BOOL IsAltTabWindow(HWND hwnd);
	std::vector<ProgramSnapshot> getOpenPrograms();
};