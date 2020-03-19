#include "stdafx.h"
#include "MainGame.h"
#include "KeyMgr.h"
#include "MyEdit.h"


CMainGame::CMainGame()
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);
}

void CMainGame::Update()
{
	CMyEdit::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CMyEdit::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Key_Update();
}

void CMainGame::Render()
{
	Rectangle(m_DC, 0, 0, WINCX, WINCY);

	CMyEdit::Get_Instance()->Render(m_DC);

	// FPS Ãâ·Â
	++m_iFPS;
	if(m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
}

void CMainGame::Release()
{
	CKeyMgr::Destroy_Instance();
	CMyEdit::Destroy_Instance();

	ReleaseDC(g_hWnd, m_DC);
}
