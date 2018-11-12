#include "IgnoreKeys.h"
#include <algorithm>

Ignorekeys::KeyEntry::KeyEntry(WORD VKey, DWORD Message, bool oneTime)
	:
	VKey(VKey),
	Message(Message),
	oneTime(oneTime)
{}

void Ignorekeys::SetKeys(const std::vector<KeyEntry>& ignoreList)
{
	if (!this->ignoreList.empty())
		this->ignoreList.clear();

	this->ignoreList = ignoreList;
}

void Ignorekeys::SetKeys(std::initializer_list<KeyEntry> ignoreList)
{
	if (!this->ignoreList.empty())
		this->ignoreList.clear();

	this->ignoreList = ignoreList;
}

bool Ignorekeys::KeyIgnored(const RAWKEYBOARD& kbd)
{
	if (ignoreList.empty())
		return false;

	auto it = std::find_if(ignoreList.begin(), ignoreList.end(), 
		[&kbd](const KeyEntry& e) {return ((kbd.VKey == e.VKey) && (kbd.Message == e.Message)); });

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