#pragma once
#include "Runnable.h"
#include "BaseDialog.h"

class FtpPatcher;

class MainWindow : public Runnable
{
public:
	MainWindow(HINSTANCE hInstance, string caption, int width, int height);
	~MainWindow();

	void Init();
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	BaseDialog* mCurrentDialog;
};