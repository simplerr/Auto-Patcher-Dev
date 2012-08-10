#include <fstream>
#include "Data.h"

Data::Data(string file)
{
	filename = file;
	ReadInformation(file);
}
	
Data::Data()
{
	filename = "none";
	modifyDate = "never";
}

Data::~Data()
{

}

// Reads the information from the data file.
void Data::ReadInformation(string file)
{
	ifstream fin(file);
	fin >> hostname >> username >> password >> executable >> directory >> version >> modifyDate >> files >> size; 
	fin.close();
}
	
// Writes to the data file.
void Data::WriteInformation(string file)
{
	ofstream fout(file);
	fout << hostname << "\n" << username << "\n" << password << "\n" << executable << "\n" << directory << "\n";
	fout << version << "\n" << modifyDate << "\n" << files << "\n" << size; 
	fout.close();
}

void Data::SetData(int version, int files, long size, string filename, string modifyDate, string hostname, string username, string password, string directory,
		string executable)
{
	this->version = version;
	this->files = files;
	this->size = size;
	this->filename = filename;
	this->modifyDate = modifyDate;
	this->hostname = hostname;
	this->username = username;
	this->password = password;
	this->directory = directory;
	this->executable = executable;
}