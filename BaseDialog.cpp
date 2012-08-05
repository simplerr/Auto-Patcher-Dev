#include "BaseDialog.h"
#include "MainWindow.h"
#include "Runnable.h"

BaseDialog::BaseDialog(UINT id, DLGPROC dlgProc) 
{
	HWND hwnd = CreateDialog(gMainWindow->GetInstance(), MAKEINTRESOURCE(id), gMainWindow->GetHwnd(), dlgProc);
	ShowWindow(hwnd, true);
	SetHwnd(hwnd);
}
	
BaseDialog::~BaseDialog() 
{
	EndDialog(GetHwnd(), 0);
}

void BaseDialog::Init() 
{

}

LRESULT BaseDialog::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam) 
{
	return 0;
}

HWND BaseDialog::GetHwnd()
{
	return mhHwnd;
}
	
void BaseDialog::SetHwnd(HWND hwnd)
{
	mhHwnd = hwnd;
}