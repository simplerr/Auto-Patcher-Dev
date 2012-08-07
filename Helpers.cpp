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
