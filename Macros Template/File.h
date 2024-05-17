#pragma once
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace File
{
	std::vector<std::string> GetFileList(const std::string& dir, const std::vector<std::string>& dirSkipList = {});
}