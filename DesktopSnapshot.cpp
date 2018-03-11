#include "stdafx.h"
#include "DesktopSnapshot.h"
#include <algorithm>

DesktopSnapshot::DesktopSnapshot(string name, vector<ProgramSnapshot> programs) {
	this->name = name;
	this->programs = programs;
}

void DesktopSnapshot::addProgram(ProgramSnapshot ps) {
	programs.push_back(ps);
}

void DesktopSnapshot::removeProgram(wstring name) {
	programs.erase(std::remove_if(programs.begin(), programs.end(), [&](ProgramSnapshot item) { return item.getName() == name; }), programs.end());
}

vector<ProgramSnapshot> DesktopSnapshot::getPrograms() {
	return programs;
}

string DesktopSnapshot::getName() {
	return name;
}

void DesktopSnapshot::start() {
	for (auto s : programs) {
		std::wcout << "Starting: " << s.getName() << std::endl;
		s.start();
	}
}