#pragma once

#include "BaseDialog.h"

class CredentialsDialog : public BaseDialog
{
public:
	CredentialsDialog();
	~CredentialsDialog();

	void Init();
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void GenerateFile();
private:
};