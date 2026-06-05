#pragma once

#include <windows.h>

class Tool
{
public:
	virtual ~Tool() = default;
	virtual bool OnMessage(HWND handle, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result) = 0;
};

