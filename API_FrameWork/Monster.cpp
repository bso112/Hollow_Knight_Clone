#include "stdafx.h"
#include "Monster.h"
#include "ObjMgr.h"


CMonster::CMonster()
{
}


CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_tInfo.fX = 400.f;
	m_tInfo.fY = 200.f;
	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;

	m_fSpeed = 1.f;
}

int CMonster::Update()
{
	if(m_bDead)
		return OBJ_DEAD;

	//m_pTarget = CObjMgr::Get_Instance()->Get_Player();

	//float	fX = 0.f, fY = 0.f, fDia = 0.f;
	//float	fRadian = 0.f;

	//// 목적지 - 출발지
	//fX = m_pTarget->Get_INFO().fX - m_tInfo.fX;
	//fY = m_pTarget->Get_INFO().fY - m_tInfo.fY;
	//fDia = sqrtf(fX * fX + fY * fY);
	//fRadian = acosf(fX / fDia);

	////// 라디안을 통한 각도 수정
	////if(m_pTarget->Get_INFO().fY > m_tInfo.fY)
	////	fRadian = (2 * PI) - fRadian;

	//m_fAngle = fRadian * 180.f / PI;

	//// 디그리를 통한 각도 수정
	//if(m_pTarget->Get_INFO().fY > m_tInfo.fY)
	//	m_fAngle *= -1.f;

	//m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	//m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;


	Update_Rect();

	return OBJ_NOEVENT;
}

void CMonster::Late_Update()
{
}

void CMonster::Render(HDC _DC)
{
	Update_Rect();

	//Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMonster::Release()
{
}
