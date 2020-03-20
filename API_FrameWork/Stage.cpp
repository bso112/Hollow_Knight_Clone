#include "stdafx.h"
#include "Stage.h"
#include "Player.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "PillBug.h"
CStage::CStage()
{
}


CStage::~CStage()
{
}

void CStage::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"BackGround");

	CObj* player = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, player);

	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CPillBug>::Create(300,300));

	
}

void CStage::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();


	//memDC의 -iScrollX 에서 시작해서 나머지를 보여준다는 것.
	//iScroll은 플레이어가 오른쪽으로 가면 차감, 왼쪽으로가면 증가다.
	//memDC를 출력하기 시작하는 좌표는 0,0이 기준이기 때문에 증감이 아니라, 부호로 표현한다.
	//예를들어 iScrollX가 차감되서 30에서 20이됬다고 쳤을때(플레이어 왼쪽으로 이동) 
	//출력할 X좌표는 -30에서 -20이 된다
	//즉, 배경이 오른쪽으로 이동한다.
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BackGround");



	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, -iScrollX, -iScrollY, SRCCOPY);

	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage::Release()
{
}
