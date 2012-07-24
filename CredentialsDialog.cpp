#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fstream>
#include "resource.h"
#include "CredentialsDialog.h"
#include "Runnable.h"
#include "Helpers.h"

using namespace std;

// Used to pass data between dialog box and CredentialsDialog
string hostname;
string username;
string password;
string filename;

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
			filename = GetText(GetDlgItem(hWndDlg, IDC_FILENAME));

			// Send message that will get catched by CredentialsDialog::MsgProc().
			SendMessage(gMainWindow->GetHwnd(), WM_COMMAND, ID_GENERATE_FILE, NULL);
			EndDialog(hWndDlg, 0);
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
{
	
}
	
CredentialsDialog::~CredentialsDialog()
{

}

void CredentialsDialog::Init()
{
	DialogBox(gMainWindow->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG1), gMainWindow->GetHwnd(), (DLGPROC)CredentialsDlgProc);
}

void CredentialsDialog::GenerateFile()
{
	// Ouput the login information to a file.
	ofstream fout("output.txt");
	fout << hostname << "\n" << username << "\n" << password << "\n" << filename;
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
			break;
		}
		break;
	}

	return 0;
}