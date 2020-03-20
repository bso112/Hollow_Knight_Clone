#include "stdafx.h"
#include "Editor.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "ImageMgr.h"


CEditor::CEditor()
{
}


CEditor::~CEditor()
{
}

void CEditor::Initialize()
{
	//��׶��� �̹���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/background1.bmp", L"background");
	//Ÿ���� TileX * TileY��ŭ �̸� �����صд�.
	CTileMgr::Get_Instance()->Initialize();
	//���� �����Ҷ� �� �̹����� �ҷ��´�.
	CImageMgr::Get_Instance()->Initialize();

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
	int iScrollX = CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(L"background");
	BitBlt(_DC, 0, 0, WINCX, WINCY, memDC, -iScrollX, -iScrollY, SRCCOPY);
	CImageMgr::Get_Instance()->Render(_DC);
	CTileMgr::Get_Instance()->Render(_DC);


}

void CEditor::Release()
{
	CImageMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
}

void CEditor::Key_Check()
{
	//�����;������� �����Ͱ� ��ũ���� �������ش�.
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_Scroll_X(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_Scroll_X(-5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(-5.f);

	//���� ���� ���콺Ŭ���̸�
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		//���콺����Ʈ�� ��ũ�ѵ� �ݴ���⸸ŭ �������ش�. (�ֳ��ϸ� ����� �� �ݴ�������� �������� ������)
		pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

		//�ش� ��ġ�� Ÿ���� �׷��ش�. �׸��� �� Ÿ�ϸŴ����� �����Ѵ�.
		CTileMgr::Get_Instance()->Picking_Tile(pt);
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		//���콺����Ʈ�� ��ũ�ѵ� �ݴ���⸸ŭ �������ش�. (�ֳ��ϸ� ����� �� �ݴ�������� �������� ������)
		pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

		CImageMgr::Get_Instance()->Drag_Image(pt);
	}

	//Ÿ�ϸ� ���̺�, �ε�
	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
		CTileMgr::Get_Instance()->Save_Tile();
	if (CKeyMgr::Get_Instance()->Key_Down('X'))
		CTileMgr::Get_Instance()->Load_Tile();
}
