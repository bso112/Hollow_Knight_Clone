#include "stdafx.h"
#include "Line.h"
#include "ScrollMgr.h"


CLine::CLine()
{
}


CLine::CLine(LINEPOS& _tLeft, LINEPOS& _tRight)
	: m_tInfo(_tLeft, _tRight)
{

}

CLine::CLine(LINEINFO& _tLineInfo)
	: m_tInfo(_tLineInfo)
{

}

CLine::~CLine()
{
	Release();
}

void CLine::Initialize()
{
}

void CLine::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	MoveToEx(_DC, (int)m_tInfo.tLeftPos.fX + iScrollX, (int)m_tInfo.tLeftPos.fY + iScrollY, nullptr);
	LineTo(_DC, (int)m_tInfo.tRightPos.fX + iScrollX, (int)m_tInfo.tRightPos.fY + iScrollY);
}

void CLine::Release()
{
}
