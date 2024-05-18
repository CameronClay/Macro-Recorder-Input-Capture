#include "StringSet.h"

void StringSet::AddStringNL(const std::string& str) {
	Lock();
	outStrings.emplace(str);
	Unlock();
}
bool StringSet::RemoveStringNL(const std::string& str) {
	Lock();
	const size_t res = outStrings.erase(str);
	Unlock();

	return res;
}

void StringSet::AddString(const std::string& str) {
	return AddStringNL(str);
}
bool StringSet::RemoveString(const std::string& str) {
	return RemoveStringNL(str);
}

const std::unordered_set<std::string>& StringSet::GetOutStrings() const {
	return outStrings;
}
void StringSet::Lock() {
	mut.lock();
}
void StringSet::Unlock() {
	mut.unlock();
}