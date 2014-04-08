#pragma once

#include <map>
#include <string>
#include <windows.h>



bool LoadConfigFile(char const * filePath, std::map<std::string, std::string> & options);
void CreateDeveloperConsole();
void SetFontForChildren(HWND parent, HFONT font);
BOOL CALLBACK SetFontProc(HWND child, LPARAM lParam);
