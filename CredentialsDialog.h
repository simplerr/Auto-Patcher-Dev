#pragma once

#include "State.h"

class CredentialsDialog : public State
{
public:
	CredentialsDialog();
	~CredentialsDialog();

	void Init();
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void GenerateFile();
private:
};