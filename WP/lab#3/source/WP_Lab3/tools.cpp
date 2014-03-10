


#include <io.h>
#include <cstdio>
#include <fcntl.h>
#include <Windows.h>
#include <string>
#include <fstream>
#include <regex>
#include <map>


void CreateDeveloperConsole()
{
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    int consoleHandleR, consoleHandleW ;
    long stdioHandle;
    FILE *fptr;

    AllocConsole();
    std::wstring strW = L"Dev Console";
    SetConsoleTitle( strW.c_str() );

    EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE , MF_GRAYED);
    DrawMenuBar(GetConsoleWindow());

    GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo );

    stdioHandle = (long)GetStdHandle( STD_INPUT_HANDLE );
    consoleHandleR = _open_osfhandle( stdioHandle, _O_TEXT );
    fptr = _fdopen( consoleHandleR, "r" );
    *stdin = *fptr;
    setvbuf( stdin, NULL, _IONBF, 0 );

    stdioHandle = (long) GetStdHandle( STD_OUTPUT_HANDLE );
    consoleHandleW = _open_osfhandle( stdioHandle, _O_TEXT );
    fptr = _fdopen( consoleHandleW, "w" );
    *stdout = *fptr;
    setvbuf( stdout, NULL, _IONBF, 0 );

    stdioHandle = (long)GetStdHandle( STD_ERROR_HANDLE );
    *stderr = *fptr;
    setvbuf( stderr, NULL, _IONBF, 0 );
}



bool LoadConfigFile(char const * filePath, std::map<std::string, std::string> & options)
{
    std::ifstream configFile(filePath, std::ifstream::in);

    if (configFile.fail())
    {
        return false;
    }

    std::regex rgx("(\\w+)\\s*=\\s*(\\w+)");
    std::smatch results;

    while (configFile.good())
    {
        std::string str;
        getline(configFile, str);       

        if (regex_search(str, results, rgx))
        {
            options[results.str(1)] = results.str(2);
        }
    }

    return true;
}

