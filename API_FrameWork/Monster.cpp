#include "stdafx.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"

CMonster::CMonster()
	:m_fRadius(0.f), m_PartolSpot(), m_fPatrol(0.f)
{
	m_tStat = {};
	//몬스터의 타깃은 항상 플레이어
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
}

bool CMonster::IsAlert()
{
	//인식범위
	RECT rc = { (LONG)(m_tInfo.fX - m_fRadius), (LONG)(m_tInfo.fY - m_fRadius), (LONG)(m_tInfo.fX + m_fRadius), (LONG)(m_tInfo.fY + m_fRadius) };
	return CCollisionMgr::Check_Sphere(m_pTarget->Get_Rect(), rc);
}

CMonster::~CMonster()
{
}

