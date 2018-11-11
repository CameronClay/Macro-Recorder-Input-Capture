#include "RecordList.h"
#include "File.h"

RecordList::InputRecord::InputRecord(const std::vector<TCHAR>& toggleVKeys) noexcept
	:
	handler(toggleVKeys)
{}

RecordList::InputRecord::InputRecord(InputRecord&& ir) noexcept
	:
	handler(std::move(ir.handler)),
	filename(std::move(ir.filename))
{
	memset(&ir, 0, sizeof(InputRecord));
}

RecordList::InputRecord& RecordList::InputRecord::operator=(RecordList::InputRecord&& ir) noexcept
{
	if (this != &ir)
	{
		handler = std::move(ir.handler);
		filename = std::move(ir.filename);
		memset(&ir, 0, sizeof(InputRecord));
	}
	return *this;
}

RecordList::RecordList()
	:
	currentRecord(RecordList::INVALIDRECORD),
	simulating(false)
{}

RecordList::~RecordList(){}

bool RecordList::Initialize(const TCHAR* workingDir)
{
	auto fileList = File::GetFileNameList(workingDir);
	for (size_t i = 0, size = fileList.size(); i < size; ++i)
	{
		records.emplace_back();
		records[i].handler.Load(fileList[i].c_str());
		records[i].filename = std::move(fileList[i]);
	}
	return true;
}

int RecordList::SelectRecord(const RAWKEYBOARD& kbd)
{
	for (size_t i = 0, size = records.size(); i < size; ++i)
	{
		if (records[i].handler.CheckForToggle(kbd))
		{
			return currentRecord = i;
		}
	}
	return RecordList::INVALIDRECORD;
}

void RecordList::SimulateRecord()
{
	if (currentRecord != RecordList::INVALIDRECORD)
	{
		simulating = true;
		records[currentRecord].handler.Simulate();
		simulating = false;
	}
}

bool RecordList::AddRecord(const std::vector<TCHAR>& toggleVKeys)
{
	const int index = FindRecord(toggleVKeys);
	if (index != -1)
		return false;

	currentRecord = records.size();
	records.emplace_back(toggleVKeys);
	return true;
}

bool RecordList::DeleteRecord(const std::vector<TCHAR>& toggleVKeys)
{
	const int index = FindRecord(toggleVKeys);
	if (index == -1)
		return false;

	remove(records[index].filename.c_str());

	if (index != (records.size() - 1))
	{
		rename(records.back().filename.c_str(), records[index].filename.c_str());
		records[index] = std::move(records.back());
	}

	records.pop_back();

	if (records.empty())
		currentRecord = RecordList::INVALIDRECORD;
	else
		currentRecord = 0;

	return true;
}

int RecordList::FindRecord(const std::vector<TCHAR>& toggleVKeys) const
{
	for (size_t i = 0, size = records.size(); i < size; ++i)
	{
		if (records[i].handler == toggleVKeys)
			return i;
	}
	return -1;
}

InputData* RecordList::GetBack() const
{
	return (currentRecord != RecordList::INVALIDRECORD) ? records[currentRecord].handler.GetBack() : nullptr;
}

void RecordList::PopBack()
{
	if (currentRecord != RecordList::INVALIDRECORD)
		records[currentRecord].handler.PopBack();
}

void RecordList::Save()
{
	if (currentRecord != RecordList::INVALIDRECORD)
	{
		InputHandler& handler = records[currentRecord].handler;
		handler.Save(("Record" + handler.FormatVKeys() + ".dat").c_str());
	}
}

void RecordList::StartRecording()
{
	if (currentRecord != RecordList::INVALIDRECORD)
		records[currentRecord].handler.StartRecording();
}

void RecordList::StopRecording()
{
	if (currentRecord != RecordList::INVALIDRECORD)
		records[currentRecord].handler.StopRecording();
}

bool RecordList::IsRecording() const
{
	if (currentRecord != RecordList::INVALIDRECORD)
		return records[currentRecord].handler.IsRecording();
	return false;
}
bool RecordList::IsSimulating() const
{
	return simulating;
}

bool RecordList::HasRecorded() const
{
	if (currentRecord != RecordList::INVALIDRECORD)
		return records[currentRecord].handler.HasRecorded();
	return false;
}

int RecordList::GetCurrentRecord() const
{
	return currentRecord;
}