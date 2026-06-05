#pragma once
#include <Windows.h>
#include <string>

#include "TBCommand.h"
#include "MessageStore.h"
#include "Tool.h"

class Tool;
constexpr int FONT_SIZE = 16;
constexpr int TOP_BUFFER = 8;
constexpr int AFTER_BUFFER = 8;

class ToolBoxWindow
{
public:

	bool Create();
	void Show() const;
	static int HandleMessages();

private:
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) const;

	static std::string_view Translate(UINT message);

	HWND handle = nullptr;
	HINSTANCE instance = nullptr;
	std::wstring className = L"ToolBoxClass";

	std::unique_ptr<Tool> activeTool;
};

