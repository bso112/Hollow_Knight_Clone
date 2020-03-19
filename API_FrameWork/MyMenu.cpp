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




	
	//��ư�� �����, �ռ� ������ �̹����� Ű���� ���� ��ư�� �̹����� �����Ѵ�.
	CObj* pObj = CAbstractFactory<CMyButton>::Create(750.f, 350.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Button_Start");
	//������Ʈ �Ŵ������� �����ϱ� ���� ��ư�� ����Ѵ�.
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
	//������Ʈ�Ŵ������� ��ư���� ��ϵǾ��ִ�. �̰͵��� ��������ش�. 
	CObjMgr::Get_Instance()->Update();
}

void CMyMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CMyMenu::Render(HDC _DC)
{
	//�޴� �ڱ��ڽ��� �׸���.
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MainMenu");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);


	CObjMgr::Get_Instance()->Render(_DC);
}

void CMyMenu::Release()
{
	//������Ʈ �Ŵ����� �̱����̰�, ��� ���� �����Ѵ�. ���� �̶�
	//CObjMgr::Destroy_Instance(); �� �ع����� �޴��� ����ϴ� �� �̿��� �÷��̾�, ���͵ ���� ������ �������� �ȴ�.
	//���� ��ư�� ���������� ����� �Լ��� ����� �����.
	CObjMgr::Get_Instance()->Delete_ID(OBJID::UI);
}
