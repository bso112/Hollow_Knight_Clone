#include "stdafx.h"
#include "Weapon.h"
#include "Monster.h"
#include "Player.h"
#include "MyImage.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CWeapon::CWeapon()
	:m_eOwner(END), m_fDamage(0.f),m_fDuration(FLT_MAX)
{
}


CWeapon::~CWeapon()
{
}

void CWeapon::Initialize()
{
	

	m_dwTimer = GetTickCount();
}

int CWeapon::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Move_Frame();

	return OBJ_NOEVENT;
}

void CWeapon::Late_Update()
{

	//지속시간동안 생존
	if (m_dwTimer + m_fDuration * 1000 < GetTickCount())
		m_bDead = true;
}

void CWeapon::Render(HDC _DC)
{
	Update_Rect();


	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	GdiTransparentBlt(_DC, (int)m_tImgRect.left + iScrollX, (int)m_tImgRect.top + iScrollY
		, m_tImgInfo.iCX, m_tImgInfo.iCY, hMemDC, m_tImgInfo.iCX * m_tFrame.iFrameScene, m_tImgInfo.iCY *m_tFrame.iFrameStart, m_tImgInfo.iCX, m_tImgInfo.iCY
		, RGB(30, 30, 30));
}

void CWeapon::Release()
{

}

void CWeapon::OnCollisionEnter(CObj * _pOther, float _fX, float _fY)
{
	if (_pOther->Get_Tag() == OBJTAG::MONSTER && m_eOwner == OWNER::PLAYER)
	{
		dynamic_cast<CMonster*>(_pOther)->Take_Damage(m_fDamage);
		Vector2 pushDir = (Vector2(_pOther->Get_INFO().fX, _pOther->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY)).Nomalize();
		dynamic_cast<CMonster*>(_pOther)->Add_Force(pushDir, 100.f, 0.2f);
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
