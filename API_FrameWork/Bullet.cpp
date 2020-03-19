#include "stdafx.h"
#include "Bullet.h"


CBullet::CBullet()
{
}


CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	m_tInfo.iCX = 30;
	m_tInfo.iCY = 30;

	m_fSpeed = 10.f;
}

int CBullet::Update()
{
	if(m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += cosf(m_fAngle * 3.141592f / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * 3.141592f / 180.f) * m_fSpeed;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CBullet::Late_Update()
{
	if(100 >= m_tRect.left || WINCX - 100 <= m_tRect.right
		|| 100 >= m_tRect.top || WINCY - 100 <= m_tRect.bottom)
		m_bDead = true;
}

void CBullet::Render(HDC _DC)
{
	Update_Rect();

	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CBullet::Release()
{
}