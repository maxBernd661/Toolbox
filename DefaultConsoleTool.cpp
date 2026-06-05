#include "DefaultConsoleTool.h"

#include <iostream>

#include "Brush.h"
#include "CommandRegistry.h"
#include "ToolBoxWindow.h"

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

bool DefaultConsoleTool::OnMessage(const HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam, LRESULT& result)
{
	switch (message)
	{
	case WM_CHAR:
		OnKeyPress(handle, wParam, lParam);
		return true;

	case WM_PAINT:
		OnPaint(handle);
		return true;

	case WM_SIZE:
		OnResize(handle, lParam);
		return true;

	default: 
		return false;
	}
}

void DefaultConsoleTool::OnPaint(const HWND handle)
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
		FONT_SIZE, 0, 0, 0,
		FW_NORMAL,
		false, false, false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN,
		L"Consolas");

	const HFONT oldFont = static_cast<HFONT>(SelectObject(context, font));

	std::span<const Message> toDraw = store.Get(availableMessages - 1);

	int y = TOP_BUFFER;

	for (auto& message : toDraw)
	{
		TextOut(context, 10, y, message.Get().c_str(), static_cast<int>(message.Get().size()));
		y += FONT_SIZE + AFTER_BUFFER;
	}

	std::wstring curString = L"> " + currentInput + L"_";
	TextOut(context, 10, y, curString.c_str(), static_cast<int>(curString.size()));

	SelectObject(context, oldFont);
	DeleteObject(font);

	EndPaint(handle, &ps);
}

void DefaultConsoleTool::OnKeyPress(const HWND handle, const WPARAM wParam, const LPARAM lParam)
{
	int keyCode = LOWORD(wParam);
	int flags = HIWORD(lParam);

	if (keyCode == VK_RETURN)
	{
		Flush();
	}
	else if (keyCode == VK_BACK)
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

	InvalidateRect(handle, nullptr, TRUE);
}

void DefaultConsoleTool::OnResize(const HWND handle, const LPARAM lParam)
{
	const int height = HIWORD(lParam);
	availableMessages = (height - TOP_BUFFER) / (FONT_SIZE + AFTER_BUFFER);
	if (availableMessages == 0)
	{
		availableMessages = 1;
	}
	std::cout << "available messages: " << availableMessages << '\n';
	InvalidateRect(handle, nullptr, FALSE);

}

void DefaultConsoleTool::Flush()
{
	if (!currentInput.empty())
	{
		store.AddUser(currentInput);

		bool found = false;
		std::vector<std::wstring> operands = Split(currentInput);
		for (const TBCommand& command : CommandRegistry::Instance().GetCommands())
		{
			std::wstring_view commandName = command.GetName();
			if (operands[0] == commandName)
			{
				found = true;
				CommandResult result = command.Execute(operands);
				for (const std::wstring& line : result)
				{
					store.AddSystem(line);
				}

				break;
			}
		}
		if (found == false)
		{
			std::wstring errorMessage = L"unknown command: '" + operands[0] + L"'. Use help to show available commands";
			store.AddSystem(errorMessage);
		}

		prevInput.push_back(currentInput);
		currentInput.clear();
	}
}
