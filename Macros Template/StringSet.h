#pragma once
#include <unordered_set>
#include <mutex>
#include <string>

//Set of strings used to display on screen
class StringSet
{
public:
	StringSet() = default;

	void AddString(const std::string& str);
	bool RemoveString(const std::string& str);

	//with New Line (NL)
	void AddStringNL(const std::string& str);
	bool RemoveStringNL(const std::string& str);

	void Lock();
	void Unlock();

	const std::unordered_set<std::string>& GetOutStrings() const;
private:
	std::unordered_set<std::string> outStrings;
	std::mutex mut;
};