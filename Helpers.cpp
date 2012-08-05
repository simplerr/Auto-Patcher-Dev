#include "Helpers.h"
#include <fstream>

string GetText(HWND hwnd)
{
	int inputLen = GetWindowTextLength(hwnd) + 1;
	char* inputBuffer = new char[inputLen];
	GetWindowText(hwnd, inputBuffer, inputLen);
	
	string text = inputBuffer;
	delete inputBuffer;

	return text;
}

void Decrypt(string file, string& hostname, string& username, string& password, string& executable, string& directory)
{
	// Read the content from file.
	ifstream fin(file);
	fin >> hostname >> username >> password >> executable >> directory;
	fin.close();
}