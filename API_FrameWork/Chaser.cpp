#include "stdafx.h"
#include "Chaser.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CChaser::CChaser()
{
	ZeroMemory(m_pFrameKey, sizeof(TCHAR) * DIR_LEN);
}


CChaser::~CChaser()
{
}

void CChaser::Initialize()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/chaser_left.bmp", L"chaser_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/chaser_right.bmp", L"chaser_right");

	memcpy(m_pFrameKey, L"chaser_right", sizeof(TCHAR) * DIR_LEN);

	m_tInfo.iCX = 100;
	m_tInfo.iCY = 100;
	m_tStat = STAT(50);

	//스폰된 장소가 정찰의 중심점
	m_PartolSpot.x = (LONG)m_tInfo.fX;
	m_PartolSpot.y = (LONG)m_tInfo.fY;

	m_fRadius = 200;
	m_fPatrol = 100;
	m_fSpeed = 1.f;
}

int CChaser::Update()
{
	Move_Frame();

	float target_fX = m_pTarget->Get_INFO().fX;


	//타겟이 인식범위안에 들어오면
	if (m_tInfo.fX + m_fRadius >= target_fX
		&& m_tInfo.fX - m_fRadius <= target_fX)
	{
		//추격상태가 된다.
		Chase_Target();
	}
	else
		//아니면 정찰
		Patrol();

	Scene_Change();
	return 0;
}

void CChaser::Late_Update()
{
}

void CChaser::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY,
		m_tInfo.iCX, m_tInfo.iCY, memDC, m_tInfo.iCX * m_tFrame.iFrameStart, m_tInfo.iCY * m_tFrame.iFrameScene,
		m_tInfo.iCX, m_tInfo.iCY, RGB(30, 30, 30));
}

void CChaser::Release()
{
}

void CChaser::Patrol()
{

	//방향에 따라 스프라이트 시트 바꾸기.
	if (m_fSpeed < 0)
		memcpy(m_pFrameKey, L"chaser_left", sizeof(TCHAR) * DIR_LEN);
	else
		memcpy(m_pFrameKey, L"chaser_right", sizeof(TCHAR) * DIR_LEN);

	m_eCurState = STATE::WALK;

	//순찰범위를 벗어나면
	if (m_tInfo.fX < m_PartolSpot.x - m_fPatrol)
	{
		if (m_tInfo.fX > m_PartolSpot.x + m_fPatrol)
		{
			float fX = m_PartolSpot.x - m_tInfo.fX;
			//부호만 구해서
			fX /= fX;
			//이동
			m_tInfo.fX += fX * m_fSpeed;
		}
	}
	else
		m_tInfo.fX += m_fSpeed;






}

void CChaser::Chase_Target()
{
	m_eCurState = STATE::RUN;


	//2초 간격으로 추격
	static DWORD timer = GetTickCount();
	//2초에 한번씩 뒤바뀌는 락
	static bool lock = false;
	if (timer + 2000 < GetTickCount())
	{
		timer = GetTickCount();
		lock = !lock;
	}

	if (!lock)
	{
		m_fSpeed = abs(m_fSpeed);

		//타겟이 왼쪽에 있으면
		if (m_pTarget->Get_INFO().fX - m_tInfo.fX < 0)
		{
			//왼쪽으로 감
			m_tInfo.fX -= m_fSpeed + 3;
			memcpy(m_pFrameKey, L"chaser_left", sizeof(TCHAR) * DIR_LEN);
		}
		else
		{
			memcpy(m_pFrameKey, L"chaser_right", sizeof(TCHAR) * DIR_LEN);
			m_tInfo.fX += m_fSpeed + 3;
		}
	}


}

void CChaser::Scene_Change()
{
	if (m_eCurState != m_ePrvState)
	{
		switch (m_eCurState)
		{
		case CChaser::IDLE:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case CChaser::WALK:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case CChaser::RUN:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case CChaser::HIT:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 500;
			break;
		}
		case CChaser::DEAD:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case CChaser::END:
			break;
		default:
			break;
		}

		m_ePrvState = m_eCurState;
	}
}
