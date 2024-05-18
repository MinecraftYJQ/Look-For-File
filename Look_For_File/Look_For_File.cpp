#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>

bool ContainsChinese(const std::wstring& str) {
    for (wchar_t ch : str) {
        if (ch >= 0x4e00 && ch <= 0x9fff) {
            return true; // 中文字符的Unicode编码范围
        }
    }
    return false;
}

std::wstring file_name;
void ListFiles(const std::wstring& path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + L"\\*").c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
                    ListFiles(path + L"\\" + findFileData.cFileName);
                }
            }
            else {
                std::wstring fileName = findFileData.cFileName;
                std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::towlower);
                if (fileName == file_name) {
                    std::wstring filePath = path + L"\\" + findFileData.cFileName;
                    if (!ContainsChinese(filePath)) {
                        std::wcout << filePath << std::endl;
                    }
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

int wmain(int argc, wchar_t* argv[]) {
    system("cls");
    if (argc < 2) {
        std::wcout << L"Usage: Look_For_File <file_name>" << std::endl;
        std::string names;
        std::wcout << L"Please enter the name of the file you are looking for:";
        std::cin >> names;
        // 使用wstring_convert和codecvt来转换字符串
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

        // 将std::string转换为std::wstring
        std::wstring wstr = converter.from_bytes(names);

        // 将std::wstring转换为wchar_t*
        wchar_t* wideCharStr = new wchar_t[wstr.length() + 1];
        std::copy(wstr.begin(), wstr.end(), wideCharStr);
        wideCharStr[wstr.length()] = '\0'; // 确保字符串以空字符结尾
        file_name = wideCharStr;
        for (wchar_t drive = L'A'; drive <= L'Z'; ++drive) {
            std::wstring rootPath = std::wstring(1, drive) + L":";
            ListFiles(rootPath);
        }
        return 0;
    }

    file_name = argv[1];
    for (wchar_t drive = L'A'; drive <= L'Z'; ++drive) {
        std::wstring rootPath = std::wstring(1, drive) + L":";
        ListFiles(rootPath);
    }
    return 0;
}