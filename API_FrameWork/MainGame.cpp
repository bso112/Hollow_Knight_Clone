#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"
#include "Monster.h"
#include "Mouse.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "ImageMgr.h"
#include "MyTime.h"


CMainGame::CMainGame()
	: m_pPlayer(nullptr), m_dwTime(GetTickCount()), m_iFPS(0), m_pCursor(nullptr)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");
	
	CMyTime::Get_Instance()->Initalize();
	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENEID::SCENE_LOGO);
	m_pCursor = CAbstractFactory<CMouse>::Create();
}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
	CMyTime::Get_Instance()->Update();
	m_pCursor->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Key_Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();
	m_pCursor->Late_Update();
}

void CMainGame::Render()
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	HDC hBackBuffer = CBmpMgr::Get_Instance()->Find_Image(L"BackBuffer");

	BitBlt(hBackBuffer, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CSceneMgr::Get_Instance()->Render(hBackBuffer);
	m_pCursor->Render(hBackBuffer);

#pragma region 출력
	// 스크롤 출력
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();
	TCHAR		szBuff[32] = L"";
	swprintf_s(szBuff, L"ScrollX: %d", iScrollX);
	TextOut(hBackBuffer, 50, 50, szBuff, lstrlen(szBuff));
	swprintf_s(szBuff, L"ScrollY: %d", iScrollY);
	TextOut(hBackBuffer, 50, 100, szBuff, lstrlen(szBuff));


	// FPS 출력
	++m_iFPS;
	if(m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

#pragma endregion

	BitBlt(m_DC, 0, 0, WINCX, WINCY, hBackBuffer, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	CSceneMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CImageMgr::Destroy_Instance();
	SAFE_DELETE(m_pCursor);
	ReleaseDC(g_hWnd, m_DC);
}
