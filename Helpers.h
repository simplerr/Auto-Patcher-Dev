#pragma once

#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
using namespace std;

#define ID_GENERATE_FILE 2001
#define CREDENTIALS_FILE "data.txt"

//! Structure with information about an zipped archive.
struct ArchiveInfo
{
	ArchiveInfo() {
		files = size = 0;
	}
	int files;
	long size;
};

//! Adds text to a richedit control.
void AddEditText(HWND hwnd, string text, COLORREF crNewColor = RGB(0, 0, 0));

//! Returns the text of a window control.
string GetText(HWND hwnd);

//! Returns the current date (08/10/12)
string GetDate();

//! Returns the size of a folder.
long SizeOfFolder(string folder);

//! Returns the size of a file.
long FileSize(string file);

//! Fills a vector with all files in a directory.
bool ListFiles(string path, string mask, vector<string>& files);

//! Archives all files in a directory to a .zip file.
void CreateArchive(string path, string name, ArchiveInfo& info);

//! Unzips an archive (.zip).
void UnzipArchive(string name, string dest);

void Decrypt(string file, string& version, string& updated, string& hostname, string& username, string& password, string& executable, string& directory);
void Encrypt(string file, string version, string updated, string hostname, string username, string password, string executable, string directory);