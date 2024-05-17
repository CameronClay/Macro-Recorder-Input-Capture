#pragma once
#include "InputHandler.h"
#include <string>

class RecordList
{
public:
	static constexpr int INVALID = -1;

	RecordList();	
	~RecordList();

	template<typename T, typename... Args>
	void AddEventToRecord(Args&&... vals)
	{
		records[currentRecord].handler.Add<T, Args...>(std::forward<Args>(vals)...);
	}

	bool Initialize(const TCHAR* workingDir);
	int SelectRecord(const RAWKEYBOARD& kbd);
	bool AddRecord(const std::vector<TCHAR>& toggleVKeys);
	bool DeleteRecord(const std::vector<TCHAR>& toggleVKeys);

	void SimulateRecord();

	const Input* GetBack() const;
	Input* GetBack();
	void PopBack();

	void Save();

	void StartRecording();
	void StopRecording();

	bool IsRecording() const;
	bool IsSimulating() const;
	bool HasRecorded() const;

	int GetCurrentRecord() const;
private:
	int FindRecord(const std::vector<TCHAR>& toggleVKeys) const;

	struct InputRecord
	{
		InputRecord() = default;
		InputRecord(InputRecord&& ir) noexcept;
		InputRecord& operator=(InputRecord&& ir) noexcept;
		InputRecord(const std::vector<TCHAR>& toggleVKeys) noexcept;

		InputHandler handler;
		std::string filename;
	};

	std::vector<InputRecord> records;
	int currentRecord;
	bool simulating;
};