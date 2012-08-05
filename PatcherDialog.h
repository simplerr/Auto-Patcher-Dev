#pragma once
#include "BaseDialog.h"

class PatcherDialog : public BaseDialog
{
public:
	PatcherDialog();
	~PatcherDialog();

	void Init();

	void UpdateInformation();
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
};

