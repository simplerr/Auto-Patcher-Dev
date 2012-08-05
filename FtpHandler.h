#pragma once

#include <string>
#include "ftp/FTPClient.h"

using namespace nsFTP;
using namespace std;

class FtpHandler
{
public:
	FtpHandler(string workingDirectory, string localDirectory);
	~FtpHandler();

	bool NewVersion();
	void DownloadAll(string remoteDirectory, string localDirectory);
	void LoadCredentials(string file);
	int GetVersion();
	int GetNumFiles();
	int GetTotalSize();
private:
	CFTPClient mFtpClient;
	string mWorkingDirectory;
	string mLocalDirectory;
	string mHost, mUser, mPass;
};

extern FtpHandler* gFtpHandler;