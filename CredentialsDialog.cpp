#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fstream>
#include <Windowsx.h>
#include "resource.h"
#include "CredentialsDialog.h"
#include "Runnable.h"
#include "Helpers.h"
#include "Data.h"

using namespace std;

// Used to pass data between dialog box and CredentialsDialog
Data dialogData;

// Procedure for the dialog box.
LRESULT CALLBACK CredentialsDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDGENERATE:
			// Read the old version.
			dialogData.ReadInformation(CREDENTIALS_FILE);

			// Store the input box values.
			dialogData.hostname = GetText(GetDlgItem(hWndDlg, IDC_HOSTNAME));
			dialogData.username = GetText(GetDlgItem(hWndDlg, IDC_USERNAME));
			dialogData.password = GetText(GetDlgItem(hWndDlg, IDC_PASSWORD));
			dialogData.executable = GetText(GetDlgItem(hWndDlg, IDC_EXECUTABLE));
			dialogData.directory = GetText(GetDlgItem(hWndDlg, IDC_DIRECTORY));
			dialogData.autoRun = Button_GetCheck(GetDlgItem(hWndDlg, IDC_AUTORUN));

			// Send message that will get catched by CredentialsDialog::MsgProc().
			SendMessage(gMainWindow->GetHwnd(), WM_COMMAND, ID_GENERATE_FILE, NULL);
			break;
		case IDC_BACK:
			// Tell MainWindow to change to the Patcher dialog.
			SendMessage(gMainWindow->GetHwnd(), WM_COMMAND, IDM_PATCHER_DIALOG, NULL);
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

CredentialsDialog::CredentialsDialog()
	: BaseDialog(IDD_DIALOG1, (DLGPROC)CredentialsDlgProc)
{
	
}
	
CredentialsDialog::~CredentialsDialog()
{

}

void CredentialsDialog::Init()
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
		Button_SetCheck(GetDlgItem(GetHwnd(), IDC_AUTORUN), data.autoRun);
	}
}

void CredentialsDialog::GenerateFile()
{
	// Ouput the data to the file.
	dialogData.WriteInformation(CREDENTIALS_FILE);
	dialogData.ReadInformation(CREDENTIALS_FILE);
}

LRESULT CredentialsDialog::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		// Generate the credentials file.
		case ID_GENERATE_FILE:
			GenerateFile();
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