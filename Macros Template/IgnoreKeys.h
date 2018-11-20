#pragma once
#include <vector>
#include <Windows.h>

class Ignorekeys
{
public:
	struct KeyEntry
	{
		KeyEntry(WORD vKey, DWORD Message, bool oneTime);

		WORD vKey;
		DWORD Message;
		bool oneTime;
	};

	Ignorekeys() = default;

	void SetKeys(std::vector<KeyEntry> ignoreList);
	void SetKeys(std::initializer_list<KeyEntry> ignoreList);
	bool KeyIgnored(const RAWKEYBOARD& kbd);
private:
	std::vector<KeyEntry> ignoreList;
};
