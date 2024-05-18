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

RecordList::InputRecord& RecordList::InputRecord::operator=(RecordList::InputRecord&& ir) noexcept {
	if (this != &ir) {
		handler = std::move(ir.handler);
		filename = std::move(ir.filename);
		memset(&ir, 0, sizeof(InputRecord));
	}
	return *this;
}

RecordList::RecordList()
	:
	currentRecord(RecordList::INVALID),
	simulating(false)
{}

RecordList::~RecordList(){}

bool RecordList::Initialize(const TCHAR* workingDir) {
	auto fileList = File::GetFileList(workingDir);
	for (size_t i = 0u, size = fileList.size(); i < size; ++i) {
		records.emplace_back();
		records[i].handler.Load(fileList[i].c_str());
		records[i].filename = std::move(fileList[i]);
	}
	return true;
}

int RecordList::SelectRecord(const RAWKEYBOARD& kbd) {
	for (size_t i = 0u, size = records.size(); i < size; ++i) {
		if (records[i].handler.CheckForToggle(kbd))
			return currentRecord = static_cast<int>(i);
	}
	return RecordList::INVALID;
}

void RecordList::SimulateRecord() {
	if (currentRecord != RecordList::INVALID) {
		simulating = true;
		records[currentRecord].handler.Simulate();
		simulating = false;
	}
}

bool RecordList::AddRecord(const std::vector<TCHAR>& toggleVKeys) {
	const int index = FindRecord(toggleVKeys);
	if (index != RecordList::INVALID)
		return false;

	currentRecord = static_cast<int>(records.size());
	records.emplace_back(toggleVKeys);
	return true;
}

bool RecordList::DeleteRecord(const std::vector<TCHAR>& toggleVKeys) {
	const int index = FindRecord(toggleVKeys);
	if (index == RecordList::INVALID)
		return false;

	if(!records[index].filename.empty())
		fs::remove(records[index].filename.c_str());

	if (index != (records.size() - 1)) {
		//fs::rename(records.back().filename.c_str(), records[index].filename.c_str());
		records[index] = std::move(records.back());
	}

	records.pop_back();
	currentRecord = RecordList::INVALID;

	return true;
}

int RecordList::FindRecord(const std::vector<TCHAR>& toggleVKeys) const {
	for (size_t i = 0u, size = records.size(); i < size; ++i) {
		if (records[i].handler == toggleVKeys)
			return static_cast<int>(i);
	}
	return RecordList::INVALID;
}

const Input* RecordList::GetBack() const {
	return (currentRecord != RecordList::INVALID) ? records[currentRecord].handler.GetBack() : nullptr;
}

Input* RecordList::GetBack() {
	return (currentRecord != RecordList::INVALID) ? records[currentRecord].handler.GetBack() : nullptr;
}

bool RecordList::PopBack() {
	if (currentRecord != RecordList::INVALID) {
		if (records[currentRecord].handler.GetCount() > 0) {
			records[currentRecord].handler.PopBack();
			return true;
		}
	}

	return false;
}

std::size_t RecordList::GetCount() const {
	if (currentRecord != RecordList::INVALID)
		return records[currentRecord].handler.GetCount();

	return 0u;
}

void RecordList::Save() {
	if (currentRecord != RecordList::INVALID) {
		InputHandler& handler = records[currentRecord].handler;
		handler.Save(("Record" + handler.FormatVKeys() + ".dat").c_str());
	}
}

void RecordList::StartRecording() {
	if (currentRecord != RecordList::INVALID)
		records[currentRecord].handler.StartRecording();
}

void RecordList::StopRecording() {
	if (currentRecord != RecordList::INVALID)
		records[currentRecord].handler.StopRecording();
}

bool RecordList::IsRecording() const {
	if (currentRecord != RecordList::INVALID)
		return records[currentRecord].handler.IsRecording();
	return false;
}
bool RecordList::IsSimulating() const {
	return simulating;
}

bool RecordList::HasRecorded() const {
	if (currentRecord != RecordList::INVALID)
		return records[currentRecord].handler.HasRecorded();
	return false;
}
int RecordList::GetCurrentRecord() const {
	return currentRecord;
}