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

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	
	//빨간 사각형 그리기
	HPEN  myPen = CreatePen(PS_SOLID, 1, RGB(255,0,0));
	HPEN oldPen = (HPEN)SelectObject(_DC, myPen);

	MoveToEx(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, nullptr);
	LineTo(_DC, m_tRect.right + iScrollX, m_tRect.top + iScrollY);
	LineTo(_DC, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
	LineTo(_DC, m_tRect.left + iScrollX, m_tRect.bottom + iScrollY);
	LineTo(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY);

	SelectObject(_DC, oldPen);
	DeleteObject(myPen);

	//타일이미지 출력
	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile");

	//타일에 스크롤 적용
	/*BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY
		, TILECX, TILECY, hMemDC, TILECX * m_iDrawID, 0, SRCCOPY);*/
}

void CTile::Release()
{
}
