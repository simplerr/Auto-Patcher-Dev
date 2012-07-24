#pragma once
#include "State.h"

class PatcherDialog : public State
{
public:
	PatcherDialog();
	~PatcherDialog();

	void Init();

	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
};

