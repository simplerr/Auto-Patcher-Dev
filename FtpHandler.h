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

class ProgressObserver : public CFTPClient::CNotification
{
public:
	void SetDialog(PatcherDialog* dialog);
	void SetFileSize(long size); 
	void SetUploading(string file);
	void OnBytesReceived(const TByteVector& /*vBuffer*/, long /*lReceivedBytes*/);
    void OnBytesSent(const TByteVector& /*vBuffer*/, long /*lSentBytes*/); 
private:
	PatcherDialog* mDialog;
};

class FtpHandler
{
public:
	FtpHandler(string workingDirectory, string localDirectory);
	~FtpHandler();

	bool NewVersion();
	void DownloadAll(string remoteDirectory, string localDirectory);
	void UploadAll();
	void LoadCredentials(string file);
	int GetVersion();
	int GetNumFiles();
	int GetTotalSize();
	string GetModifyDate();
	void SetObserver(ProgressObserver* observer);
private:
	CFTPClient mFtpClient;
	ProgressObserver* mObserver;
	string mWorkingDirectory;
	string mLocalDirectory;
	string mHost, mUser, mPass;
};

extern FtpHandler* gFtpHandler;