#pragma once
#include "BaseDialog.h"

class ProgressObserver;

//! Dialog wrapper for the Patcher dialog.
class PatcherDialog : public BaseDialog
{
public:
	PatcherDialog();
	~PatcherDialog();

	void Init();

	INT_PTR GetPatchNotes();
	void UpdateInformation();
	void UploadPatch();
	void AddBytesSent(long bytes);
	void AddBytesReceived(long bytes);
	void SetFileSize(long size);
	void SetProgressStatus(string status);
	void AddText(string text, COLORREF color = RGB(0, 0, 0));
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	// Static entry point fot the upload thread.
	static void UploadThreadEntryPoint(void* pThis);
private:
	ProgressObserver* mObserver;
};

