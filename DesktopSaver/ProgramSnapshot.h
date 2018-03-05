#pragma once
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

class ProgramSnapshot {
private:
	string executableName ;
	double top;
	double bottom;
	double left;
	double right;
public:
	ProgramSnapshot(string name , double top, double bottom, double left, double right);
	double getTop();
	double getBot();
	double getLeft();
	double getRight();
	string getName();
};
