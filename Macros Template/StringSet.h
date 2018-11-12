#pragma once
#include <unordered_set>
#include <mutex>
#include <string>

class StringSet
{
public:
	StringSet() = default;

	void AddString(const std::string& str);
	bool RemoveString(const std::string& str);

	void AddStringNL(const std::string& str);
	bool RemoveStringNL(const std::string& str);

	void Lock();
	void Unlock();

	std::unordered_set<std::string>& GetOutStrings();
private:
	std::unordered_set<std::string> outStrings;
	std::mutex mut;
};