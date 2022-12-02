#pragma once
#include <conio.h>
#include "Utils.h"
#include <fstream>

enum class KEY_STATE
{
	NONE = 0,
	TAP,
	HOLD,
	AWAY
};

typedef KEY_STATE MOUSE_STATE;

struct keyInfo
{
	KEY_STATE	state;
	bool		bPrev;
};

typedef keyInfo mouseInfo;

class Input
{
	keyInfo keys[256];
	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	INPUT_RECORD irInBuf[128];

	mouseInfo mouse[2];

	COORD mousePosition;

private:
	VOID ErrorExit(LPCSTR lpszMessage);
	VOID KeyEventProc(KEY_EVENT_RECORD ker);
	VOID MouseEventProc(MOUSE_EVENT_RECORD mer);

public:
	void readEveryFrame() {
		DWORD cNumRead = 0;

		Borland::GotoXY(0, 25);
		if (GetNumberOfConsoleInputEvents(hStdin, &cNumRead) == 0)
		{
			printf("Input Reading Failure %d\n", GetLastError());
			return;
		}

		for (int i = 0; i < 256; i++) {
			switch (keys[i].state) {
			case KEY_STATE::AWAY:
				keys[i].state = KEY_STATE::NONE;
				break;
			case KEY_STATE::TAP:
				keys[i].state = KEY_STATE::HOLD;
				break;
			}
		}
		for (int i = 0; i < 2; i++) {
			switch (mouse[i].state) {
			case MOUSE_STATE::AWAY:
				mouse[i].state = MOUSE_STATE::NONE;
				break;
			case MOUSE_STATE::TAP:
				mouse[i].state = MOUSE_STATE::HOLD;
				break;
			}
		}

		if (cNumRead == 0) {
			return;
		}

		if (!ReadConsoleInput(
			hStdin,      // input buffer handle
			irInBuf,     // buffer to read into
			128,         // size of read buffer
			&cNumRead)) // number of records read
			ErrorExit("ReadConsoleInput");

		for (int i = 0; i < (int)cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input
				KeyEventProc(irInBuf[i].Event.KeyEvent);
				break;

			case MOUSE_EVENT: // mouse input
				MouseEventProc(irInBuf[i].Event.MouseEvent);
				break;
			}
		}
	}

	auto getKeyDown(WORD keyCode) const
	{
		return keys[keyCode & 0x00ff].state == KEY_STATE::TAP;
	}

	auto getKey(WORD keyCode) const
	{
		printf("key state stay [%x]\n", keys[keyCode & 0x00ff].state);
		return keys[keyCode & 0x00ff].state == KEY_STATE::TAP
			|| keys[keyCode & 0x00ff].state == KEY_STATE::HOLD
			|| keys[keyCode & 0x00ff].state == KEY_STATE::AWAY;
	}

	auto getKeyUp(WORD keyCode) const
	{
		printf("key state up [%x]\n", keys[keyCode & 0x00ff].state);
		return keys[keyCode & 0x00ff].state == KEY_STATE::AWAY;
	}

	auto getMouseButton(int button) const
	{
		printf("mouse state [%x]\n", mouse[button].state);
		return mouse[button].state == MOUSE_STATE::TAP
			|| mouse[button].state == MOUSE_STATE::HOLD
			|| mouse[button].state == MOUSE_STATE::AWAY;
	}
	auto getMouseButtonDown(int button) const
	{
		printf("mouse state down [%x]\n", mouse[button].state);
		return mouse[button].state == MOUSE_STATE::TAP;
	}
	auto getMouseButtonUp(int button) const
	{
		printf("mouse state up [%x]\n", mouse[button].state);
		return mouse[button].state == MOUSE_STATE::AWAY;
	}

	Position getMousePosition() const
	{
		return { mousePosition.X, mousePosition.Y };
	}


public:
	Input();
	virtual ~Input();
};