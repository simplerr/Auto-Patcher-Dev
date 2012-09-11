#pragma once

#include "BaseDialog.h"

//! Dialog wrapper for the Credentials dialog.
class CredentialsDialog : public BaseDialog
{
public:
	CredentialsDialog();
	~CredentialsDialog();

	void Init();
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void GenerateFile();
	void HideBack();
private:
};