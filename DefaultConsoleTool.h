#pragma once
#include "MessageStore.h"
#include "Tool.h"

class DefaultConsoleTool : public Tool
{
public:
	bool OnMessage( HWND handle, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result) override;


private:
	void OnPaint(HWND handle);
	void OnKeyPress(HWND handle,WPARAM wParam, LPARAM lParam);
	void OnResize(HWND handle, LPARAM lParam);
	void Flush();

	MessageStore store = {};
	std::vector<std::wstring> prevInput = {};
	std::wstring currentInput;
	int availableMessages = 1;
};
