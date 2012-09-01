#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fstream>
#include <Windowsx.h>
#include "resource.h"
#include "LoginDialog.h"
#include "Runnable.h"
#include "Helpers.h"
#include "Data.h"

using namespace std;

// Procedure for the dialog box.
LRESULT CALLBACK LoginDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDLOGIN:
			{
			char buffer[256];
			Edit_GetText(GetDlgItem(hWndDlg, IDC_PASSWORD), buffer, 255);
			Data data("data.sef");

			// Compare the password.
			if(data.password == string(buffer)) {
				// Change to patcher dialog.
				SendMessage(gMainWindow->GetHwnd(), WM_COMMAND, IDM_PATCHER_DIALOG, NULL);
			}
			else {
				MessageBox(0, "Invalid password, try again!\n", 0, MB_ICONERROR);
				Static_SetText(GetDlgItem(hWndDlg, IDC_PASSWORD), "");
			}
			
			break;
			}
		case IDCANCEL:
			// Tell MainWindow to change to the Patcher dialog.
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

LoginDialog::LoginDialog()
	: BaseDialog(IDD_DIALOG4, (DLGPROC)LoginDlgProc)
{
	// Set focus to the password field.
	SetFocus(GetDlgItem(GetHwnd(), IDC_PASSWORD));
}
	
LoginDialog::~LoginDialog()
{

}

void LoginDialog::Init()
{
	// Check if there already exists a credentials file.
	WIN32_FIND_DATA data;
	if(FindFirstFile(CREDENTIALS_FILE, &data) != INVALID_HANDLE_VALUE  && GetLastError() != ERROR_FILE_NOT_FOUND) 
	{
		// Decrypt the information from the file.
		Data data(CREDENTIALS_FILE);

		// Set the edit box values.
		Edit_SetText(GetDlgItem(GetHwnd(), IDC_HOSTNAME), data.hostname.c_str());
		Edit_SetText(GetDlgItem(GetHwnd(), IDC_USERNAME), data.username.c_str());
		Edit_SetText(GetDlgItem(GetHwnd(), IDC_PASSWORD), data.password.c_str());
		Edit_SetText(GetDlgItem(GetHwnd(), IDC_EXECUTABLE), data.executable.c_str());
		Edit_SetText(GetDlgItem(GetHwnd(), IDC_DIRECTORY), data.directory.c_str());
	}
}

LRESULT LoginDialog::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		// Generate the credentials file.
		case ID_GENERATE_FILE:
			MessageBox(0, "Credentials successfully updated!", "Success", 0);

			// Tell MainWindow to change to the Patcher dialog.
			SendMessage(gMainWindow->GetHwnd(), WM_COMMAND, IDM_PATCHER_DIALOG, NULL);
			break;
		case IDC_UPDATE_FILES:

			break;
		}
		break;
	}

	return 0;
}