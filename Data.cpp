#include <fstream>
#include "Data.h"

Data::Data(string file)
{
	mFilename = file;
	ReadInformation(file);
}
	
Data::Data()
{
	mFilename = "none";
	SetModifyDate("date");
}

Data::~Data()
{

}

// Reads the information from the data file.
void Data::ReadInformation(string file)
{
	ifstream fin(file);
	fin >> mVersion >> mModifyDate >> mHostname >> mUsername >> mPassword >> mExecutable >> mDirectory;
	fin.close();
}
	
// Writes to the data file.
void Data::WriteInformation(string file)
{
	ofstream fout(file);
	fout << mVersion << "\n" << mModifyDate  << "\n" << mHostname << "\n" << mUsername << "\n" << mPassword << "\n" << mExecutable << "\n" << mDirectory;
	fout.close();
}

int Data::GetVersion()
{
	return mVersion;
}
	
string Data::GetModifyDate()
{
	return mModifyDate;
}
	
string Data::GetHostname()
{
	return mHostname;
}
	
string Data::GetUsername()
{
	return mUsername;
}
	
string Data::GetPassword()
{
	return mPassword;
}

string Data::GetDirectory()
{
	return mDirectory;
}
	
string Data::GetExecutable()
{
	return mExecutable;
}

void Data::SetData(int version, string date, string hostname, string username, string password, string executable, string directory)
{
	SetVersion(version);
	SetModifyDate(date);
	SetHostname(hostname);
	SetUsername(username);
	SetPassword(password);
	SetExecutable(executable);
	SetDirectory(directory);
}

void Data::SetVersion(int version)
{
	mVersion = version;
}

void Data::SetModifyDate(string date)
{
	mModifyDate = date;
}
	
void Data::SetHostname(string hostname)
{
	mHostname = hostname;
}
	
void Data::SetUsername(string username)
{
	mUsername = username;
}
	
void Data::SetPassword(string password)
{
	mPassword = password;
}

void Data::SetDirectory(string directory)
{
	mDirectory = directory;
}
	
void Data::SetExecutable(string executable)
{
	mExecutable = executable;
}
