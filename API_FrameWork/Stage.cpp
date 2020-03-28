#include "stdafx.h"
#include "Stage.h"
#include "Player.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "PillBug.h"
#include "TileMgr.h"
#include "ImageMgr.h"
#include "MyImage.h"

CStage::CStage()
{
}


CStage::~CStage()
{
}

void CStage::Initialize()
{

	//백그라운드 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_backlayer1.bmp", L"tutorial_backlayer1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_backlayer2.bmp", L"tutorial_backlayer2");

	//포그라운드 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_mainlayer.bmp", L"tutorial_mainlayer");


	

	//플레이어 로드
	CObj* player = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, player);
	
	//UI 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/healthUI.bmp", L"healthUI");


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
	

	//백그라운드 렌더
	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(L"tutorial_backlayer1");
	GdiTransparentBlt(_DC, 0 + iScrollX, 0 + iScrollY, 9448, 2160, memDC, 0, 0, 9448, 2160, RGB(30, 30, 30));
	 memDC = CBmpMgr::Get_Instance()->Find_Image(L"tutorial_backlayer2");
	GdiTransparentBlt(_DC, 0 + iScrollX, 0 + iScrollY, 9448, 2160, memDC, 0, 0, 9448, 2160, RGB(30, 30, 30));

	//오브젝트 (플레이어, 몬스터 등 렌더)
	CObjMgr::Get_Instance()->Render(_DC);

	//포그라운드 렌더
	 memDC = CBmpMgr::Get_Instance()->Find_Image(L"tutorial_mainlayer");
	GdiTransparentBlt(_DC, 0 + iScrollX, 0 + iScrollY, 9448, 2160, memDC, 0, 0, 9448, 2160, RGB(30, 30, 30));


	//이펙트 렌더
	CImageMgr::Get_Instance()->Render(_DC);
	CTileMgr::Get_Instance()->Render(_DC);

	//UI 렌더
	 memDC = CBmpMgr::Get_Instance()->Find_Image(L"healthUI");
	GdiTransparentBlt(_DC, 0, 0, 172, 106, memDC, 0, 0, 172, 106, RGB(30, 30, 30));

}

void CStage::Release()
{
	CTileMgr::Destroy_Instance();
	CImageMgr::Destroy_Instance();


}
