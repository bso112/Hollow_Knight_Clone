#include "stdafx.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"

CMonster::CMonster()
	:m_fRadius(0.f), m_PartolSpot(), m_fPatrol(0.f)
{
	m_tStat = {};
	//������ Ÿ���� �׻� �÷��̾�
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
}

bool CMonster::IsAlert()
{
	//�νĹ���
	RECT rc = { (LONG)(m_tInfo.fX - m_fRadius), (LONG)(m_tInfo.fY - m_fRadius), (LONG)(m_tInfo.fX + m_fRadius), (LONG)(m_tInfo.fY + m_fRadius) };
	return CCollisionMgr::Check_Sphere(m_pTarget->Get_Rect(), rc);
}

CMonster::~CMonster()
{
}

