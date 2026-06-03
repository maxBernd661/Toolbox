#include "ToolBoxWindow.h"

int main() 
{
	ToolBoxWindow window;
	if (!window.Create())
	{
		return -1;
	}

	window.Show();
	return window.HandleMessages();
}
