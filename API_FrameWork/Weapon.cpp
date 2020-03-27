#include "stdafx.h"
#include "Weapon.h"
#include "Monster.h"
#include "Player.h"
#include "MyImage.h"

CWeapon::CWeapon()
	:m_eOwner(END), m_fDamage(0.f),m_fDuration(FLT_MAX)
{
}


CWeapon::~CWeapon()
{
}

void CWeapon::Initialize()
{
	if (m_pEffect)
		m_pEffect->Initialize();

	m_dwTimer = GetTickCount();
}

int CWeapon::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	if (m_pEffect)
		m_pEffect->Set_Pos(m_tInfo.fX, m_tInfo.fY);

	return OBJ_NOEVENT;
}

void CWeapon::Late_Update()
{
	if (m_pEffect)
		m_pEffect->Late_Update();

	//지속시간동안 생존
	if (m_dwTimer + m_fDuration * 1000 < GetTickCount())
		m_bDead = true;
}

void CWeapon::Render(HDC _DC)
{
	if (m_pEffect)
		m_pEffect->Render(_DC);
}

void CWeapon::Release()
{
	SAFE_DELETE(m_pEffect);
}

void CWeapon::OnCollisionEnter(CObj * _pOther, float _fX, float _fY)
{
	if (_pOther->Get_Tag() == OBJTAG::MONSTER && m_eOwner == OWNER::PLAYER)
	{
		dynamic_cast<CMonster*>(_pOther)->Take_Damage(m_fDamage);
	}
	else if (_pOther->Get_Tag() == OBJTAG::PLAYER && m_eOwner == OWNER::MONSTER)
	{
		dynamic_cast<CPlayer*>(_pOther)->Take_Damage(m_fDamage);
	}
}


void CWeapon::Swing(float fAngle, float fSpeed)
{
	//회전
}

void CWeapon::Shoot()
{
}
