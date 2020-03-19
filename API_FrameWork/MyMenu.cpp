#include "stdafx.h"
#include "MyMenu.h"
#include "BmpMgr.h"
#include "MyButton.h"
#include "ObjMgr.h"
#include "MenuPointer.h"



CMyMenu::CMyMenu()
{
}


CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize()
{	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/MainMenu.bmp", L"MainMenu");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/GameStart.bmp", L"Button_Start");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/Editor.bmp", L"Button_Edit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/GameEnd.bmp", L"Button_Exit");




	
	//버튼을 만들고, 앞서 생성한 이미지의 키값을 통해 버튼의 이미지를 셋팅한다.
	CObj* pObj = CAbstractFactory<CMyButton>::Create(750.f, 350.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Button_Start");
	//오브젝트 매니저에서 관리하기 위해 버튼을 등록한다.
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(750.f, 410.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Button_Edit");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(750.f, 470.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Button_Exit");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);
}

void CMyMenu::Update()
{
	//오브젝트매니저에는 버튼들이 등록되어있다. 이것들을 실행시켜준다. 
	CObjMgr::Get_Instance()->Update();
}

void CMyMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CMyMenu::Render(HDC _DC)
{
	//메뉴 자기자신을 그린다.
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MainMenu");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);


	CObjMgr::Get_Instance()->Render(_DC);
}

void CMyMenu::Release()
{
	//오브젝트 매니저는 싱글톤이고, 모든 씬에 존재한다. 따라서 이때
	//CObjMgr::Destroy_Instance(); 를 해버리면 메뉴가 사용하는 것 이외의 플레이어, 몬스터등에 대한 정보도 지워지게 된다.
	//따라서 버튼만 선별적으로 지우는 함수를 만들어 지운다.
	CObjMgr::Get_Instance()->Delete_ID(OBJID::UI);
}
