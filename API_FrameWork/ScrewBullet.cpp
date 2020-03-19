#include "stdafx.h"
#include "ScrewBullet.h"


CScrewBullet::CScrewBullet()
{
}


CScrewBullet::~CScrewBullet()
{
	Release();
}

void CScrewBullet::Initialize()
{
	m_tInfo.iCX = 20;
	m_tInfo.iCY = 20;
	m_fSpeed = 5.f;

	m_fRotAngle = 0.f;
	m_fRotSpeed = 30.f;
	m_fRotDis = 20.f;

	m_bStart = true;
}

int CScrewBullet::Update()
{
	if(m_bDead)
		return OBJ_DEAD;

	if(m_bStart)
	{
		m_tCenter.x = (LONG)m_tInfo.fX;
		m_tCenter.y = (LONG)m_tInfo.fY;
		m_bStart = false;
	}

	m_tCenter.x += (LONG)(cosf(m_fAngle * PI / 180.f) * m_fSpeed);
	m_tCenter.y -= (LONG)(sinf(m_fAngle * PI / 180.f) * m_fSpeed);

	m_fRotAngle += m_fRotSpeed;

	m_tInfo.fX = m_tCenter.x + (cosf(m_fRotAngle * PI / 180.f) * m_fRotDis);
	m_tInfo.fY = m_tCenter.y - (sinf(m_fRotAngle * PI / 180.f) * m_fRotDis);

	Update_Rect();

	return OBJ_NOEVENT;
}

void CScrewBullet::Late_Update()
{
	if(100 >= m_tRect.left || WINCX - 100 <= m_tRect.right
		|| 100 >= m_tRect.top || WINCY - 100 <= m_tRect.bottom)
		m_bDead = true;
}

void CScrewBullet::Render(HDC _DC)
{
	Update_Rect();

	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CScrewBullet::Release()
{
}
