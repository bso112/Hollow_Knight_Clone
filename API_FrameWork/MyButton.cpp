#include "stdafx.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"


CMyButton::CMyButton()
	:m_pText(nullptr), m_pFrameKey(nullptr)
{
}


CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	m_tInfo.iCX = 150;
	m_tInfo.iCY = 45;

	m_iDrawID = 0;
}

int CMyButton::Update()
{
	Update_Rect();




	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{

	//버튼의 포인트를 받아온다.
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	//해당렉트안에 포인트가 있는가?
	if (PtInRect(&m_tRect, pt))
	{
		//만약 왼쪽 마우스클릭되면
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			//현재 프레임키가 Button_Start이면
			if (!lstrcmp(m_pFrameKey, L"Button_Start"))
			{
				//스테이지 씬으로 전환
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_STAGE);
				return;
			}
			//현재 프레임키가 Button_Edit이면
			else if (!lstrcmp(m_pFrameKey, L"Button_Edit"))
			{

			}
			//현재 프레임키가 Button_Exit이면
			else if (!lstrcmp(m_pFrameKey, L"Button_Exit"))
				//현재 윈도우 종료
				DestroyWindow(g_hWnd);
		}
		m_iDrawID = 1;
	}
	else
		m_iDrawID = 0;
}

void CMyButton::Render(HDC _DC)
{
	Update_Rect();


	//프레임키는 현재 버튼이 어떤 이미지를 가지고 있는지 구분하는 역할을 함.
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	if (hMemDC)
	{
		//이미지는 스프라이트 이미지이기 때문에 m_iDrawID로 구분하여 출력함.
		//결과, Late_Update에서 버튼에 마우스를 올렸을때 m_iDrawID = 1;로 설정했으므로 버튼의 이미지가 바뀜.
		GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY
			, RGB(0, 0, 0));

	}

	if(m_pTarget)
		//텍스트 출력
		DrawText(_DC, m_pText, lstrlen(m_pText), &m_tRect, DT_CENTER | DT_VCENTER);




}

void CMyButton::Release()
{
}
