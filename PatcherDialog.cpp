#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include "resource.h"
#include "Runnable.h"
#include "PatcherDialog.h"
#include "FtpHandler.h"

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
		case IDC_EDIT_CREDENTIALS:
			// Send a message that MainWindow catches and creates the credentials dialog.
			// Send the dialog Hwnd as wParam.
			SendMessage(gMainWindow->GetHwnd(), WM_COMMAND, (WPARAM)hWndDlg, IDM_EDIT_CREDENTIALS);
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
	: BaseDialog(IDD_DIALOG2, (DLGPROC)PatcherDlgProc)
{
	UpdateInformation();
}
	
PatcherDialog::~PatcherDialog()
{

}

void PatcherDialog::Init()
{
	
}

void PatcherDialog::UpdateInformation()
{
	// Set the information label values.
	int version = gFtpHandler->GetVersion();
	int size = gFtpHandler->GetTotalSize();
	int files = gFtpHandler->GetNumFiles();

	char versionText[25], sizeText[25], filesText[25];
	sprintf(versionText, "Version: %i", version);
	sprintf(sizeText, "Size: %i", size);
	sprintf(filesText, "Files: %i", files);

	Static_SetText(GetDlgItem(GetHwnd(), IDC_FTP_VERSION), versionText);
	Static_SetText(GetDlgItem(GetHwnd(), IDC_FTP_SIZE), sizeText);
	Static_SetText(GetDlgItem(GetHwnd(), IDC_FTP_FILES), filesText);

	// Local information.
}

LRESULT PatcherDialog::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}