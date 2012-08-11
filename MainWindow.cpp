// Use Windows 7 looking controls.
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windowsx.h>
#include <Richedit.h>
#include <Commctrl.h>
#include "resource.h"
#include "MainWindow.h"
#include "CredentialsDialog.h"
#include "PatcherDialog.h"
#include "Helpers.h"
#include "FtpHandler.h"

// Set the globals.
Runnable*	gMainWindow		= 0;
FtpHandler* gFtpHandler		= 0;

using namespace std;

//! The program starts here.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	InitCommonControls();
	LoadLibrary("riched32.dll");

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	MainWindow window(hInstance, "Generic Launcher", 640, 480);
	gMainWindow = &window;

	window.Init();

	return gMainWindow->Run();
}

MainWindow::MainWindow(HINSTANCE hInstance, string caption, int width, int height)
	: Runnable(hInstance, caption, width, height)
{
	gFtpHandler = new FtpHandler();
	mCurrentDialog = NULL;
}
	
void MainWindow::Init()
{
	WIN32_FIND_DATA data;
	// Create and init the state.
	if(FindFirstFile(CREDENTIALS_FILE, &data) == INVALID_HANDLE_VALUE  && GetLastError() == ERROR_FILE_NOT_FOUND) 
		mCurrentDialog = new CredentialsDialog();
	else 
		mCurrentDialog = new PatcherDialog();
}

MainWindow::~MainWindow()
{
	delete gFtpHandler;
	delete mCurrentDialog;
}

LRESULT MainWindow::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(wParam == ID_GENERATE_FILE)
		int asda = 1;
	else if(wParam == IDM_PATCHER_DIALOG) {
		delete mCurrentDialog;
		mCurrentDialog = new PatcherDialog();
	}

	// Message sent when the Patcher dialogs "edit credentials button is pressed".
	if(lParam == IDM_EDIT_CREDENTIALS) {
		delete mCurrentDialog;
		mCurrentDialog = new CredentialsDialog();
		mCurrentDialog->Init();
	}

	// Let the current state handle the message.
	if(mCurrentDialog != NULL)
		mCurrentDialog->MsgProc(msg, wParam, lParam);

	// Default MsgProc.
	return Runnable::MsgProc(msg, wParam, lParam);
}