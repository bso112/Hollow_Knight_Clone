#include "stdafx.h"
#include "Mouse.h"


CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;
}

int CMouse::Update()
{
	// GetCursorPos() 
	// 마우스 커서의 위치 좌표를 얻어오는 함수이다.

	// ScreenToClient()
	// 전체 스크린 영여겡서 클라이언 좌표로 변환해주는 함수이다.
	// 사용하지 않을 경우 클라이언 좌표가 아닌 윈도우 전체 영역의 좌표로 계산이 된다.

	POINT	pt = {};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	ShowCursor(false);

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;
	
	Update_Rect();

	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC _DC)
{
	Update_Rect();

	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMouse::Release()
{
}
