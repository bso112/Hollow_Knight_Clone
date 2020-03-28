
#include "stdafx.h"

void Draw_Rect(HDC _DC, const RECT& _rc, float _iScrollX, float _iScrollY)
{
	HPEN  myPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(_DC, myPen);

	MoveToEx(_DC, _rc.left + (int)_iScrollX, _rc.top + (int)_iScrollY, nullptr);
	LineTo(_DC, _rc.right + (int)_iScrollX, _rc.top + (int)_iScrollY);
	LineTo(_DC, _rc.right + (int)_iScrollX, _rc.bottom + (int)_iScrollY);
	LineTo(_DC, _rc.left + (int)_iScrollX, _rc.bottom + (int)_iScrollY);
	LineTo(_DC, _rc.left + (int)_iScrollX, _rc.top + (int)_iScrollY);

	SelectObject(_DC, oldPen);
	DeleteObject(myPen);
}
