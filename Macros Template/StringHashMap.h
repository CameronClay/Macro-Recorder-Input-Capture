#pragma once
#include <unordered_map>
#include <mutex>
#include <string>

class StringHashMap
{
public:
	struct EMPTY_STRUCT{};

	StringHashMap() = default;

	void AddString(const std::string& str);
	bool RemoveString(const std::string& str);

	void AddStringNL(const std::string& str);
	bool RemoveStringNL(const std::string& str);

	void Lock();
	void Unlock();

	std::unordered_map <std::string, EMPTY_STRUCT>& GetOutStrings();
private:
	std::unordered_map <std::string, EMPTY_STRUCT> outStrings;
	std::mutex mut;
};