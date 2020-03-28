#include "stdafx.h"
#include "Editor.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "ImageMgr.h"
#include "MyImage.h"
#include "Obj.h"

CEditor::CEditor()
	:m_iSelected(0)
{
}


CEditor::~CEditor()
{
}

void CEditor::Initialize()
{

	//백그라운드 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_backlayer1.bmp", L"tutorial_backlayer1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_backlayer2.bmp", L"tutorial_backlayer2");

	//포그라운드 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_mainlayer.bmp", L"tutorial_mainlayer");

	//타일을 TileX * TileY만큼 미리 생성해둔다.
	CTileMgr::Get_Instance()->Initialize();
	//맵을 제작할때 쓸 이미지를 불러온다.
	CImageMgr::Get_Instance()->Initialize();


	CObj* img;

	//몬스터
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/PillBug/move.bmp", L"pillBug");
	img = CAbstractFactory<CMyImage>::Create(0, 0, L"pillBug", 256, 256);
	CImageMgr::Get_Instance()->Add_EditImage(img);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Fly/move.bmp", L"fly");
	img = CAbstractFactory<CMyImage>::Create(0, 0, L"pillBug", 256, 256);
	CImageMgr::Get_Instance()->Add_EditImage(img);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Husk/move.bmp", L"husk");
	img = CAbstractFactory<CMyImage>::Create(0, 0, L"pillBug", 256, 256);
	CImageMgr::Get_Instance()->Add_EditImage(img);

	


}

void CEditor::Update()
{
	Key_Check();
	CTileMgr::Get_Instance()->Update();
	CImageMgr::Get_Instance()->Update();


}

void CEditor::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	CImageMgr::Get_Instance()->Late_Update();
}

void CEditor::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	//백그라운드 렌더
	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(L"tutorial_backlayer1");
	GdiTransparentBlt(_DC, 0 + iScrollX, 0 + iScrollY, 9448, 2160, memDC, 0, 0, 9448, 2160, RGB(30, 30, 30));
	memDC = CBmpMgr::Get_Instance()->Find_Image(L"tutorial_backlayer2");
	GdiTransparentBlt(_DC, 0 + iScrollX, 0 + iScrollY, 9448, 2160, memDC, 0, 0, 9448, 2160, RGB(30, 30, 30));


	//포그라운드 렌더
	memDC = CBmpMgr::Get_Instance()->Find_Image(L"tutorial_mainlayer");
	GdiTransparentBlt(_DC, 0 + iScrollX, 0 + iScrollY, 9448, 2160, memDC, 0, 0, 9448, 2160, RGB(30, 30, 30));


	CImageMgr::Get_Instance()->Render(_DC);
	CTileMgr::Get_Instance()->Render(_DC);

	//이미지 구분을 위한 이미지 이름 출력
	const TCHAR* szBuff = CImageMgr::Get_Instance()->Get_ImageName(m_iSelected);
	if (szBuff)
		TextOut(_DC, 800, 500, szBuff, lstrlen(szBuff));



}

void CEditor::Release()
{
	CImageMgr::Destroy_Instance();
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

	//이미지피킹
	if (CKeyMgr::Get_Instance()->Key_Down('F'))
	{
		++m_iSelected;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('G'))
	{
		--m_iSelected;
	}

	if (m_iSelected < 0)
		m_iSelected = 0;
	if (m_iSelected >= CImageMgr::Get_Instance()->Get_VecImageSize())
		m_iSelected = CImageMgr::Get_Instance()->Get_VecImageSize() - 1;

	if (CKeyMgr::Get_Instance()->Key_Down('D'))
	{
		//깊은 복사로 이미지를 하나 생성한다.
		CObj* pImage = CImageMgr::Get_Instance()->Get_Image(m_iSelected);
		if (pImage)
		{
			POINT pt = {};
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);

			//마우스포인트는 스크롤된 반대방향만큼 보정해준다. (왜냐하면 배경이 그 반대방향으로 움직였기 때문에)
			pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
			pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

			pImage->Set_Pos((float)pt.x, (float)pt.y);
			CImageMgr::Get_Instance()->Add_Image(pImage);
		}
	}
	

	//해당 이미지 지우기
	if (CKeyMgr::Get_Instance()->Key_Down('Q'))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		//마우스포인트는 스크롤된 반대방향만큼 보정해준다. (왜냐하면 배경이 그 반대방향으로 움직였기 때문에)
		pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();
		CImageMgr::Get_Instance()->Remove_Image(pt);


	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		//마우스포인트는 스크롤된 반대방향만큼 보정해준다. (왜냐하면 배경이 그 반대방향으로 움직였기 때문에)
		pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

		//해당 위치에 타일을 그려준다. 그리는 건 타일매니저에 위임한다.
		CTileMgr::Get_Instance()->Picking_Tile(pt);
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing('K'))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		//마우스포인트는 스크롤된 반대방향만큼 보정해준다. (왜냐하면 배경이 그 반대방향으로 움직였기 때문에)
		pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

		//해당 위치에 타일의 상태를 뒤집는다.
		CTileMgr::Get_Instance()->UnPiking_Tile(pt);
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		//마우스포인트는 스크롤된 반대방향만큼 보정해준다. (왜냐하면 배경이 그 반대방향으로 움직였기 때문에)
		pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

		CImageMgr::Get_Instance()->Drag_Image(pt);
	}

	//되돌리기
	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
		CImageMgr::Get_Instance()->Pop_back();

	//타일맵 세이브, 로드
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		CTileMgr::Get_Instance()->Save_Tile();
		CImageMgr::Get_Instance()->Save_Image();
	}
	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		CTileMgr::Get_Instance()->Load_Tile();
		CImageMgr::Get_Instance()->Load_Image();

	}
}
