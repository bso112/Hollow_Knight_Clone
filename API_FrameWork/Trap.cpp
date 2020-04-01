#include "stdafx.h"
#include "Trap.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "Monster.h"
#include "ObjMgr.h"
CTrap::CTrap()
{
}


CTrap::~CTrap()
{
}

void CTrap::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Editor/spike.bmp", L"spike");
}

int CTrap::Update()
{
	return 0;
}

void CTrap::Late_Update()
{
}

void CTrap::Render(HDC _DC)
{
	Update_Rect();

	if (lstrcmp(m_pFrameKey, L""))
	{

		int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();


		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


		GdiTransparentBlt(_DC, (int)m_tImgRect.left + iScrollX, (int)m_tImgRect.top + iScrollY
			, m_tImgInfo.iCX, m_tImgInfo.iCY, hMemDC, m_tImgInfo.iCX * m_tFrame.iFrameScene, m_tImgInfo.iCY *m_tFrame.iFrameStart, m_tImgInfo.iCX, m_tImgInfo.iCY
			, RGB(30, 30, 30));
	}
}

void CTrap::Release()
{
}

void CTrap::OnCollisionEnter(CObj * _pOther, float _fX, float _fY)
{
	if (_pOther->Get_Tag() == OBJTAG::PLAYER)
	{
		dynamic_cast<CPlayer*>(_pOther)->Take_Damage(20);
		CPlayer* player = dynamic_cast<CPlayer*>(_pOther);
		player->Take_Damage(20);
		Vector2 dir = Vector2(_pOther->Get_INFO().fX, _pOther->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY);
		dir = dir.Nomalize();
		player->Add_Force(dir, 1500.f, 0.2f);
	}

	//if (_pOther->Get_Tag() == OBJTAG::WEAPON)
	//{
	//	CPlayer* player = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	//	Vector2 dir = Vector2(player->Get_INFO().fX, player->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY);
	//	dir = dir.Nomalize();
	//	player->Add_Force(dir, 3000.f, 0.2f);
	//}
	if (_pOther->Get_Tag() == OBJTAG::MONSTER)
	{
		CMonster* monster = dynamic_cast<CMonster*>(_pOther);
		monster->Take_Damage(20);
		Vector2 dir = Vector2(_pOther->Get_INFO().fX, _pOther->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY);
		dir = dir.Nomalize();
		monster->Add_Force(dir, 1500.f, 0.2f);
	}
}
