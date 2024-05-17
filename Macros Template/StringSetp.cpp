#include "StringSet.h"

void StringSet::AddStringNL(const std::string& str)
{
	outStrings.emplace(str);
}
bool StringSet::RemoveStringNL(const std::string& str)
{
	return outStrings.erase(str);
}

void StringSet::AddString(const std::string& str)
{
	Lock();
	AddStringNL(str);
	Unlock();
}
bool StringSet::RemoveString(const std::string& str)
{
	Lock();
	const size_t res = RemoveStringNL(str);
	Unlock();

	return res;
}

const std::unordered_set<std::string>& StringSet::GetOutStrings()
{
	return outStrings;
}
void StringSet::Lock()
{
	mut.lock();
}
void StringSet::Unlock()
{
	mut.unlock();
}