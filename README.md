# Macro Recorder (Input-Capture)
Program able to record, capture, and simulate keyboard and mouse input with a text based overlapped-interface. Utilizes raw input; raw input is raw device input before it is modified by windows. Using raw input makes clicks and keyboard presses feel more responsive and smooth compared to standard input.

# Controls
```
Selecting Record (record must exist already):
1. <Unique record key combination>

Toggling Recording state:
1. CTRL + F1

Adding Record:
1. CTRL + ALT + A
2. <Unique record key combination>

Deleting Record:
1. CTRL + ALT + D
2. <Unique record key combination>

Exiting Program:
1. CTRL + DOWN
```
Menu and controls are also displayed on the overlapped menu.

## Unit Testing
- Unit testing is done using Google Test.

## Tooling
- C++
- Win32 API
- Google Test
- Visual Studio
