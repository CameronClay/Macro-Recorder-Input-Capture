#pragma once
#include <vector>
#include <tchar.h>

class KeyComboRec
{
public:
	enum RecordType
	{
		NONE,
		RECORDING,
		DELETING
	};
	KeyComboRec();

	void AddVKey(TCHAR key);
	void StartRecording();
	void StartDeleting();
	void Stop();

	const std::vector<TCHAR>& GetVKeys() const;
	RecordType GetRecordType() const;
	bool HasRecorded() const;
private:
	std::vector<TCHAR> vKeys;
	RecordType recordType;
};