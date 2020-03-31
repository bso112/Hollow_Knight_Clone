#include "stdafx.h"
#include "ScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f), m_dwShakeTimer(0), m_fShakeTime(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Update()
{
	if (m_dwShakeTimer + m_fShakeTime * 1000 > GetTickCount())
	{
		//0.1초에 한번씩 움직임
		if (m_dwShakeTermTimer + 25 < GetTickCount())
		{
			//카메라 흔듦
			Set_Scroll_X(m_fShakeIntencity);
			m_fShakeIntencity = -m_fShakeIntencity;
			m_dwShakeTermTimer = GetTickCount();
		}
	}
	else
	{
		m_dwShakeTimer = 0;
		m_fShakeTime = 0.f;
		m_dwShakeTermTimer = 0;
	}

}


void CScrollMgr::Scroll_Lock()
{
	//캐릭터가 맵의 (0,0) 즉, 왼쪽 위를 기준으로 위치하고 있다.
	//따라서 캐릭터가 그 상태에서 왼쪽으로 이동하거나 위로 이동했을때 스크롤하면 맵의 바깥쪽이 보이게된다.
	//따라서 락을 걸어준다.

	//캐릭터가 왼쪽으로 이동할때 락 
	if (0 < m_fScrollX)
		m_fScrollX = 0.f;
	//캐릭터가 위로 이동할때 락
	if (0 < m_fScrollY)
		m_fScrollY = 0.f;

	//캐릭터가 오른쪽으로 이동할때 락
	//WINCX - TILECX * TILEX 는 맵이 스크롤될 수 있는 최대값이다.
	//즉, 최대 스크롤값을 제한해 준것이다.
	if (-8440 > m_fScrollX)
		m_fScrollX = -8440;
	//캐릭터가 아래로 이동할때 락
	if (-1970 > m_fScrollY)
		m_fScrollY = -1970;
}

void CScrollMgr::Shake_Camera(float _fIntencity, float _fTime)
{
	m_fShakeIntencity = _fIntencity;
	m_fShakeTime = _fTime;
	m_dwShakeTimer = GetTickCount();
	m_dwShakeTermTimer = GetTickCount();
}
