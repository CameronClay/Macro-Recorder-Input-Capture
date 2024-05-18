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

InputHandler& InputHandler::operator=(InputHandler&& ih) noexcept {
	if (this != &ih)
	{
		toggleVKeys = std::move(ih.toggleVKeys);
		inputs = std::move(ih.inputs);
		recording = ih.recording;
		memset(&ih, 0, sizeof(InputHandler));
	}
	return *this;
}


bool InputHandler::operator==(const std::vector<TCHAR>& vKeys) const {
	if (vKeys.size() != toggleVKeys.size())
		return false;

	for (std::size_t i = 0u, size = toggleVKeys.size(); i < size; ++i) {
		if (vKeys[i] != toggleVKeys[i])
			return false;
	}
	return true;
}

InputHandler::~InputHandler() {
	Cleanup();
}

void InputHandler::Cleanup() {
	inputs.clear();
}

void InputHandler::Simulate() {
	StopRecording();

	for (const auto& it : inputs)
		it.Simulate();
}

bool InputHandler::Load(const char* filename) {
	std::ifstream stream;
	stream.open(filename, std::ifstream::in | std::ifstream::binary);
	if (!stream.is_open() || stream.fail())
		return false;

	int nKeys;
	stream.read((char*)&nKeys, sizeof(int));
	if (stream.fail())
		return false;

	for (int i = 0; i < nKeys; i++) {
		TCHAR key;
		stream.read((char*)&key, sizeof(TCHAR));
		if (stream.fail())
			return false;

		toggleVKeys.push_back(key);
	}

	auto make_input = [&stream](int uuid) {
		switch (uuid)
		{
		case DelayData::uuid:
			return Input(DelayData{ stream });
		case MouseClickData::uuid:
			return Input(MouseClickData{ stream });
		case MouseXClickData::uuid:
			return Input(MouseXClickData{ stream });
		case MouseMoveData::uuid:
			return Input(MouseMoveData{ stream });
		case MouseScrollData::uuid:
			return Input(MouseScrollData{ stream });
		case KbdData::uuid:
			return Input(KbdData{ stream });
		default:
			return Input();
		}
	};

	while (!stream.eof()) {
		int uuid;
		stream.read((char*)&uuid, sizeof(int));
		if (stream.fail())
			return false;

		if ((uuid < 0) || (uuid > 5))
			return false;

		Add(make_input(uuid));
	}

	return true;
}
bool InputHandler::Save(const char* filename) {
	StopRecording();
	if (!HasRecorded())
		return false;

	std::ofstream stream;
	stream.open(filename, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
	if (!stream.is_open() || stream.fail())
		return false;

	const std::size_t nKeys = toggleVKeys.size();
	stream.write((char*)&nKeys, sizeof(int));

	for (const auto& it : toggleVKeys) {
		stream.write((char*)&it, sizeof(TCHAR));
		if (stream.fail())
			return false;
	}

	for (const auto& it : inputs) {
		it.SaveData(stream);
		if (stream.fail()) {
			stream.close();
			return false;
		}
	}

	stream.close();

	return true;
}

Input* InputHandler::GetBack() const {
	return (Input*)(!inputs.empty() ? &inputs.back() : nullptr);
}

std::size_t InputHandler::GetCount() const {
	return inputs.size();
}

void InputHandler::PopBack() {
	inputs.pop_back();
}

void InputHandler::StartRecording() {
	Cleanup();
	recording = true;
}
void InputHandler::StopRecording() {
	recording = false;
}

bool InputHandler::IsRecording() const {
	return recording;
}
bool InputHandler::HasRecorded() const {
	return !inputs.empty();
}
bool InputHandler::CheckForToggle(const RAWKEYBOARD& kbd) const {
	return CheckKey::VKComboDown(kbd, toggleVKeys);
}

std::string InputHandler::FormatVKeys() const {
	std::stringstream stream;
	for (const auto c : toggleVKeys)
		stream << int(c) << '+';

	std::string str = stream.str();
	str.pop_back();

	return str;
}
