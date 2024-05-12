#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <algorithm>

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
        return 1;
    }

    file_name = argv[1];
    for (wchar_t drive = L'A'; drive <= L'Z'; ++drive) {
        std::wstring rootPath = std::wstring(1, drive) + L":";
        ListFiles(rootPath);
    }
    return 0;
}