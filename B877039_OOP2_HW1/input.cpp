#include "input.h"

VOID Input::ErrorExit(LPCSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

VOID Input::KeyEventProc(KEY_EVENT_RECORD ker)
{
	Borland::GotoXY(0, 26);
	printf("Key event: ");

	switch (keys[ker.wVirtualKeyCode & 0x00ff].state)
	{
	case KEY_STATE::NONE:
		keys[ker.wVirtualKeyCode].state = KEY_STATE::TAP;
		break;
	case KEY_STATE::TAP:
		keys[ker.wVirtualKeyCode].state = KEY_STATE::HOLD;
		break;
	case KEY_STATE::HOLD:
		keys[ker.wVirtualKeyCode].state = KEY_STATE::AWAY;
		break;
	case KEY_STATE::AWAY:
		keys[ker.wVirtualKeyCode].state = KEY_STATE::NONE;
		break;
	}
}

VOID Input::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif

	switch (mer.dwEventFlags)
	{
	case 0:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			switch (mouse[0].state) {
			case MOUSE_STATE::NONE:
				mouse[0].state = MOUSE_STATE::TAP;
				mousePosition = mer.dwMousePosition;
				break;
			case MOUSE_STATE::TAP:
				mouse[0].state = MOUSE_STATE::HOLD;
				mousePosition = mer.dwMousePosition;
				break;
			}
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			switch (mouse[1].state) {
			case MOUSE_STATE::NONE:
				mouse[1].state = MOUSE_STATE::TAP;
				mousePosition = mer.dwMousePosition;
				break;
			case MOUSE_STATE::TAP:
				mouse[1].state = MOUSE_STATE::HOLD;
				mousePosition = mer.dwMousePosition;
				break;
			}
			mousePosition = mer.dwMousePosition;
		}
		else
		{
			for (int i = 0; i < 2; i++)
			{
				switch (mouse[i].state) {
				case MOUSE_STATE::HOLD:
					mouse[i].state = MOUSE_STATE::AWAY;
					mousePosition = mer.dwMousePosition;
					break;
				}
			}
		}
		break;
	case DOUBLE_CLICK:
		printf("double click\n");
		break;
	}
}

Input::Input()
{
	DWORD fdwMode;

	for (int i = 0; i < 256; i++)
	{
		keys[i].state = KEY_STATE::NONE;
	}
	mouse[0].state = MOUSE_STATE::NONE;
	mouse[1].state = MOUSE_STATE::NONE;
	mousePosition = { 0, 0 };

	memset(irInBuf, 0, sizeof(INPUT_RECORD) * 128);

	// Get the standard input handle.

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");

	// Save the current input mode, to be restored on exit.

	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode");

	// Enable the window and mouse input events.

	fdwMode = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT) & ~ENABLE_QUICK_EDIT_MODE;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");
}

Input::~Input()
{
	SetConsoleMode(hStdin, fdwSaveOldMode);
}
