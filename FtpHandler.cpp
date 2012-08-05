#include <fstream>
#include <iostream>
#include "FtpHandler.h"

FtpHandler::FtpHandler(string workingDirectory, string localDirectory)
{
	WORD wVersionRequested;
	WSADATA wsaData;

	// Startup the usage of sockets.
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);

	// Load login credentials.
	LoadCredentials("credentials.txt");

	CLogonInfo logonInfo(mHost, 21, mUser, mPass);

	// Attempt to login.
	if(!mFtpClient.Login(logonInfo))
		MessageBox(0, "Can't connect to server.", "Connection error", 0);

	mWorkingDirectory = workingDirectory;
	mLocalDirectory = localDirectory;
}

FtpHandler::~FtpHandler()
{
	// Logout from the FTP server.
	mFtpClient.Logout();
}

// Returns true if there exist a newer version on the FTP server.
bool FtpHandler::NewVersion()
{
	// Compare the version files.
	mFtpClient.DownloadFile(mWorkingDirectory + "version.txt", mLocalDirectory + "ftp_version.txt", CType::Image(), true); 
	std::ifstream fin(mLocalDirectory +"ftp_version.txt");

	// Read the versions.
	int ftpVersion, clientVersion;
	fin >> ftpVersion;
	fin.close();

	// Remove temp file.
	remove("data/ftp_version.txt");

	fin.open("data/version.txt");
	fin >> clientVersion;
	fin.close();

	// Does the client have the latest version?
	return clientVersion < ftpVersion || clientVersion > 9999;
}
	
// Download all files in the remote directory.
void FtpHandler::DownloadAll(string remoteDirectory, string localDirectory)
{
	// Get directory listing.
	TSpFTPFileStatusVector list;
	mFtpClient.List(remoteDirectory, list, true);

	// Titles.
	cout << "[File]";
	//gotoxy(20, getxy().Y);
	cout << "[Size]\n";

	// Loop through and download all files.
	for(auto iter = list.begin() + 2; iter != list.end(); iter++) {
		TSpFTPFileStatus file = (*iter);

		// Download the file.
		string ftpFilename = remoteDirectory + file->Name();
		string localFilename = localDirectory + file->Name();
		remove(localFilename.c_str());
		if(!mFtpClient.DownloadFile(ftpFilename, localFilename, CType::Image(), true))
			MessageBox(0, "Error downloading file.", "Update error", 0);
		else {
			cout << file->Name();
			//gotoxy(20, getxy().Y);
			cout << file->Size() << endl;
		}
	}

	cout << "\n";
}

// Loads the login credentials from a text file.
void FtpHandler::LoadCredentials(string file)
{
	std::ifstream fin(file);
	fin >> mHost >> mHost >> mUser >> mPass;
	fin.close();
}

int FtpHandler::GetVersion()
{
	// Compare the version files.ftp
	mFtpClient.DownloadFile(mWorkingDirectory + "credentials.txt", mLocalDirectory + "tmp_credentials.txt", CType::Image(), true); 
	std::ifstream fin("data/tmp_credentials.txt");

	// Read the versions.
	int ftpVersion;
	fin >> ftpVersion;
	fin.close();

	// Remove temp file.
	remove("data/ftp_version.txt");

	return ftpVersion;
}
	
int FtpHandler::GetNumFiles()
{
	// Get directory listing.
	TSpFTPFileStatusVector list;
	mFtpClient.List(mWorkingDirectory, list, true);

	return list.size() - 2;
}
	
int FtpHandler::GetTotalSize()
{
	TSpFTPFileStatusVector list;
	mFtpClient.List(mWorkingDirectory, list, true);
	
	// Loop through and add all file sizes.
	int size = 0;
	for(auto iter = list.begin() + 2; iter != list.end(); iter++) {
		TSpFTPFileStatus file = (*iter);
		size += file->Size();
	}

	return size;
}