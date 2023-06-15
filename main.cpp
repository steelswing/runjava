/*
Ну вы же понимаете, что код здесь только мой?
Well, you do understand that the code here is only mine?
 */

/* 
 * File:   main.cpp
 * Author: LWJGL2
 *
 * Created on 16 июня 2023 г., 08:26
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <windows.h>

using namespace std;

const wchar_t* char_to_wchar(const char *c);
char* wchar_to_char(const wchar_t* pwchar);
inline std::wstring get_env(const char* key);

__declspec (dllexport)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR szCmdLine, int iCmdShow) {
    int nArgs;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    wstring str = L"\"" + get_env("JAVA_HOME") + L"\\bin\\java.exe\" -jar ";

    for (int i = 1; i < nArgs; i++) {
        str += argv[i];
        str += L" ";
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof (si));
    si.cb = sizeof (si);
    ZeroMemory(&pi, sizeof (pi));
    CreateProcess(NULL, str.c_str(), NULL, NULL, 0, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    return 0;
}

inline std::wstring get_env(const char* key) {
    if (key == nullptr) {
        throw std::invalid_argument("Null pointer passed as environment variable name");
    }
    if (*key == '\0') {
        throw std::invalid_argument("Value requested for the empty-name environment variable");
    }
    const wchar_t* ev_val = char_to_wchar(getenv(key));
    if (ev_val == nullptr) {
        throw std::runtime_error("Environment variable not defined");
    }
    return std::wstring{ ev_val};
}

char* wchar_to_char(const wchar_t* pwchar) {
    // get the number of characters in the string.
    int currentCharIndex = 0;
    char currentChar = pwchar[currentCharIndex];

    while (currentChar != '\0') {
        currentCharIndex++;
        currentChar = pwchar[currentCharIndex];
    }

    const int charCount = currentCharIndex + 1;

    // allocate a new block of memory size char (1 byte) instead of wide char (2 bytes)
    char* filePathC = (char*) malloc(sizeof (char) * charCount);

    for (int i = 0; i < charCount; i++) {
        // convert to char (1 byte)
        char character = pwchar[i];

        *filePathC = character;

        filePathC += sizeof (char);

    }
    filePathC += '\0';

    filePathC -= (sizeof (char) * charCount);

    return filePathC;
}

const wchar_t* char_to_wchar(const char *c) {
    const size_t cSize = strlen(c) + 1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs(wc, c, cSize);
    return wc;
}