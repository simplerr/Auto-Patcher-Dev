#include <fstream>
#include <iostream>
#include "FtpHandler.h"
#include "Helpers.h"

FtpHandler::FtpHandler(string workingDirectory, string localDirectory)
{
	WORD wVersionRequested;
	WSADATA wsaData;

	// Startup the usage of sockets.
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);

	// Load login credentials.
	LoadCredentials(CREDENTIALS_FILE);

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

void FtpHandler::UploadAll()
{
	// Skip the the first 2 files (. and ..)
	WIN32_FIND_DATA data;
	string dir = mLocalDirectory + "*";
	HANDLE handle = FindFirstFile(dir.c_str(), &data);
	FindNextFile(handle, &data);

	while(GetLastError() != ERROR_NO_MORE_FILES)
	{
		FindNextFile(handle, &data);

		if(GetLastError() != ERROR_NO_MORE_FILES) {
			mFtpClient.Delete(mWorkingDirectory + data.cFileName);
			mFtpClient.UploadFile(mLocalDirectory + data.cFileName, mWorkingDirectory + data.cFileName, false, CRepresentation(CType::Image()), true);
		}
	}
}

// Loads the login credentials from a text file.
void FtpHandler::LoadCredentials(string file)
{
	string trash;
	Decrypt(CREDENTIALS_FILE, trash, trash, mHost, mUser, mPass, trash, trash);
}

int FtpHandler::GetVersion()
{
	// Compare the version files.
	string tmp = mLocalDirectory + "tmp_credentials.txt";
	remove(tmp.c_str());

	if(!mFtpClient.DownloadFile(mWorkingDirectory + "credentials.txt", tmp, CType::Image(), true)) 
		return -1;

	std::ifstream fin("data/tmp_credentials.txt");

	// Read the versions.
	int ftpVersion;
	fin >> ftpVersion;
	fin.close();

	remove("data/tmp_credentials.txt");

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

string FtpHandler::GetModifyDate()
{
	// Compare the version files.
	string tmp = mLocalDirectory + "tmp_credentials.txt";
	remove(tmp.c_str());

	if(!mFtpClient.DownloadFile(mWorkingDirectory + "credentials.txt", tmp, CType::Image(), true)) 
		return "error";

	std::ifstream fin("data/tmp_credentials.txt");

	// Read the date (second line).
	string date;
	fin >> date >> date;
	fin.close();

	remove("data/tmp_credentials.txt");

	return date;
}