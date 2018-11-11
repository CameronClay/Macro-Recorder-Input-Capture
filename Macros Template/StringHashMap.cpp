#include "StringHashMap.h"

void StringHashMap::AddStringNL(const std::string& str)
{
	outStrings.emplace(std::pair<std::string, EMPTY_STRUCT>(str, {}));
}
bool StringHashMap::RemoveStringNL(const std::string& str)
{
	return outStrings.erase(str);
}

void StringHashMap::AddString(const std::string& str)
{
	Lock();
	AddStringNL(str);
	Unlock();
}
bool StringHashMap::RemoveString(const std::string& str)
{
	Lock();
	const size_t res = RemoveStringNL(str);
	Unlock();
	return res;
}

std::unordered_map <std::string, StringHashMap::EMPTY_STRUCT>& StringHashMap::GetOutStrings()
{
	return outStrings;
}
void StringHashMap::Lock()
{
	mut.lock();
}
void StringHashMap::Unlock()
{
	mut.unlock();
}