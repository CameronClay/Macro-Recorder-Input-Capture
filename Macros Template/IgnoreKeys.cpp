#include "IgnoreKeys.h"
#include <algorithm>

Ignorekeys::KeyEntry::KeyEntry(WORD VKey, DWORD Message, bool oneTime)
	:
	VKey(VKey),
	Message(Message),
	oneTime(oneTime)
{}

void Ignorekeys::SetKeys( std::vector<KeyEntry> ignoreList )
{
	this->ignoreList = std::move( ignoreList );
}

void Ignorekeys::SetKeys(std::initializer_list<KeyEntry> ignoreList)
{
	this->ignoreList = ignoreList;
}

bool Ignorekeys::KeyIgnored(const RAWKEYBOARD& kbd)
{
	if (ignoreList.empty())
		return false;

	auto key_in_list = [ &kbd ]( const KeyEntry& e )
	{
		return ( ( kbd.VKey == e.VKey ) && ( kbd.Message == e.Message ) );
	};

	if( auto it = std::find_if( ignoreList.begin(), ignoreList.end(), key_in_list); 
		it != ignoreList.end() )
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