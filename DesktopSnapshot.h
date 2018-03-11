#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include "ProgramSnapshot.h"

using namespace std;

class DesktopSnapshot {
private:
	string name;
	vector<ProgramSnapshot> programs;
public:
	DesktopSnapshot() {}
	DesktopSnapshot(string name, vector <ProgramSnapshot> programs);
	void addProgram(ProgramSnapshot ps);
	void removeProgram(wstring name);
	vector<ProgramSnapshot> getPrograms();
	string getName();
	void start();

	template<class Archive> void serialize(Archive &ar, const unsigned int version) {
		ar & name;
		ar & programs;
	}
};
