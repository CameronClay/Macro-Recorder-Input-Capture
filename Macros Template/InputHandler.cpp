#include "InputHandler.h"
#include "CheckKey.h"
#include <sstream>


InputHandler::InputHandler( std::vector<TCHAR> toggleVKeys )
	:
	toggleVKeys( std::move( toggleVKeys ) )
{}


bool InputHandler::operator==(const std::vector<TCHAR>& vKeys) const
{
	if (vKeys.size() != toggleVKeys.size())
		return false;

	for( int i = 0; i < int( toggleVKeys.size() ); ++i )
	{
		if (vKeys[i] != toggleVKeys[i])
			return false;
	}
	return true;
}

void InputHandler::Cleanup()
{
	inputs.clear();
}

void InputHandler::Simulate()
{
	StopRecording();
	for( const auto& input : inputs )
		input.Simulate();
}

bool InputHandler::Load(const std::string& filename)
{
	std::ifstream stream(
		filename, std::ifstream::in | std::ifstream::binary);
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

	auto make_input = [ &stream ](int uuid)
	{
		switch( uuid )
		{
			case DelayData::uuid:
				return Input( DelayData{ stream } );
			case MouseClickData::uuid:
				return Input( MouseClickData{ stream } );
			case MouseXClickData::uuid:
				return Input( MouseXClickData{ stream } );
			case MouseMoveData::uuid:
				return Input( MouseMoveData{ stream } );
			case MouseScrollData::uuid:
				return Input( MouseScrollData{ stream } );
			case KbdData::uuid:
				return Input( KbdData{ stream } );
			default:
				throw std::runtime_error( "Never throws, handled before call." );
		}
	};

	while (!stream.eof())
	{
		int uuid;
		stream.read((char*)&uuid, sizeof(int));
		if (stream.fail())
			return false;

		if( uuid < 0 || uuid > 5 )
			return false;
		
		Add( make_input( uuid ) );
	}

	return true;
}
bool InputHandler::Save(const std::string& filename)
{
	StopRecording();

	if (!HasRecorded())
		return false;

	std::ofstream stream(filename, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
	if (!stream.is_open() || stream.fail())
		return false;

	const int nKeys = int( toggleVKeys.size() );
	stream.write((const char*)&nKeys, sizeof(int));

	stream.write( 
		reinterpret_cast< const char* >( toggleVKeys.data() ), 
		sizeof( TCHAR ) * toggleVKeys.size() );

	if( stream.fail() )
		return false;

	for( const auto& data : inputs )
	{
		data.SaveData( stream );
		if( stream.fail() )
		{
			stream.close();
			return false;
		}
	}

	stream.close();

	return true;
}

Input* InputHandler::GetBack()
{
	if( inputs.empty() )
		return nullptr;

	return &inputs.back();
}
const Input* InputHandler::GetBack() const
{
	if( inputs.empty() )
		return nullptr;

	return &inputs.back();
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

	for( const auto c : toggleVKeys )
	{
		stream << int( c ) << '+';
	}
	std::string str = stream.str();
	str.pop_back();
	return str;
}
