#include "stdafx.h"
#include "Stage.h"
#include "Player.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "PillBug.h"
#include "TileMgr.h"
#include "ImageMgr.h"

CStage::CStage()
{
}


CStage::~CStage()
{
}

void CStage::Initialize()
{
	//��׶��� �̹���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/background1.bmp", L"background");

	CObj* player = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, player);

	//CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CPillBug>::Create(300,300));
	
	//��� �ε�
	CTileMgr::Get_Instance()->Initialize();
	CImageMgr::Get_Instance()->Initialize();
	CTileMgr::Get_Instance()->Load_Tile();
	CImageMgr::Get_Instance()->Load_Image();

	
}

void CStage::Update()
{
	CObjMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();
	CImageMgr::Get_Instance()->Update();
}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	CTileMgr::Get_Instance()->Late_Update();
	CImageMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();


	//memDC�� -iScrollX ���� �����ؼ� �������� �����شٴ� ��.
	//iScroll�� �÷��̾ ���������� ���� ����, �������ΰ��� ������.
	//memDC�� ����ϱ� �����ϴ� ��ǥ�� 0,0�� �����̱� ������ ������ �ƴ϶�, ��ȣ�� ǥ���Ѵ�.
	//������� iScrollX�� �����Ǽ� 30���� 20�̉�ٰ� ������(�÷��̾� �������� �̵�) 
	//����� X��ǥ�� -30���� -20�� �ȴ�
	//��, ����� ���������� �̵��Ѵ�.
	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(L"background");
	BitBlt(_DC, 0, 0, WINCX, WINCY, memDC, -iScrollX, -iScrollY, SRCCOPY);

	CTileMgr::Get_Instance()->Render(_DC);
	CImageMgr::Get_Instance()->Render(_DC);
	CObjMgr::Get_Instance()->Render(_DC);


}

void CStage::Release()
{
	CTileMgr::Destroy_Instance();
	CImageMgr::Destroy_Instance();

}
