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

	//��׶��� �̹���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_backlayer1.bmp", L"tutorial_backlayer1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_backlayer2.bmp", L"tutorial_backlayer2");

	//���׶��� �̹���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_mainlayer.bmp", L"tutorial_mainlayer");

	//Ÿ���� TileX * TileY��ŭ �̸� �����صд�.
	CTileMgr::Get_Instance()->Initialize();
	//���� �����Ҷ� �� �̹����� �ҷ��´�.
	CImageMgr::Get_Instance()->Initialize();


	CObj* img;

	//����
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

	//��׶��� ����
	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(L"tutorial_backlayer1");
	GdiTransparentBlt(_DC, 0 + iScrollX, 0 + iScrollY, 9448, 2160, memDC, 0, 0, 9448, 2160, RGB(30, 30, 30));
	memDC = CBmpMgr::Get_Instance()->Find_Image(L"tutorial_backlayer2");
	GdiTransparentBlt(_DC, 0 + iScrollX, 0 + iScrollY, 9448, 2160, memDC, 0, 0, 9448, 2160, RGB(30, 30, 30));


	//���׶��� ����
	memDC = CBmpMgr::Get_Instance()->Find_Image(L"tutorial_mainlayer");
	GdiTransparentBlt(_DC, 0 + iScrollX, 0 + iScrollY, 9448, 2160, memDC, 0, 0, 9448, 2160, RGB(30, 30, 30));


	CImageMgr::Get_Instance()->Render(_DC);
	CTileMgr::Get_Instance()->Render(_DC);

	//�̹��� ������ ���� �̹��� �̸� ���
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
	//�����;������� �����Ͱ� ��ũ���� �������ش�.
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_Scroll_X(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_Scroll_X(-5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(-5.f);

	//�̹�����ŷ
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
		//���� ����� �̹����� �ϳ� �����Ѵ�.
		CObj* pImage = CImageMgr::Get_Instance()->Get_Image(m_iSelected);
		if (pImage)
		{
			POINT pt = {};
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);

			//���콺����Ʈ�� ��ũ�ѵ� �ݴ���⸸ŭ �������ش�. (�ֳ��ϸ� ����� �� �ݴ�������� �������� ������)
			pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
			pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

			pImage->Set_Pos((float)pt.x, (float)pt.y);
			CImageMgr::Get_Instance()->Add_Image(pImage);
		}
	}
	

	//�ش� �̹��� �����
	if (CKeyMgr::Get_Instance()->Key_Down('Q'))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		//���콺����Ʈ�� ��ũ�ѵ� �ݴ���⸸ŭ �������ش�. (�ֳ��ϸ� ����� �� �ݴ�������� �������� ������)
		pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();
		CImageMgr::Get_Instance()->Remove_Image(pt);


	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON))
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
	if (CKeyMgr::Get_Instance()->Key_Pressing('K'))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		//���콺����Ʈ�� ��ũ�ѵ� �ݴ���⸸ŭ �������ش�. (�ֳ��ϸ� ����� �� �ݴ�������� �������� ������)
		pt.x -= (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

		//�ش� ��ġ�� Ÿ���� ���¸� �����´�.
		CTileMgr::Get_Instance()->UnPiking_Tile(pt);
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

	//�ǵ�����
	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
		CImageMgr::Get_Instance()->Pop_back();

	//Ÿ�ϸ� ���̺�, �ε�
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
