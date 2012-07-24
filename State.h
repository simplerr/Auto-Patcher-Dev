#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class State
{
public:
	State() {};
	virtual ~State() {};

	virtual void Init() {};
	virtual LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam) {return 0;};
private:
};