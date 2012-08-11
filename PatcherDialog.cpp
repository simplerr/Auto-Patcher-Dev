#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include <fstream>
#include "resource.h"
#include "Runnable.h"
#include "PatcherDialog.h"
#include "FtpHandler.h"
#include "Helpers.h"
#include "Data.h"
#include "zip.h"
#include "unzip.h"

#include <shlobj.h>		//for Shell API
#include <Shlwapi.h>	//for Shell API

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
		case IDC_EXIT:
			EndDialog(hWndDlg, 0);
			PostQuitMessage(0);
			break;
		case IDC_EDIT_CREDENTIALS:
			// Send a message that MainWindow catches and creates the credentials dialog.
			// Send the dialog Hwnd as wParam.
			SendMessage(gMainWindow->GetHwnd(), WM_COMMAND, (WPARAM)hWndDlg, IDM_EDIT_CREDENTIALS);
			break;
		case IDC_UPDATE_FILES:
			SendMessage(gMainWindow->GetHwnd(), WM_COMMAND, (WPARAM)hWndDlg, IDM_UPDATE_FILES);
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
	mObserver = new ProgressObserver();

	UpdateInformation();

	mObserver = new ProgressObserver();
	mObserver->SetDialog(this);
	gFtpHandler->SetObserver(mObserver);

	Static_SetText(GetDlgItem(GetHwnd(), IDC_UPLOADING), "Upload progress:");

	AddText("Connecting...\n");
	if(gFtpHandler->IsConnected())
		AddText("Connected!\n");
	else
		AddText("Error connecting!\n");

	AddText("Waiting...\n");
}
	
PatcherDialog::~PatcherDialog()
{
	delete mObserver;
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
	sprintf(sizeText, "Size: %.2f MB", size/1000000.0f);
	sprintf(filesText, "Files: %i", files);
	string modifyDate = "Updated: " + gFtpHandler->GetModifyDate();

	Static_SetText(GetDlgItem(GetHwnd(), IDC_FTP_VERSION), versionText);
	Static_SetText(GetDlgItem(GetHwnd(), IDC_FTP_SIZE), sizeText);
	Static_SetText(GetDlgItem(GetHwnd(), IDC_FTP_FILES), filesText);
	Static_SetText(GetDlgItem(GetHwnd(), IDC_UPDATED_DATE), modifyDate.c_str());

	// Local information.
}

void PatcherDialog::UploadPatch()
{
	// Remove the data.zip if there is any.
	remove("data.zip");

	// Create the new archive and upload it.
	ArchiveInfo info;
	CreateArchive("app", "data.zip", info);
	mObserver->SetStatus("Creating archive...");
	AddText("Archiving...\n");
	Data data(CREDENTIALS_FILE);
	AddText("Uploading...\n");
	gFtpHandler->UploadFile(data.directory, "data.zip");
	remove("data.zip");

	// Create the version information file.
	data.version = gFtpHandler->GetVersion() + 1;
	data.modifyDate = GetDate();
	data.files = info.files;
	data.size = info.size;
	data.WriteInformation(CREDENTIALS_FILE);

	// Upload the info file.
	gFtpHandler->UploadFile(data.directory, CREDENTIALS_FILE);

	mObserver->SetStatus("Latest version uploaded!");
	AddText("Updated!\n-\n");

	UpdateInformation();
}

LRESULT PatcherDialog::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Upload all files to the FTP server.
	if(lParam == IDM_UPDATE_FILES)
		UploadPatch();

	return 0;
}

void PatcherDialog::AddBytesSent(long bytes)
{
	long pos = SendDlgItemMessage(GetHwnd(), IDC_PROGRESS1, PBM_GETPOS, 0, 0);
	SendDlgItemMessage(GetHwnd(), IDC_PROGRESS1, PBM_SETPOS, pos + bytes, 0);
}
	
void PatcherDialog::AddBytesReceived(long bytes)
{

}

void PatcherDialog::SetFileSize(long size)
{
	SendDlgItemMessage(GetHwnd(), IDC_PROGRESS1, PBM_SETRANGE32 , 0, size);
	SendDlgItemMessage(GetHwnd(), IDC_PROGRESS1, PBM_SETPOS, 0, 0);
}

void PatcherDialog::SetProgressStatus(string status)
{
	Static_SetText(GetDlgItem(GetHwnd(), IDC_UPLOADING), status.c_str());
}

void PatcherDialog::AddText(string text, COLORREF color)
{
	AddEditText(GetDlgItem(GetHwnd(), IDC_ACTION_LOG), text, color);
	Sleep(250);
}