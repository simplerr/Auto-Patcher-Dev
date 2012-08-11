#include "Runnable.h"
#include "BaseDialog.h"

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Don't start processing messages until the application has been created.
	if(gMainWindow != NULL )
		return gMainWindow->MsgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

Runnable::Runnable(HINSTANCE hInstance, std::string caption, int width, int height)
{
	mCaption		= caption;
	mhInstance		= hInstance;
	mhMainWindow	= NULL;
	mWidth			= width;
	mHeight			= height;

	// Init the window.
	InitWindow();
}

Runnable::~Runnable()
{

}

bool Runnable::InitWindow()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = mhInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, "RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	// Create the window with a custom size and make it centered
	// NOTE: WS_CLIPCHILDREN Makes the area under child windows not be displayed. (Useful when rendering DirectX and using windows controls).
	RECT R = {0, 0, mWidth, mHeight};
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	mhMainWindow = CreateWindow("D3DWndClassName", mCaption.c_str(), 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN, GetSystemMetrics(SM_CXSCREEN)/2-(mWidth/2),
		GetSystemMetrics(SM_CYSCREEN)/2-(mHeight/2), mWidth, mHeight, 
		0, 0, mhInstance, 0); 

	if(!mhMainWindow)
	{
		MessageBox(0, "CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	//ShowWindow(mhMainWindow, SW_HIDE);
	UpdateWindow(mhMainWindow);

	return true;
}

// The message loop.
int Runnable::Run()
{
	float time = 0.0f;
	MSG  msg;
    msg.message = WM_NULL;

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE ))
		{
			if(!IsDialogMessage(gMainWindow->GetCurrentDialog()->GetHwnd(), &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
    }
	return (int)msg.wParam;
}

// Handles all window messages.
LRESULT Runnable::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(GetHwnd(), msg, wParam, lParam);
}

void Runnable::SwitchScreenMode()
{

}

HINSTANCE Runnable::GetInstance()
{
	return mhInstance;
}
	
HWND Runnable::GetHwnd()
{
	return mhMainWindow;
}

void Runnable::SetVisible(bool visible)
{
	if(visible)
		ShowWindow(mhMainWindow, SW_SHOW);
	else
		ShowWindow(mhMainWindow, SW_HIDE);
}