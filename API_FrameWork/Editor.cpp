#include "stdafx.h"
#include "Editor.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"


CEditor::CEditor()
{
}


CEditor::~CEditor()
{
}

void CEditor::Initialize()
{
	//타일 스프라이트시트를 불러와 비트맵 매니저에 저장한다.
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile.bmp", L"Tile");
	//타일을 TileX * TileY만큼 미리 생성해둔다.
	CTileMgr::Get_Instance()->Initialize();
}

void CEditor::Update()
{
	Key_Check();
	CTileMgr::Get_Instance()->Update();
}

void CEditor::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
}

void CEditor::Render(HDC _DC)
{
	CTileMgr::Get_Instance()->Render(_DC);
}

void CEditor::Release()
{
	CTileMgr::Destroy_Instance();
}

void CEditor::Key_Check()
{
	//에디터씬에서는 에디터가 스크롤을 셋팅해준다.
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_Scroll_X(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_Scroll_X(-5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(-5.f);

	//만약 왼쪽 마우스클릭이면
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		//마우스포인트는 스크롤된 반대방향만큼 보정해준다. (왜냐하면 배경이 그 반대방향으로 움직였기 때문에)
		pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

		//해당 위치에 타일을 그려준다. 그리는 건 타일매니저에 위임한다.
		CTileMgr::Get_Instance()->Picking_Tile(pt, 1);
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

		CTileMgr::Get_Instance()->Picking_Tile(pt, 0);
	}

	//타일맵 세이브, 로드
	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
		CTileMgr::Get_Instance()->Save_Tile();
	if (CKeyMgr::Get_Instance()->Key_Down('X'))
		CTileMgr::Get_Instance()->Load_Tile();
}
