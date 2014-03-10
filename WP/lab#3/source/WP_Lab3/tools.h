#pragma once

#include <map>
#include <string>



bool LoadConfigFile(char const * filePath, std::map<std::string, std::string> & options);
void CreateDeveloperConsole();
