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
	currentRecord(RecordList::INVALID),
	simulating(false)
{}

RecordList::~RecordList(){}

bool RecordList::Initialize(const TCHAR* workingDir)
{
	auto fileList = File::GetFileList(workingDir);
	for( auto& file : fileList )
	{
		auto& record = records.emplace_back();
		record.handler.Load( file );
		record.filename = std::move( file );
	}
	return true;
}

int RecordList::SelectRecord(const RAWKEYBOARD& kbd)
{
	auto find_record = [ &kbd ]( const RecordList::InputRecord& _record )
	{
		return _record.handler.CheckForToggle( kbd );
	}; 

	if( auto it = std::find_if( records.begin(), records.end(), find_record );
		it != records.end() )
	{
		return currentRecord = ( it - records.begin() );
	}
	else
	{
		return RecordList::INVALID;
	}
}

void RecordList::SimulateRecord()
{
	if (currentRecord != RecordList::INVALID)
	{
		simulating = true;
		records[currentRecord].handler.Simulate();
		simulating = false;
	}
}

bool RecordList::AddRecord(const std::vector<TCHAR>& toggleVKeys)
{
	const int index = FindRecord(toggleVKeys);
	if (index != RecordList::INVALID)
		return false;

	currentRecord = records.size();
	records.emplace_back(toggleVKeys);
	return true;
}

bool RecordList::DeleteRecord(const std::vector<TCHAR>& toggleVKeys)
{
	const int index = FindRecord(toggleVKeys);
	if (index == RecordList::INVALID)
		return false;

	fs::remove(records[index].filename);

	if (index != (records.size() - 1))
	{
		fs::rename(records.back().filename, records[index].filename);
		records[index] = std::move(records.back());
	}

	records.pop_back();
	currentRecord = RecordList::INVALID;

	return true;
}

int RecordList::FindRecord(const std::vector<TCHAR>& toggleVKeys) const
{
	auto it = std::find_if(
		records.begin(),
		records.end(),
		[ & ]( const auto& _record )
	{
		return _record.handler == toggleVKeys;
	} );

	return ( it != records.end() ) ?
		it - records.begin() : RecordList::INVALID;
}

const Input* RecordList::GetBack() const
{
	return ( currentRecord != RecordList::INVALID ) ?
		records[ currentRecord ].handler.GetBack() : nullptr;
}
Input* RecordList::GetBack()
{
	return ( currentRecord != RecordList::INVALID ) ?
		records[ currentRecord ].handler.GetBack() : nullptr;
}

void RecordList::PopBack()
{
	if (currentRecord != RecordList::INVALID)
		records[currentRecord].handler.PopBack();
}

void RecordList::Save()
{
	if (currentRecord != RecordList::INVALID)
	{
		InputHandler& handler = records[currentRecord].handler;
		handler.Save(("Record" + handler.FormatVKeys() + ".dat").c_str());
	}
}

void RecordList::StartRecording()
{
	if (currentRecord != RecordList::INVALID)
		records[currentRecord].handler.StartRecording();
}

void RecordList::StopRecording()
{
	if (currentRecord != RecordList::INVALID)
		records[currentRecord].handler.StopRecording();
}

bool RecordList::IsRecording() const
{
	if (currentRecord != RecordList::INVALID)
		return records[currentRecord].handler.IsRecording();
	return false;
}
bool RecordList::IsSimulating() const
{
	return simulating;
}

bool RecordList::HasRecorded() const
{
	if (currentRecord != RecordList::INVALID)
		return records[currentRecord].handler.HasRecorded();
	return false;
}

int RecordList::GetCurrentRecord() const
{
	return currentRecord;
}