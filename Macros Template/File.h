//Copyright (c) <2015> <Cameron Clay>

#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <tchar.h>

namespace File
{
	std::vector<std::string> GetFileNameList(const TCHAR* folder, DWORD filter = NULL, bool inclusion = false);

	void Remove(const TCHAR* fileName);
	bool Exists(const TCHAR* fileName);

	void SetCurDirectory(const TCHAR* folderName);
	void GetCurDirectory(TCHAR* buffer);

	// FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_TEMPORARY, blank = NORMAL
	void CreateFolder(const TCHAR* fileName, DWORD fileAttributes = FILE_ATTRIBUTE_NORMAL);
	void RemoveFolder(const TCHAR* fileName);
};


