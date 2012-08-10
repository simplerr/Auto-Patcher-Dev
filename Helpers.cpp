#include "Helpers.h"
#include <fstream>
#include <time.h>

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