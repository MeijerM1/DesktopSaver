#include "stdafx.h"
#include "ProgramSnapshot.h"

ProgramSnapshot::ProgramSnapshot(string name, double top, double bottom, double left, double right)
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

string ProgramSnapshot::getName() {
	return executableName;
}