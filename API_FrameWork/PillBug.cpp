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
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/PillBug/move.bmp", L"pillBug_move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/PillBug/dead.bmp", L"pillBug_dead");

	memcpy(m_pFrameKey, L"pillBug_move", DIR_LEN);
	m_tInfo.iCX = 140;
	m_tInfo.iCY = 70;
	m_tImgInfo.iCX = 256;
	m_tImgInfo.iCY = 256;

	m_eFront = FRONT::RIGHT;

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

	Move_Frame();
	Patrol();
	Scene_Change();


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
		m_eFront = FRONT::RIGHT;
		m_fSpeed *= -1.f;
	}

	else if (m_tInfo.fX > m_PartolSpot.x + m_fPatrol)
	{
		m_eFront = FRONT::LEFT;
		m_fSpeed *= -1.f;
	}
}

void CPillBug::Chase_Target()
{
}

void CPillBug::Scene_Change()
{

	if (m_eCurState != m_ePrvState)
	{
		switch (m_eCurState)
		{
		case STATE::MOVE:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case STATE::DEAD:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = m_eCurState;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}
		
		}
		m_ePrvState = m_eCurState;
	}
}
