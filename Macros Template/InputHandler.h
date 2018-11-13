#pragma once
#include <vector>
#include <memory>
#include "InputData.h"
#include <variant>

class InputHandler
{
public:
	InputHandler() = default;
	InputHandler( std::vector<TCHAR> toggleVKeys );
	bool operator==(const std::vector<TCHAR>& vKeys) const;

	void Cleanup();

	template<typename T, typename... Args>
	void Add(Args&&... vals)
	{
		input.emplace_back( T{ std::forward<Args>( vals )... } );
	}
	template<typename T>
	void Add( T&& _value )
	{
		input.push_back( std::move( _value ) );
	}
	void Simulate();

	bool Load(const std::string& filename);
	bool Save(const std::string& filename);

	Input* GetBack();
	const Input* GetBack() const;

	void PopBack();

	void StartRecording();
	void StopRecording();

	bool IsRecording() const;
	bool HasRecorded() const;
	bool CheckForToggle(const RAWKEYBOARD& kbd) const;

	std::string FormatVKeys();
private:
	std::vector<TCHAR> toggleVKeys;
	std::vector<Input> inputs;
	bool recording = false;
};