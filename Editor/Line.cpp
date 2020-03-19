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
	float fScrollX = CScrollMgr::Get_Instance()->Get_Scroll_X();

	MoveToEx(_DC, (int)(m_tInfo.tLeftPos.fX + fScrollX)
		, (int)m_tInfo.tLeftPos.fY, nullptr);
	LineTo(_DC, (int)(m_tInfo.tRightPos.fX + fScrollX)
		, (int)m_tInfo.tRightPos.fY);
}

void CLine::Release()
{
}
