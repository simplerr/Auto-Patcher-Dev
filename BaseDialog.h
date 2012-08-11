#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//! Base class for all dialogs.
class BaseDialog
{
public:
	BaseDialog(UINT id, DLGPROC dlgProc);
	virtual ~BaseDialog();

	virtual void Init();
	virtual LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	HWND GetHwnd();
	void SetHwnd(HWND hwnd);
private:
	HWND mhHwnd;
};