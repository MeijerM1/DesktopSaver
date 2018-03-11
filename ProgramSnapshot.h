#pragma once
#include "stdafx.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

class ProgramSnapshot {
private:
	wstring executableName ;
	double top;
	double bottom;
	double left;
	double right;
public:
	ProgramSnapshot() {};
	ProgramSnapshot(wstring name , double top, double bottom, double left, double right);
	double getTop();
	double getBot();
	double getLeft();
	double getRight();
	wstring getName();
	bool start();
	
	template<class Archive> void serialize(Archive &ar, const unsigned int version) {
		ar & executableName;
		ar & top;
		ar & bottom;
		ar & left;
		ar & right;
	}
};
