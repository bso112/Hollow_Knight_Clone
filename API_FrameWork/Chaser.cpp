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

	m_fDir = 1.f;

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

	//타깃과의 거리측정
	m_distToTarget = (Vector2(m_pTarget->Get_INFO().fX, m_pTarget->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY)).magnitude();

	//타겟이 인식범위안에 들어오면
	if (m_distToTarget < m_fRadius)
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

	(HPEN)SelectObject(_DC, (HPEN)GetStockObject(WHITE_PEN));

	MoveToEx(_DC, m_tInfo.fX + iScrollX, m_tInfo.fY + iScrollY, nullptr);
	LineTo(_DC, m_PartolSpot.x + iScrollX, m_PartolSpot.y + iScrollY);

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

	m_eCurState = STATE::IDLE;

	//순찰범위를 벗어나면
	if (m_tInfo.fX < m_PartolSpot.x - m_fPatrol
		|| m_tInfo.fX > m_PartolSpot.x + m_fPatrol)
	{

		m_eCurState = STATE::WALK;

		Vector2 dirToPatrol = Vector2((float)m_PartolSpot.x, (float)m_PartolSpot.y) - Vector2(m_tInfo.fX, m_tInfo.fY);
		//순찰 장소와의 거리측정
		float distToPatrol = dirToPatrol.magnitude();

		dirToPatrol = dirToPatrol.Nomalize();

		//도착할때까지
		if (distToPatrol > 2)
		{
			//순찰장소로 간다.
			m_tInfo.fX += dirToPatrol.fX * m_fSpeed;

			dirToPatrol.fX < 0 ? m_fDir = -1 : m_fDir = 1;

		}

	}
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
			m_fDir = -1;
		}
		else
		{
			//오른쪽으로 감
			m_tInfo.fX += m_fSpeed + 3;
			m_fDir = 1;
		}
	}
	else
		m_eCurState = STATE::IDLE;


}

void CChaser::Scene_Change()
{

	//거리가 너무 가까워지면 정신없이 바뀜
	if (m_distToTarget > 1.f)
	{
		//방향에 따라 스프라이트 시트 바꾸기.
		if (m_fDir < 0)
			memcpy(m_pFrameKey, L"chaser_left", sizeof(TCHAR) * DIR_LEN);
		else
			memcpy(m_pFrameKey, L"chaser_right", sizeof(TCHAR) * DIR_LEN);
	}


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
			m_tFrame.bLoop = true;
			break;
		}
		case CChaser::WALK:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case CChaser::RUN:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case CChaser::HIT:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 500;
			m_tFrame.bLoop = false;
			break;
		}
		case CChaser::DEAD:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
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
