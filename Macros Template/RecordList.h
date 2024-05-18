#pragma once
#include "InputHandler.h"
#include <string>

class RecordList
{
public:
	static constexpr int INVALID = -1;

	RecordList();	
	~RecordList();

	//Adds key or mouse event to currently selected record
	template<typename T, typename... Args>
	void AddEventToRecord(Args&&... vals) {
		records[currentRecord].handler.Add<T, Args...>(std::forward<Args>(vals)...);
	}

	bool Initialize(const TCHAR* workingDir);

	//Select record if record is found matching keys down in kbd
	//If no record is found returns INVALID
	int SelectRecord(const RAWKEYBOARD& kbd);

	//Add record or select record based on unique toggleVKeys if already exists
	//Returns true if record was added and false if an existing record was set as active
	bool AddRecord(const std::vector<TCHAR>& toggleVKeys);

	//Returns true if a record deleted with matching toogleVKeys, and false if no record was found
	bool DeleteRecord(const std::vector<TCHAR>& toggleVKeys);

	void SimulateRecord();

	std::size_t GetCount() const;

	const Input* GetBack() const;
	Input* GetBack();

	//Removes last item in recordlist
	//Returns true if removed, and false if failed
	bool PopBack();

	void Save();

	void StartRecording();
	void StopRecording();

	bool IsRecording() const;
	bool IsSimulating() const;
	bool HasRecorded() const;

	//Get currently selected record, returns INVALID if no record is selected
	int GetCurrentRecord() const;
private:
	//Find record matching toggleVKeys, returns INVALID if no record was found
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