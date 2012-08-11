#pragma once

#include <string>
#include "ftp/FTPClient.h"

using namespace nsFTP;
using namespace std;

/*
NOTES:

mFtpClient->UploadFile(...) must have fStoreUnique = false.
Passive mode has to be on in all function calls, fPasv = true.

*/

class PatcherDialog;

//! Gets updated when uploading/downloading files.
class ProgressObserver : public CFTPClient::CNotification
{
public:
	void SetDialog(PatcherDialog* dialog);
	void SetFileSize(long size); 
	void SetStatus(string status);
	void AddText(string text);
	void OnBytesReceived(const TByteVector& /*vBuffer*/, long /*lReceivedBytes*/);
    void OnBytesSent(const TByteVector& /*vBuffer*/, long /*lSentBytes*/); 
private:
	PatcherDialog* mDialog;
};

//! Handles everything on the FTP side.
class FtpHandler
{
public:
	FtpHandler();
	~FtpHandler();

	bool NewVersion();
	void DownloadAll(string remoteDirectory, string localDirectory);
	void UploadFile(string dest, string file);
	void LoadCredentials(string file);
	int GetVersion();
	int GetNumFiles();
	int GetTotalSize();
	string GetModifyDate();
	void SetObserver(ProgressObserver* observer);
	bool IsConnected();
private:
	CFTPClient mFtpClient;
	ProgressObserver* mObserver;
	string mWorkingDirectory;
	string mHost, mUser, mPass;
	bool mConnected;
};

extern FtpHandler* gFtpHandler;