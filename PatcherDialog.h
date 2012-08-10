#pragma once
#include "BaseDialog.h"

class ProgressObserver;

class PatcherDialog : public BaseDialog
{
public:
	PatcherDialog();
	~PatcherDialog();

	void Init();

	void UpdateInformation();
	void UploadPatch();
	void AddBytesSent(long bytes);
	void AddBytesReceived(long bytes);
	void SetFileSize(long size);
	void SetProgressStatus(string status);
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	ProgressObserver* mObserver;
};

