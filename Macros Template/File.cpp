#include "File.h"
#include <Shlwapi.h>

#pragma comment( lib, "shlwapi.lib" )

std::vector<std::string> File::GetFileNameList(const TCHAR* folder, DWORD filter, bool inclusion)
{
	WIN32_FIND_DATA fileSearch;
	TCHAR buffer[MAX_PATH] = {};
	PathCombine(buffer, folder, _T("*"));

	std::vector<std::string> list;

	HANDLE hnd = FindFirstFile(buffer, &fileSearch);
	if (hnd != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (inclusion ? (fileSearch.dwFileAttributes & filter) : !(fileSearch.dwFileAttributes & filter))
			{
				if (fileSearch.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (_tcscmp(fileSearch.cFileName, _T(".")) != 0 && _tcscmp(fileSearch.cFileName, _T("..")) != 0)
					{
						std::vector<std::string> a = GetFileNameList((std::string(folder) + _T("\\") + std::string(fileSearch.cFileName)).c_str(), filter, inclusion);
						for (auto i = a.begin(); i != a.end(); i++)
						{
							list.push_back(std::string(fileSearch.cFileName) + _T("\\") + *i);
						}
					}
				}
				else
				{
					SYSTEMTIME t;
					FileTimeToSystemTime(&fileSearch.ftLastWriteTime, &t);
					list.push_back(fileSearch.cFileName);
				}
			}
		} while (FindNextFile(hnd, &fileSearch));
		FindClose(hnd);
	}

	return list;
}

void File::CreateFolder(const TCHAR* fileName, DWORD fileAttributes)
{
	if (!CreateDirectory(fileName, NULL))
	{
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			TCHAR *buff = _tcsdup(fileName);
			PathRemoveFileSpec(buff);
			CreateFolder(buff);
			CreateDirectory(fileName, NULL);
			free(buff);
		}
	}
	SetFileAttributes(fileName, fileAttributes);
}

void File::RemoveFolder(const TCHAR* fileName)
{
	RemoveDirectory(fileName);
}

void File::Remove(const TCHAR* fileName)
{
	DeleteFile(fileName);
}

bool File::Exists(const TCHAR* fileName)
{
	return PathFileExists(fileName) != FALSE;
}

void File::SetCurDirectory(const TCHAR* folderName)
{
	SetCurrentDirectory(folderName);
}

void File::GetCurDirectory(TCHAR* buffer)
{
	GetCurrentDirectory(MAX_PATH, buffer);
}