#pragma once

#include <string>
using namespace std;

//! Stores the login data and takes care of encryption/decryption.
struct Data
{
	Data(string file);
	Data();
	~Data();

	void SetData(int version, int files, long size, string filename, string modifyDate, string hostname, string username, string password, string directory,
		string executable);
	void ReadInformation(string file);
	void WriteInformation(string file);

	int version;
	int files;
	long size;
	string filename;
	string modifyDate;
	string hostname;
	string username;
	string password;
	string directory;
	string executable;
};