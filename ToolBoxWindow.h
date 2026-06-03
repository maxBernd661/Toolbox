#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "TBCommand.h"

class ToolBoxWindow
{
public:
	bool Create();
	void Show();
	int HandleMessages();

private:
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnPaint();

	void OnKeyPress(WPARAM wParam, LPARAM lParam);
	void Flush();

	std::string_view Translate(UINT message);

private:
	HWND handle = nullptr;
	HINSTANCE instance = nullptr;
	std::wstring className = L"ToolBoxClass";

	std::vector<std::wstring> messages = { L"ToolBox v1"};
	std::vector<std::wstring> prevInput = {};
	std::wstring currentInput;
};

