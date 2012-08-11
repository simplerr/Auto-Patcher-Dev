#pragma once
#include "Runnable.h"
#include "BaseDialog.h"

class FtpPatcher;

//! The main window which extends Runnable.
class MainWindow : public Runnable
{
public:
	MainWindow(HINSTANCE hInstance, string caption, int width, int height);
	~MainWindow();

	void Init();
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	BaseDialog* GetCurrentDialog();
private:
	BaseDialog* mCurrentDialog;
};