#pragma once
#include <vector>
#include <memory>
#include "InputData.h"

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
	void Add(Args&&... vals) {
		inputs.emplace_back(T{ std::forward<Args>(vals)... });
	}

	template<typename T>
	void Add(T&& val) {
		inputs.push_back(std::move(val));
	}

	void Simulate();

	bool Load(const char* filename);
	bool Save(const char* filename);

	std::size_t GetCount() const;
	Input* GetBack() const;
	void PopBack();

	void StartRecording();
	void StopRecording();

	bool IsRecording() const;
	bool HasRecorded() const;
	bool CheckForToggle(const RAWKEYBOARD& kbd) const;

	std::string FormatVKeys() const;
private:
	std::vector<TCHAR> toggleVKeys;
	std::vector<Input> inputs;
	bool recording;
};