#pragma once
#include <windef.h>
#include <wingdi.h>
class Brush
{
public:
	Brush(COLORREF color): handle(CreateSolidBrush(color))
	{
	}

	~Brush()
	{
		if (handle) {
			DeleteObject(handle);
		}
	}

	operator HBRUSH() const
	{
		return handle;
	}

private:
	HBRUSH handle;
};

