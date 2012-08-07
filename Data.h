#pragma once

#include <string>
using namespace std;

//! Stores the login data and takes care of encryption/decryption.
class Data
{
public:
	Data(string file);
	Data();
	~Data();

	void ReadInformation(string file);
	void WriteInformation(string file);

	int GetVersion();
	string GetModifyDate();
	string GetHostname();
	string GetUsername();
	string GetPassword();
	string GetDirectory();
	string GetExecutable();

	void SetData(int version, string date, string hostname, string username, string password, string executable, string directory);
	void SetVersion(int version);
	void SetModifyDate(string date);
	void SetHostname(string hostname);
	void SetUsername(string username);
	void SetPassword(string password);
	void SetDirectory(string directory);
	void SetExecutable(string executable);

private:
	int mVersion;
	string mFilename;
	string mModifyDate;
	string mHostname;
	string mUsername;
	string mPassword;
	string mDirectory;
	string mExecutable;
};