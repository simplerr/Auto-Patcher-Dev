#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "resource.h"
#include "Runnable.h"
#include "PatcherDialog.h"

// Procedure for the dialog box.
LRESULT CALLBACK PatcherDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			
			break;
		case IDEXIT:
			EndDialog(hWndDlg, 0);
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWndDlg, 0);
		PostQuitMessage(0);
		break;
	}

	return 0;
}

PatcherDialog::PatcherDialog()
{

}
	
PatcherDialog::~PatcherDialog()
{

}

void PatcherDialog::Init()
{
	DialogBox(gMainWindow->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG2), gMainWindow->GetHwnd(), (DLGPROC)PatcherDlgProc);
}

LRESULT PatcherDialog::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}