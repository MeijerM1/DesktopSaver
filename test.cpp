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
#include "DesktopSnapshot.h"
#include "test.h"
#include "WindowsUtility.h"
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

int main(void)
{
	WindowsUtility util;
	std::vector<ProgramSnapshot> programs = util.getOpenPrograms();

	for (ProgramSnapshot s : programs) {
		std::wcout << s.getName() << std::endl;
	}

	std::cout << "Enter a name for your snapshot: ";
	string name;
	std::cin >> name;
	DesktopSnapshot ds(name, programs);	

	std::ofstream ofs("filename");

	{
		boost::archive::text_oarchive oa(ofs);
		oa << ds;
	}


	std::cout << "Snapshot " << ds.getName() << " is ready to be loaded" << std::endl;
	std::cout << "Do you want to restart your applications?(Y/N)" << std::endl;

	std::string result;
	std::cin >> result;

	if (result != "Y") {
		return 0;
	}

	std::cout << "Starting programs" << std::endl;
	ds.start();

	return 0;
}