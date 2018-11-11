#pragma once
#include <vector>
#include <memory>
#include "InputData.h"
#include <variant>

class InputHandler
{
public:
	InputHandler();
	InputHandler(const std::vector<TCHAR>& toggleVKeys);
	InputHandler(InputHandler&& ih) noexcept;
	InputHandler& operator=(InputHandler&& ih) noexcept;
	bool operator==(const std::vector<TCHAR>& vKeys) const;

	~InputHandler();

	void Cleanup();

	template<typename T, typename... Args>
	void Add(Args&&... vals)
	{
		inputs.push_back(T{std::forward<Args>(vals)... });
	}

	void Simulate();

	bool Load(const char* filename);
	bool Save(const char* filename);

	InputData* GetBack() const;
	void PopBack();

	void StartRecording();
	void StopRecording();

	bool IsRecording() const;
	bool HasRecorded() const;
	bool CheckForToggle(const RAWKEYBOARD& kbd) const;

	std::string FormatVKeys();
private:
	std::vector<TCHAR> toggleVKeys;
	std::vector<InputData> inputs;
	bool recording;
};