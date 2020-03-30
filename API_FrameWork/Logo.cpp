#include "stdafx.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"


CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo/Logo.bmp", L"Logo");

	m_hVideo = MCIWndCreate(g_hWnd, nullptr
		, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, L"../Video/Intro.wmv");

	CSoundMgr::Get_Instance()->Initialize();
	CSoundMgr::Get_Instance()->PlayBGM(L"Crossroads_Bass.wav");

	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);

	MCIWndPlay(m_hVideo);
}

void CLogo::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN)
		|| MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MENU);
		return;
	}
}

void CLogo::Late_Update()
{
}

void CLogo::Render(HDC _DC)
{
	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");
	//BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
	CSoundMgr::Get_Instance()->StopAll();
	MCIWndClose(m_hVideo);
	MCIWndDestroy(m_hVideo);
}
