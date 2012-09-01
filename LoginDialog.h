#pragma once

#include "BaseDialog.h"

//! Dialog wrapper for the Credentials dialog.
class LoginDialog : public BaseDialog
{
public:
	LoginDialog();
	~LoginDialog();

	void Init();
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
};