#include "stdafx.h"
#include "GuideBullet.h"
#include "ObjMgr.h"


CGuideBullet::CGuideBullet()
{
}


CGuideBullet::~CGuideBullet()
{
	Release();
}

void CGuideBullet::Initialize()
{
	m_tInfo.iCX = 30;
	m_tInfo.iCY = 30;

	m_fSpeed = 5.f;
}

int CGuideBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pTarget = CObjMgr::Get_Instance()->Get_Target(this, OBJID::MONSTER);

	if (m_pTarget)
	{
		float fX = m_pTarget->Get_INFO().fX - m_tInfo.fX;
		float fY = m_pTarget->Get_INFO().fY - m_tInfo.fY;
		float fDis = sqrtf(fX * fX + fY * fY);
		float fRadian = acosf(fX / fDis);

		m_fAngle = fRadian * 180.f / PI;

		if (m_tInfo.fY < m_pTarget->Get_INFO().fY)
			m_fAngle *= -1.f;
	}

	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CGuideBullet::Late_Update()
{
	if (100 >= m_tRect.left || WINCX - 100 <= m_tRect.right
		|| 100 >= m_tRect.top || WINCY - 100 <= m_tRect.bottom)
		m_bDead = true;
}

void CGuideBullet::Render(HDC _DC)
{
	Update_Rect();
	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CGuideBullet::Release()
{
}
