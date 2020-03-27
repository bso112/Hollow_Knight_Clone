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
	memcpy(m_pFrameKey, L"pill_bug", DIR_LEN);
	m_tInfo.iCX = 140;
	m_tInfo.iCY = 70;
	m_tStat = STAT(30);

	//스폰된 장소가 정찰의 중심점
	m_PartolSpot.x = (LONG)m_tInfo.fX;
	m_PartolSpot.y = (LONG)m_tInfo.fY;

	m_fPatrol = 300;

	m_fSpeed = 1.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.iFrameScene = 1;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 200;


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
		m_tFrame.iFrameEnd = 1;
		m_tFrame.iFrameScene = 1;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 200;

	}
	else if(m_tInfo.fX > m_PartolSpot.x + m_fPatrol)
	{
		m_fSpeed *= -1.f;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 1;
		m_tFrame.iFrameScene = 0;
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
