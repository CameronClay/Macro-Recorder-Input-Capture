#include "IgnoreKeys.h"
#include <algorithm>

Ignorekeys::KeyEntry::KeyEntry(WORD vKey, DWORD Message, bool oneTime)
	:
	vKey(vKey),
	Message(Message),
	oneTime(oneTime)
{}

void Ignorekeys::SetKeys(const std::vector<KeyEntry>& ignoreList)
{
	this->ignoreList = ignoreList;
}

void Ignorekeys::SetKeys(std::initializer_list<KeyEntry> ignoreList)
{
	this->ignoreList = ignoreList;
}

bool Ignorekeys::KeyIgnored(const RAWKEYBOARD& kbd)
{
	if (ignoreList.empty())
		return false;

	auto it = std::find_if(ignoreList.begin(), ignoreList.end(), 
		[&kbd](const KeyEntry& e) {return ((kbd.VKey == e.vKey) && (kbd.Message == e.Message)); });

	if ((it != ignoreList.end()))
	{
		if (it->oneTime)
		{
			if(it != (ignoreList.end() - 1))
				*it = ignoreList.back();

			ignoreList.pop_back();
		}
		return true;
	}
	return false;
}