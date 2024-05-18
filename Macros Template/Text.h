#pragma once
#include <tchar.h>

namespace Text {
	//Hotkey instructions
	/* Selecting Record (record must exist already):
	1. <Unique record key combination>
	*/

	/* Toggling Recording state:
	1. CTRL + F1
	*/

	/* Adding Record:
	1. CTRL + ALT + A
	2. <Unique record key combination>
	*/

	/* Deleting Record:
	1. CTRL + ALT + D
	2. <Unique record key combination>
	*/

	/* Exiting Program:
	1. CTRL + DOWN
	*/

	const TCHAR INSTRUCTIONS[] = _T("| SELECT_REC - KEY_COMBO | TOGGLE_REC - CTRL + F1 | SIM - CTRL + F2 | ADD - CTRL + ALT + A | DEL - CTRL + ALT + D | EXIT - CTRL + DOWN | ");

	const TCHAR ADDING_RECORD[] = _T("Adding Record... waiting for key combination");
	const TCHAR DELETING_RECORD[] = _T("Deleting Record... waiting for key combination");
	const TCHAR RECORDING[] = _T("Recording....");
	const TCHAR SIMUALTING_RECORD[] = _T("Simulating Record...");
	const TCHAR CURRENT_RECORD[] = _T("Current Record = ");
}