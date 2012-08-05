#pragma once

#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
using namespace std;

#define ID_GENERATE_FILE 2001

string GetText(HWND hwnd);
void Decrypt(string file, string& hostname, string& username, string& password, string& executable, string& directory);