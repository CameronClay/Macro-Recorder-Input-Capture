#pragma once
#include <vector>
#include <Windows.h>

class Ignorekeys
{
public:
	struct KeyEntry
	{
		KeyEntry(WORD VKey, DWORD Message, bool oneTime);

		WORD VKey;
		DWORD Message;
		bool oneTime;
	};

	Ignorekeys();
	~Ignorekeys();

	void SetKeys(const std::vector<KeyEntry>& ignoreList);
	void SetKeys(std::initializer_list<KeyEntry> ignoreList);
	bool KeyIgnored(const RAWKEYBOARD& kbd);
private:
	std::vector<KeyEntry> ignoreList;
};
