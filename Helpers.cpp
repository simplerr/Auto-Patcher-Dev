#include "Helpers.h"
#include <fstream>
#include <time.h>
#include <stack>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Richedit.h>
#include <Commctrl.h>
#include "zip.h"

//! Adds text to a richedit control.
/**
@param hwnd Handle to the control.
@param text The text to add.
@param crNewColor The text color.
@note Works on any window control, without the color effect.
*/
void AddEditText(HWND hwnd, string text, COLORREF crNewColor)
{
   char *Text = (char *)malloc(lstrlen(text.c_str()) + 5);
   CHARFORMAT cf;
   int iTotalTextLength = GetWindowTextLength(hwnd);
   int iStartPos = iTotalTextLength;
   int iEndPos;

   strcpy(Text, text.c_str());

   SendMessage(hwnd, EM_SETSEL, (WPARAM)(int)iTotalTextLength, (LPARAM)(int)iTotalTextLength);
   SendMessage(hwnd, EM_REPLACESEL, (WPARAM)(BOOL)FALSE, (LPARAM)(LPCSTR)Text);

   free(Text);

   cf.cbSize      = sizeof(CHARFORMAT);
   cf.dwMask      = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
   cf.dwEffects   = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
   cf.crTextColor = crNewColor;

   iEndPos = GetWindowTextLength(hwnd);

   SendMessage(hwnd, EM_SETSEL, (WPARAM)(int)iStartPos, (LPARAM)(int)iEndPos);
   SendMessage(hwnd, EM_SETCHARFORMAT, (WPARAM)(UINT)SCF_SELECTION, (LPARAM)&cf);
   SendMessage(hwnd, EM_HIDESELECTION, (WPARAM)(BOOL)TRUE, (LPARAM)(BOOL)FALSE);

   SendMessage(hwnd, EM_LINESCROLL, (WPARAM)(int)0, (LPARAM)(int)1);
}

string GetText(HWND hwnd)
{
	int inputLen = GetWindowTextLength(hwnd) + 1;
	char* inputBuffer = new char[inputLen];
	GetWindowText(hwnd, inputBuffer, inputLen);
	
	string text = inputBuffer;
	delete inputBuffer;

	return text;
}

void Decrypt(string file, string& version, string& updated, string& hostname, string& username, string& password, string& executable, string& directory)
{
	// Read the content from file.
	ifstream fin(file);
	fin >> version >> updated >> hostname >> username >> password >> executable >> directory;
	fin.close();
}

void Encrypt(string file, string version, string updated, string hostname, string username, string password, string executable, string directory)
{
	// Ouput the login information to the file.
	ofstream fout(file);
	fout << version << "\n" << updated  << "\n" << hostname << "\n" << username << "\n" << password << "\n" << executable << "\n" << directory;
	fout.close();
}

string GetDate()
{
	// Current date/time based on current system
	time_t now = time(0);

	// Convert now to tm struct for local timezone
	tm* localtm = localtime(&now);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%x", localtm);
	string date = buffer;

	return date;
}

long SizeOfFolder(string folder)
{
	long size = 0;

	WIN32_FIND_DATAA data;
	string dir = folder + ".";
	FindFirstFile(dir.c_str(), &data);
	return 0;
}

long FileSize(string file)
{
	FILE *pFile = NULL;

	fopen_s(&pFile, file.c_str(), "rb");

	// Set the file pointer to end of file.
	fseek(pFile, 0, SEEK_END);

	// Get the file size.
	int size = ftell(pFile);

	// Close stream and release buffer.
	fclose(pFile);

	return size;
}

bool ListFiles(string path, string mask, vector<string>& files)
{
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;
	string spec;
    stack<string> directories;

    directories.push(path);
    files.clear();

    while (!directories.empty()) {
        path = directories.top();
        spec = path + "/" + mask;
        directories.pop();

        hFind = FindFirstFile(spec.c_str(), &ffd);
        if (hFind == INVALID_HANDLE_VALUE)  {
            return false;
        } 

        do {
            if (strcmp(ffd.cFileName, ".") != 0 && 
                strcmp(ffd.cFileName, "..") != 0) {
                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    directories.push(path + "/" + ffd.cFileName);
                }
                else {
                    files.push_back(path + "/" + ffd.cFileName);
                }
            }
        } while (FindNextFile(hFind, &ffd) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES) {
            FindClose(hFind);
            return false;
        }

        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }
}

// Packages all files in "path" into a .zip file.
void CreateArchive(string path, string name, ArchiveInfo& info)
{
	HZIP hz = CreateZip(name.c_str(), 0);

	HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;
	string spec;
    stack<string> directories;

    directories.push(path);

    while (!directories.empty()) {
        path = directories.top();
        spec = path + "/*";
        directories.pop();

        hFind = FindFirstFile(spec.c_str(), &ffd);
        if (hFind == INVALID_HANDLE_VALUE)  
            return;

        do {
            if (strcmp(ffd.cFileName, ".") != 0 && strcmp(ffd.cFileName, "..") != 0) {
                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					// Add folder to archive.
					ZipAddFolder(hz, string(path + "/" + ffd.cFileName).c_str());
                    directories.push(path + "/" + ffd.cFileName);
                }
                else {
					// Add file to archive.
					ZipAdd(hz, string(path + "/" + ffd.cFileName).c_str(), string(path + "/" + ffd.cFileName).c_str());
					info.files++;
					info.size += FileSize(path + "/" + ffd.cFileName);
                }
            }
        } while (FindNextFile(hFind, &ffd) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES) {
            FindClose(hFind);
            return;
        }

        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

	CloseZip(hz);
}

void UnzipArchive(string name, string dest)
{
	//HZIP hz = OpenZip("data/simple1.zip", 0);
	////SetUnzipBaseDir(hz, "\\");
	//ZIPENTRY ze;
	//GetZipItem(hz, -1, &ze);
	//int items = ze.index;

	//for(int zi = 0; zi < items; zi++)
	//{
	//	ZIPENTRY ze;
	//	GetZipItem(hz, zi, &ze);
	//	UnzipItem(hz, zi, ze.name);
	//}

	//CloseZip(hz);
}