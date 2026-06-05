#include "ToolBoxWindow.h"
#include "CommandRegistry.h"
#include "Brush.h"
#include "DefaultConsoleTool.h"

#include <iostream>

bool ToolBoxWindow::Create()
{
	CommandRegistry::Instance().Build();

	activeTool = std::make_unique<DefaultConsoleTool>();

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
		nullptr,
		nullptr,
		instance,
		this
	);

	return handle != nullptr;
}

void ToolBoxWindow::Show() const
{
	ShowWindow(handle, SW_SHOW);
}

int ToolBoxWindow::HandleMessages()
{
	MSG message = {};
	while (GetMessage(&message, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

LRESULT ToolBoxWindow::StaticWndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	ToolBoxWindow* window = nullptr;

	if (message == WM_NCCREATE)
	{
		const auto createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
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

LRESULT ToolBoxWindow::WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) const
{
	std::string_view messageName = Translate(message);
	std::cout << messageName << " (" << message << ")" << '\n';

	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	if (activeTool)
	{
		LRESULT result = 0;
		if (activeTool->OnMessage(handle, message, wParam, lParam, result))
		{
			return result;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

std::string_view ToolBoxWindow::Translate(const UINT message)
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

