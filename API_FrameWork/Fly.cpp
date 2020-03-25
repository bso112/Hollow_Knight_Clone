#include "stdafx.h"
#include "Fly.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CFly::CFly()
{
}


CFly::~CFly()
{
}

void CFly::Initialize()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/fly_left.bmp", L"fly_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/fly_right.bmp", L"fly_right");

	memcpy(m_pFrameKey, L"fly_right", sizeof(TCHAR) * DIR_LEN);

	m_tInfo.iCX = 120;
	m_tInfo.iCY = 115;
	m_tStat = STAT(80);

	//스폰된 장소가 정찰의 중심점
	m_PartolSpot.x = (LONG)m_tInfo.fX;
	m_PartolSpot.y = (LONG)m_tInfo.fY;

	//인식범위
	m_fRadius = 400;
	//정찰범위
	m_fPatrol = 100;
	//속도
	m_fSpeed = 1.f;
}

int CFly::Update()
{
	Move_Frame();

	float target_fX = m_pTarget->Get_INFO().fX;

	//타깃과의 거리측정
	m_distToTarget = (Vector2(m_pTarget->Get_INFO().fX, m_pTarget->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY)).magnitude();

	//기본상태
	m_eCurState = STATE::IDLE;

	//타겟이 인식범위안에 들어오면
	if (m_tInfo.fX + m_fRadius >= target_fX
		&& m_tInfo.fX - m_fRadius <= target_fX)
	{
		//놀라고

		//추격상태가 된다.
		Chase_Target();
	}
	else
		//아니면 정찰
		Patrol();

	Scene_Change();
	return 0;
}

void CFly::Late_Update()
{
}

void CFly::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY,
		m_tInfo.iCX, m_tInfo.iCY, memDC, m_tInfo.iCX * m_tFrame.iFrameStart, m_tInfo.iCY * m_tFrame.iFrameScene,
		m_tInfo.iCX, m_tInfo.iCY, RGB(30, 30, 30));
}

void CFly::Release()
{
}

void CFly::Patrol()
{
	//순찰장소로 간다.

	Vector2 dirToPatrol = Vector2((float)m_PartolSpot.x, (float)m_PartolSpot.y) - Vector2(m_tInfo.fX, m_tInfo.fY);
	//순찰 장소와의 거리측정
	float distToPatrol = dirToPatrol.magnitude();

	dirToPatrol = dirToPatrol.Nomalize();
	
	//도착할때까지
	if (distToPatrol > 2)
	{
		//순찰장소로 간다.
		m_tInfo.fX += dirToPatrol.fX * m_fSpeed;
		m_tInfo.fY += dirToPatrol.fY * m_fSpeed;

		dirToPatrol.fX < 0 ? m_fDir = -abs(m_fDir) : m_fDir = abs(m_fDir);

	}


}

void CFly::Chase_Target()
{

	//플레이어 쫒기
	m_eCurState = STATE::RUN;


	float fX = m_pTarget->Get_INFO().fX - m_tInfo.fX;
	float fY = m_pTarget->Get_INFO().fY - m_tInfo.fY;
	float dia = sqrtf(fX * fX + fY * fY);

	//거리가 5보다 작으면
	if (dia < 5.f)
	{
		//멈춤
		return;
	}
	float theta = acosf(fX / dia);


	//타깃이 더 아래애 있으면 
	if (m_tInfo.fY < m_pTarget->Get_INFO().fY)
		theta = (2 * PI) - theta;

	float fDirX = cosf(theta);
	float fDirY = sinf(theta);

	float chasing_speed = m_fSpeed + 1;
	m_tInfo.fX += fDirX * chasing_speed;
	//윈도우 좌표계는 데카르트 좌표계의 반대니까.
	m_tInfo.fY -= fDirY * chasing_speed;

	//부호 뽑아내기
	m_fDir = fDirX / abs(fDirX);


}

void CFly::Scene_Change()
{

	//거리가 너무 가까워지면 정신없이 바뀜
	if (m_distToTarget > 5.f)
	{
		//방향에 따라 스프라이트 시트 바꾸기.
		if (m_fDir < 0)
			memcpy(m_pFrameKey, L"fly_left", sizeof(TCHAR) * DIR_LEN);
		else
			memcpy(m_pFrameKey, L"fly_right", sizeof(TCHAR) * DIR_LEN);
	}


	if (m_eCurState != m_ePrvState)
	{
		switch (m_eCurState)
		{
		case IDLE:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case WALK:
		{

		}
		case RUN:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case HIT:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 500;
			break;
		}
		case DEAD:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case END:
			break;
		default:
			break;
		}

		m_ePrvState = m_eCurState;
	}
}
