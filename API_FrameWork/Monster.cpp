#include "stdafx.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "MyTime.h"

CMonster::CMonster()
	:m_fRadius(0.f), m_PartolSpot(), m_fPatrol(0.f), m_fDir(0.f), m_fDeadWait(0.f), m_dwDeadTimer(MAXDWORD), m_eFront(FRONT::LEFT), m_Gravity(Vector2(0, GRAVITY)),
	m_velocity(Vector2(0,0)), m_dwForceTimer(MAXDWORD), m_fForceTime(0.f), m_fAttRange(0.f)
{
	m_tStat = {};
	//몬스터의 타깃은 항상 플레이어
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	
	m_fDeadWait = 2.f;
	m_eTag = OBJTAG::MONSTER;
}

void CMonster::Add_Force(Vector2 _vDir, float _fForce, float _fTime)
{
	m_dwForceTimer = GetTickCount();
	m_velocity = _vDir * _fForce;
	m_fForceTime = _fTime;
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
		Vector2 pushDir = (Vector2(m_pTarget->Get_INFO().fX, m_pTarget->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY)).Nomalize();
		dynamic_cast<CPlayer*>(_pOther)->Add_Force(pushDir, 100.f, 0.2f);

	}
}

void CMonster::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC, (int)m_tImgRect.left + iScrollX, (int)m_tImgRect.top + iScrollY
		, m_tImgInfo.iCX, m_tImgInfo.iCY, memDC, m_tImgInfo.iCX * m_tFrame.iFrameScene, m_tImgInfo.iCY *m_tFrame.iFrameStart, m_tImgInfo.iCX, m_tImgInfo.iCY
		, RGB(30, 30, 30));

#pragma region 디버그


	TCHAR		szBuff[32] = L"";
	swprintf_s(szBuff, L"체력: %d", (int)m_tStat.m_fHp);
	TextOut(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, szBuff, lstrlen(szBuff));


#pragma endregion

}

CMonster::~CMonster()
{
}

