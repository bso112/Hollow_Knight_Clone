#include "stdafx.h"
#include "PillBug.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CPillBug::CPillBug()
{
}


CPillBug::~CPillBug()
{
}

void CPillBug::Initialize()
{
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/pill_bug.bmp", L"pill_bug");

	m_tInfo.iCX = 70;
	m_tInfo.iCY = 70;
	m_tStat = STAT(30);

	//스폰된 장소가 정찰의 중심점
	m_PartolSpot.x = (LONG)m_tInfo.fX;
	m_PartolSpot.y = (LONG)m_tInfo.fY;

	m_fPatrol = 100;

	m_fSpeed = 3.f;
}

int CPillBug::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Patrol();
	Move_Frame();


	return OBJ_NOEVENT;
}

void CPillBug::Late_Update()
{
}

void CPillBug::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(L"pill_bug");

	GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY,
		m_tInfo.iCX, m_tInfo.iCY, memDC, m_tInfo.iCX * m_tFrame.iFrameStart, m_tInfo.iCY * m_tFrame.iFrameScene,
		m_tInfo.iCX, m_tInfo.iCY, RGB(30, 30, 30));

}

void CPillBug::Release()
{
}

void CPillBug::Patrol()
{	

	m_tInfo.fX += m_fSpeed;

	if (m_tInfo.fX < m_PartolSpot.x - m_fPatrol)
	{
		m_fSpeed *= -1.f;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 0;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 200;
	}
	else if(m_tInfo.fX > m_PartolSpot.x + m_fPatrol)
	{
		m_fSpeed *= -1.f;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 0;
		m_tFrame.iFrameScene = 1;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 200;
	}
}

void CPillBug::Chase_Target()
{
}

void CPillBug::Scene_Change()
{
}
