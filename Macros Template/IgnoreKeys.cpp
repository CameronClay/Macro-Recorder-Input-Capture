#include "IgnoreKeys.h"

Ignorekeys::KeyEntry::KeyEntry(WORD VKey, DWORD Message, bool oneTime)
	:
	VKey(VKey),
	Message(Message),
	oneTime(oneTime)
{}


Ignorekeys::Ignorekeys() {}
Ignorekeys::~Ignorekeys() {}

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

	for (int i = 0, size = ignoreList.size(); i < size; ++i)
	{
		if ((kbd.VKey == ignoreList[i].VKey) && (kbd.Message == ignoreList[i].Message))
		{
			if (ignoreList[i].oneTime)
			{
				if (i != size - 1)
					ignoreList[i] = ignoreList.back();

				ignoreList.pop_back();
			}
			return true;
		}
	}
	return false;
}