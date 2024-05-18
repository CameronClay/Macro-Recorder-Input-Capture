#include "KeyComboRec.h"

KeyComboRec::KeyComboRec()
	:
	recordType(NONE)
{}

void KeyComboRec::AddVKey(TCHAR key) {
	//If no repeats add key
	if (!HasRecorded() || (vKeys.back() != key))
		vKeys.push_back(key);
}
void KeyComboRec::StartRecording() {
	vKeys.clear();
	recordType = RECORDING;
}
void KeyComboRec::StartDeleting() {
	vKeys.clear();
	recordType = DELETING;
}
void KeyComboRec::Stop() {
	recordType = NONE;
}

const std::vector<TCHAR>& KeyComboRec::GetVKeys() const {
	return vKeys;
}

KeyComboRec::RecordType KeyComboRec::GetRecordType() const {
	return recordType;
}

bool KeyComboRec::HasRecorded() const {
	return !vKeys.empty();
}