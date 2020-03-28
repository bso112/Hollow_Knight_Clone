
#include "stdafx.h"

void Draw_Rect(HDC _DC, const RECT& _rc, int _iScrollX, int _iScrollY)
{
	HPEN  myPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(_DC, myPen);

	MoveToEx(_DC, _rc.left + _iScrollX, _rc.top + _iScrollY, nullptr);
	LineTo(_DC, _rc.right + _iScrollX, _rc.top + _iScrollY);
	LineTo(_DC, _rc.right + _iScrollX, _rc.bottom + _iScrollY);
	LineTo(_DC, _rc.left + _iScrollX, _rc.bottom + _iScrollY);
	LineTo(_DC, _rc.left + _iScrollX, _rc.top + _iScrollY);

	SelectObject(_DC, oldPen);
	DeleteObject(myPen);
}
