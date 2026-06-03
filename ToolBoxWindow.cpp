#include "ToolBoxWindow.h"
#include "CommandRegistry.h"
#include "Brush.h"
#include <iostream>
#include <sstream>
#include <format>

static std::vector<std::wstring> Split(const std::wstring& str)
{
	std::vector<std::wstring> result;
	std::wstringstream stream(str);

	std::wstring part;
	while (stream >> part)
	{
		result.push_back(part);
	}

	return result;
}

bool ToolBoxWindow::Create()
{
	CommandRegistry::Instance().Build();

	instance = GetModuleHandle(nullptr);
	className = L"ToolBoxClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = StaticWndProc;
	wc.hInstance = instance;
	wc.lpszClassName = className.c_str();

	RegisterClass(&wc);

	handle = CreateWindowEx(
		0,
		className.c_str(),
		L"ToolBox",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		instance,
		this
	);

	return handle != nullptr;
}

void ToolBoxWindow::Show()
{
	ShowWindow(handle, SW_SHOW);
}

int ToolBoxWindow::HandleMessages()
{
	MSG message = {};
	while (GetMessage(&message, NULL, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

LRESULT ToolBoxWindow::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ToolBoxWindow* window = nullptr;

	if (message == WM_NCCREATE)
	{
		auto createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		window = static_cast<ToolBoxWindow*>(createStruct->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
	}
	else
	{
		window = reinterpret_cast<ToolBoxWindow*>(
			GetWindowLongPtr(hWnd, GWLP_USERDATA)
			);
	}

	if (window)
		return window->WndProc(hWnd, message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT ToolBoxWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::string_view messageName = Translate(message);
	std::cout << messageName << " (" << message << ")" << std::endl;
	switch (message)
	{
	case WM_CHAR:
		OnKeyPress(wParam, lParam);
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void ToolBoxWindow::OnPaint()
{
	PAINTSTRUCT ps;
	HDC context = BeginPaint(handle, &ps);

	RECT rect;
	GetClientRect(handle, &rect);

	Brush bgBrush(RGB(0, 0, 0));
	FillRect(context, &ps.rcPaint, bgBrush);

	SetBkMode(context, TRANSPARENT);
	SetTextColor(context, RGB(255, 255, 255));

	HFONT font = CreateFont(
	16, 0, 0, 0,
	FW_NORMAL,
	false, false, false,
	DEFAULT_CHARSET,
	OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS,
	DEFAULT_QUALITY,
	FIXED_PITCH || FF_MODERN,
	L"Consolas");

	HFONT oldFont = static_cast<HFONT>(SelectObject(context, font));

	int y = 8;
	for(const std::wstring& message : messages)
	{
		TextOut(context, 10, y, message.c_str(), static_cast<int>(message.size()));
		y += 18;
	}

	std::wstring curString = L"> " + currentInput + L"_";
	TextOut(context, 10, y, curString.c_str(), static_cast<int>(curString.size()));

	SelectObject(context, oldFont);
	DeleteObject(font);

	EndPaint(handle, &ps);
}

void ToolBoxWindow::OnKeyPress(WPARAM wParam, LPARAM lParam)
{
	int keyCode = LOWORD(wParam);
	int flags = HIWORD(lParam);

	if (keyCode == VK_RETURN)
	{
		Flush();
	}
	else if(keyCode == VK_BACK)
	{
		if (!currentInput.empty())
		{
			currentInput.pop_back();
		}
	}
	else
	{
		currentInput += static_cast<wchar_t>(wParam);
	}

	InvalidateRect(handle, NULL, TRUE);
}

void ToolBoxWindow::Flush()
{
	if (!currentInput.empty())
	{
		messages.push_back(L"> " + currentInput);

		bool found = false;
		std::vector<std::wstring> operands = Split(currentInput);
		for(const TBCommand& command : CommandRegistry::Instance().GetCommands())
		{
			std::wstring_view commandName = command.GetName();
			if (operands[0] == commandName)
			{
				found = true;
				CommandResult result = command.Execute(operands);
				for (const std::wstring& line : result)
				{
					messages.push_back(line);
				}

				break;
			}
		}
		if (found == false)
		{
			std::wstring errorMessage = std::format(L"unknown command: '{}'. Use help to show available commands", operands[0]);
			messages.push_back(errorMessage);
		}

		prevInput.push_back(currentInput);
		currentInput.clear();
	}
}

std::string_view ToolBoxWindow::Translate(UINT message)
{
	switch (message)
	{
	case WM_NULL: return "WM_NULL";
	case WM_CREATE: return "WM_CREATE";
	case WM_DESTROY: return "WM_DESTROY";
	case WM_MOVE: return "WM_MOVE";
	case WM_SIZE: return "WM_SIZE";
	case WM_ACTIVATE: return "WM_ACTIVATE";
	case WM_SETFOCUS: return "WM_SETFOCUS";
	case WM_KILLFOCUS: return "WM_KILLFOCUS";
	case WM_ENABLE: return "WM_ENABLE";
	case WM_SETREDRAW: return "WM_SETREDRAW";
	case WM_SETTEXT: return "WM_SETTEXT";
	case WM_GETTEXT: return "WM_GETTEXT";
	case WM_GETTEXTLENGTH: return "WM_GETTEXTLENGTH";
	case WM_PAINT: return "WM_PAINT";
	case WM_CLOSE: return "WM_CLOSE";
	case WM_QUERYENDSESSION: return "WM_QUERYENDSESSION";
	case WM_QUIT: return "WM_QUIT";
	case WM_ERASEBKGND: return "WM_ERASEBKGND";
	case WM_SHOWWINDOW: return "WM_SHOWWINDOW";
	case WM_SETCURSOR: return "WM_SETCURSOR";
	case WM_MOUSEMOVE: return "WM_MOUSEMOVE";
	case WM_LBUTTONDOWN: return "WM_LBUTTONDOWN";
	case WM_LBUTTONUP: return "WM_LBUTTONUP";
	case WM_LBUTTONDBLCLK: return "WM_LBUTTONDBLCLK";
	case WM_RBUTTONDOWN: return "WM_RBUTTONDOWN";
	case WM_RBUTTONUP: return "WM_RBUTTONUP";
	case WM_RBUTTONDBLCLK: return "WM_RBUTTONDBLCLK";
	case WM_MBUTTONDOWN: return "WM_MBUTTONDOWN";
	case WM_MBUTTONUP: return "WM_MBUTTONUP";
	case WM_MOUSEWHEEL: return "WM_MOUSEWHEEL";
	case WM_KEYDOWN: return "WM_KEYDOWN";
	case WM_KEYUP: return "WM_KEYUP";
	case WM_CHAR: return "WM_CHAR";
	case WM_SYSKEYDOWN: return "WM_SYSKEYDOWN";
	case WM_SYSKEYUP: return "WM_SYSKEYUP";
	case WM_COMMAND: return "WM_COMMAND";
	case WM_TIMER: return "WM_TIMER";
	case WM_HSCROLL: return "WM_HSCROLL";
	case WM_VSCROLL: return "WM_VSCROLL";
	case WM_NCHITTEST: return "WM_NCHITTEST";
	case WM_WINDOWPOSCHANGED: return "WM_WINDOWPOSCHANGED";
	case WM_WINDOWPOSCHANGING: return "WM_WINDOWPOSCHANGING";
	default: return "UNKNOWN";
	}
}

