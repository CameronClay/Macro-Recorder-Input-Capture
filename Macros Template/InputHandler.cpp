#include "InputHandler.h"
#include "CheckKey.h"
#include <sstream>

InputHandler::InputHandler()
	:
	recording(false)
{}

InputHandler::InputHandler(const std::vector<TCHAR>& toggleVKeys)
	:
	toggleVKeys(toggleVKeys),
	recording(false)
{}

InputHandler::InputHandler(InputHandler&& ih) noexcept
	:
	toggleVKeys(std::move(ih.toggleVKeys)),
	inputs(std::move(ih.inputs)),
	recording(ih.recording)
{
	memset(&ih, 0, sizeof(InputHandler));
}

InputHandler& InputHandler::operator=(InputHandler&& ih) noexcept
{
	if (this != &ih)
	{
		toggleVKeys = std::move(ih.toggleVKeys);
		inputs = std::move(ih.inputs);
		recording = ih.recording;
		memset(&ih, 0, sizeof(InputHandler));
	}
	return *this;
}


bool InputHandler::operator==(const std::vector<TCHAR>& vKeys) const
{
	if (vKeys.size() != toggleVKeys.size())
		return false;

	for (int i = 0, size = toggleVKeys.size(); i < size; ++i)
	{
		if (vKeys[i] != toggleVKeys[i])
			return false;
	}
	return true;
}

InputHandler::~InputHandler()
{
	Cleanup();
}

void InputHandler::Cleanup()
{
	inputs.clear();
}

void InputHandler::Simulate()
{
	StopRecording();

	for (auto& it : inputs)
		std::visit(SimulateVisitor{}, it);
}

bool InputHandler::Load(const char* filename)
{
	std::ifstream stream;
	stream.open(filename, std::ifstream::in | std::ifstream::binary);
	if (!stream.is_open() || stream.fail())
		return false;

	int nKeys;
	stream.read((char*)&nKeys, sizeof(int));
	if (stream.fail())
		return false;

	for (int i = 0; i < nKeys; i++)
	{
		TCHAR key;
		stream.read((char*)&key, sizeof(TCHAR));
		if (stream.fail())
			return false;

		toggleVKeys.push_back(key);
	}


	while (!stream.eof())
	{
		int uuid;
		stream.read((char*)&uuid, sizeof(int));
		if (stream.fail())
			return false;

		InputData dat;
		switch (uuid)
		{
		case DelayData::uuid:
			dat.emplace<DelayData>();
			break;
		case MouseClickData::uuid:
			dat.emplace<MouseClickData>();
			break;
		case MouseXClickData::uuid:
			dat.emplace<MouseXClickData>();
			break;
		case MouseMoveData::uuid:
			dat.emplace<MouseMoveData>();
			break;
		case MouseScrollData::uuid:
			dat.emplace<MouseScrollData>();
			break;
		case KbdData::uuid:
			dat.emplace<KbdData>();
			break;
		default:
			return false;
		}
		std::visit(ReadVisitor{ stream }, dat);
		Add<InputData>(dat);
	}

	return true;
}
bool InputHandler::Save(const char* filename)
{
	StopRecording();
	if (!HasRecorded())
		return false;

	std::ofstream stream;
	stream.open(filename, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
	if (!stream.is_open() || stream.fail())
		return false;

	int nKeys = toggleVKeys.size();
	stream.write((char*)&nKeys, sizeof(int));

	for (auto& it : toggleVKeys)
	{
		stream.write((char*)&it, sizeof(TCHAR));
		if (stream.fail())
			return false;
	}

	for (auto& it : inputs)
	{
		std::visit(SaveVisitor{stream}, it);
		if (stream.fail())
		{
			stream.close();
			return false;
		}
	}

	stream.close();

	return true;
}

InputData* InputHandler::GetBack() const
{
	return (InputData*)(!inputs.empty() ? &inputs.back() : nullptr);
}

void InputHandler::PopBack()
{
	inputs.pop_back();
}

void InputHandler::StartRecording()
{
	Cleanup();
	recording = true;
}
void InputHandler::StopRecording()
{
	recording = false;
}

bool InputHandler::IsRecording() const
{
	return recording;
}
bool InputHandler::HasRecorded() const
{
	return !inputs.empty();
}
bool InputHandler::CheckForToggle(const RAWKEYBOARD& kbd) const
{
	return CheckKey::VKComboDown(kbd, toggleVKeys);
}

std::string InputHandler::FormatVKeys()
{
	std::stringstream stream;
	for (uint32_t i = 0, size = toggleVKeys.size(); i < size; i++)
	{
		stream << (int)toggleVKeys[i];
		if (i != size - 1)
			stream << '+';
	}
	return stream.str();
}
