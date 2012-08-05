#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fstream>
#include "resource.h"
#include "CredentialsDialog.h"
#include "Runnable.h"
#include "Helpers.h"
#include <Windowsx.h>

using namespace std;

// Used to pass data between dialog box and CredentialsDialog
string hostname;
string username;
string password;
string executable;
string directory;

// Procedure for the dialog box.
LRESULT CALLBACK CredentialsDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDGENERATE:
			// Store the input box values.
			hostname = GetText(GetDlgItem(hWndDlg, IDC_HOSTNAME));
			username = GetText(GetDlgItem(hWndDlg, IDC_USERNAME));
			password = GetText(GetDlgItem(hWndDlg, IDC_PASSWORD));
			executable = GetText(GetDlgItem(hWndDlg, IDC_EXECUTABLE));
			directory = GetText(GetDlgItem(hWndDlg, IDC_DIRECTORY));

			// Send message that will get catched by CredentialsDialog::MsgProc().
			SendMessage(gMainWindow->GetHwnd(), WM_COMMAND, ID_GENERATE_FILE, NULL);
			break;
		case IDEXIT:
			EndDialog(hWndDlg, 0);
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWndDlg, 0);
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
	if(FindFirstFile("credentials.txt", &data) != INVALID_HANDLE_VALUE  && GetLastError() != ERROR_FILE_NOT_FOUND) 
	{
		// Decrypt the information from the file.
		string hostname, username, password, executable, directory;
		Decrypt("credentials.txt", hostname, username, password, executable, directory);

		// Set the edit box values.
		Edit_SetText(GetDlgItem(GetHwnd(), IDC_HOSTNAME), hostname.c_str());
		Edit_SetText(GetDlgItem(GetHwnd(), IDC_USERNAME), username.c_str());
		Edit_SetText(GetDlgItem(GetHwnd(), IDC_PASSWORD), password.c_str());
		Edit_SetText(GetDlgItem(GetHwnd(), IDC_EXECUTABLE), executable.c_str());
		Edit_SetText(GetDlgItem(GetHwnd(), IDC_DIRECTORY), directory.c_str());
	}
}

void CredentialsDialog::GenerateFile()
{
	// [TODO] Let Encrypt() handle this!
	// Ouput the login information to a file.
	ofstream fout("credentials.txt");
	fout << hostname << "\n" << username << "\n" << password << "\n" << executable << "\n" << directory;
	fout.close();
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
			SendMessage(gMainWindow->GetHwnd(), WM_COMMAND, IDM_PATCHER_DIALOG, NULL);
			break;
		}
		break;
	}

	return 0;
}