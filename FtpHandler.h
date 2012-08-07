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
private:
	CFTPClient mFtpClient;
	string mWorkingDirectory;
	string mLocalDirectory;
	string mHost, mUser, mPass;
};

extern FtpHandler* gFtpHandler;