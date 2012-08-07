#pragma once

#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
using namespace std;

#define ID_GENERATE_FILE 2001
#define CREDENTIALS_FILE "data/credentials.txt"

string GetText(HWND hwnd);
string GetDate();
void Decrypt(string file, string& version, string& updated, string& hostname, string& username, string& password, string& executable, string& directory);
void Encrypt(string file, string version, string updated, string hostname, string username, string password, string executable, string directory);