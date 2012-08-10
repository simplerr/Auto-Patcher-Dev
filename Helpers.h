#pragma once

#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
using namespace std;

#define ID_GENERATE_FILE 2001
#define CREDENTIALS_FILE "data.txt"

struct ArchiveInfo
{
	ArchiveInfo() {
		files = size = 0;
	}
	int files;
	long size;
};

string GetText(HWND hwnd);
string GetDate();
void Decrypt(string file, string& version, string& updated, string& hostname, string& username, string& password, string& executable, string& directory);
void Encrypt(string file, string version, string updated, string hostname, string username, string password, string executable, string directory);
long SizeOfFolder(string folder);
long FileSize(string file);
bool ListFiles(string path, string mask, vector<string>& files);
void CreateArchive(string path, string name, ArchiveInfo& info);
void UnzipArchive(string name, string dest);