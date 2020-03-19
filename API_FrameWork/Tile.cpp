#include "stdafx.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"


CTile::CTile()
	: m_iDrawID(0)
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

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	//Ÿ���̹��� ���
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile");

	//Ÿ�Ͽ� ��ũ�� ����
	BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY
		, TILECX, TILECY, hMemDC, TILECX * m_iDrawID, 0, SRCCOPY);
}

void CTile::Release()
{
}
