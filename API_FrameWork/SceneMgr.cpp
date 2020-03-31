#include "stdafx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "MyMenu.h"
#include "Stage.h"
#include "Editor.h"
#include "ScrollMgr.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr()
	:m_eCurScene(SCENE_END), m_ePreScene(SCENE_END)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID _eID)
{
	m_eCurScene = _eID;

	if (m_ePreScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SCENE_LOGO:
			m_pScene = new CLogo;
			break;
		case SCENE_MENU:
			m_pScene = new CMyMenu;
			break;
		case SCENE_STAGE:
			m_pScene = new CStage;
			break;
		case SCENE_EDIT:
			m_pScene = new CEditor;
			break;
		}
		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}	

void CSceneMgr::Update()
{
	CScrollMgr::Get_Instance()->Update();
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC _DC)
{
	m_pScene->Render(_DC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}
