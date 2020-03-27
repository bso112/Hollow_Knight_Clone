#include "stdafx.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Player.h"

CMonster::CMonster()
	:m_fRadius(0.f), m_PartolSpot(), m_fPatrol(0.f), m_fDir(0.f)
{
	m_tStat = {};
	//몬스터의 타깃은 항상 플레이어
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	

	m_eTag = OBJTAG::MONSTER;
}

bool CMonster::IsAlert()
{
	//인식범위
	RECT rc = { (LONG)(m_tInfo.fX - m_fRadius), (LONG)(m_tInfo.fY - m_fRadius), (LONG)(m_tInfo.fX + m_fRadius), (LONG)(m_tInfo.fY + m_fRadius) };
	if (m_pTarget == nullptr)
		return false;
	return CCollisionMgr::Check_Sphere(m_pTarget->Get_Rect(), rc);
}

void CMonster::OnCollisionEnter(CObj * _pOther, float _fX, float _fY)
{
	if (_pOther->Get_Tag() == OBJTAG::PLAYER)
	{
		dynamic_cast<CPlayer*>(_pOther)->Take_Damage(20);
	}
}

CMonster::~CMonster()
{
}

