#include "stdafx.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"


CTile::CTile()
	: m_iDrawID(0), m_bColider(false)
{
}


CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;

	m_eTag = OBJTAG::TILE;
}

int CTile::Update()
{
	return OBJ_NOEVENT;
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC _DC)
{
	Update_Rect();


	//콜라이더인 타일만 그린다. 
	if (!m_bColider)
		return;

	//스테이지일때는 E를 누르지않으면 안보인다.
	if (CSceneMgr::Get_Instance()->Get_CurrentScene() == CSceneMgr::SCENEID::SCENE_STAGE)
		if (!CKeyMgr::Get_Instance()->Key_Pressing('E'))
			return;

	float iScrollX = CScrollMgr::Get_Instance()->Get_Scroll_X();
	float iScrollY = CScrollMgr::Get_Instance()->Get_Scroll_Y();
	
	Draw_Rect(_DC, m_tRect, iScrollX, iScrollY);
}

void CTile::Release()
{
}
