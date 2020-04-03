#include "stdafx.h"
#include "ImageMgr.h"
#include "MyImage.h"
#include "BmpMgr.h"
#include "PillBug.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
#include "Chaser.h"
#include "ObjMgr.h"
#include "Jumper.h"
#include "Fly.h"
#include "HuskGaurd.h"
#include "Trap.h"
#include "FalseKnight.h"
CImageMgr* CImageMgr::m_pInstance = nullptr;
CImageMgr::CImageMgr()
{
}


CImageMgr::~CImageMgr()
{
	Release();
}

void CImageMgr::Initialize()
{

}

void CImageMgr::Update()
{
	auto& iter = m_vecImage.begin();
	for (; iter != m_vecImage.end();)
	{
		int msg =(*iter)->Update();
		if (msg == OBJ_DEAD)
		{
			SAFE_DELETE(*iter);
			iter = m_vecImage.erase(iter);

			if (m_vecImage.empty())
				break;
		}
		else
			++iter;
	}
}

void CImageMgr::Late_Update()
{
	for (auto& img : m_vecImage)
	{
		img->Late_Update();
	}
}

void CImageMgr::Render(HDC _DC)
{
	for (auto& img : m_vecImage)
		img->Render(_DC);

}

void CImageMgr::Remove_Image(POINT _pt)
{
	auto& iter = m_vecImage.begin();
	auto& iter_end = m_vecImage.end();
	for (; iter != iter_end;)
	{
		if (PtInRect(&(*iter)->Get_Rect(), _pt))
		{
			iter = m_vecImage.erase(iter);
			iter_end = m_vecImage.end();

		}
		else
			++iter;
	}

}

void CImageMgr::Release()
{
	for_each(m_vecImageForEditor.begin(), m_vecImageForEditor.end(), Safe_Delete<CObj*>);
	m_vecImageForEditor.clear();

	for_each(m_vecImage.begin(), m_vecImage.end(), Safe_Delete<CObj*>);
	m_vecImage.clear();

}

CObj* CImageMgr::Get_Image(int _iIndex)
{
	//유효성검사
	if (_iIndex < 0 || (size_t)_iIndex >= m_vecImageForEditor.size())
		return nullptr;

	CMyImage* newImage = new CMyImage(*dynamic_cast<CMyImage*>(m_vecImageForEditor[_iIndex]));

	return newImage;
}

const TCHAR * CImageMgr::Get_ImageName(int _iIndex)
{
	//유효성검사
	if (_iIndex < 0 || (size_t)_iIndex >= m_vecImageForEditor.size())
		return nullptr;

	return m_vecImageForEditor[_iIndex]->Get_pFrameKey();
}



void CImageMgr::Drag_Image(POINT & _pt)
{
	for (auto& pImage : m_vecImage)
	{
		if (PtInRect(&pImage->Get_Rect(), _pt))
		{
			pImage->Set_Pos((float)_pt.x, (float)_pt.y);
			break;
		}
	}
}

void CImageMgr::Save_Image()
{
	HANDLE hFile = CreateFile(L"../Data/Background.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;

	for (auto& img : m_vecImage)
	{

		//이미지의 위치, 크기를 저장한다.
		WriteFile(hFile, &img->Get_INFO(), sizeof(INFO), &dwByte, NULL);
		//이미지의 태그 저장.
		WriteFile(hFile, &dynamic_cast<CMyImage*>(img)->Get_Tag(), sizeof(SAVEDATA::TAG), &dwByte, NULL);
		//프레임 키 저장
		WriteFile(hFile, img->Get_pFrameKey(), sizeof(TCHAR) *DIR_LEN, &dwByte, NULL);
	}




	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Image Save", L"Success", MB_OK);
}

void CImageMgr::Load_Image()
{
	HANDLE hFile = CreateFile(L"../Data/Background.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;


	DWORD dwByte = 0;
	INFO tInfo = {};
	TCHAR pFrameKey[DIR_LEN] = L"";
	SAVEDATA::TAG eTag = SAVEDATA::END;


	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &eTag, sizeof(SAVEDATA::TAG), &dwByte, NULL);
		ReadFile(hFile, pFrameKey, sizeof(TCHAR) * DIR_LEN, &dwByte, NULL);

		if (0 == dwByte)
			break;


		//만약 스테이지 씬이면
		if (CSceneMgr::Get_Instance()->Get_CurrentScene() == CSceneMgr::SCENEID::SCENE_STAGE)
		{
			//태그로 구분한다.
			switch (eTag)
			{
			case SAVEDATA::PILLBUG:
			{
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CPillBug>::Create(tInfo.fX, tInfo.fY));
				break;

			}
			case SAVEDATA::CHASER:
			{
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CChaser>::Create(tInfo.fX, tInfo.fY));
				break;
			}
			case SAVEDATA::TERRAIN:
			{
				CObj* pObj = CAbstractFactory<CMyImage>::Create(tInfo.fX, tInfo.fY);
				pObj->Set_FrameKey(pFrameKey);
				pObj->Set_Size(tInfo.iCX, tInfo.iCY);
				dynamic_cast<CMyImage*>(pObj)->Set_Tag(eTag);
				m_vecImage.push_back(pObj);
				break;
			}
			case SAVEDATA::JUMPER:
			{
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CJumper>::Create(tInfo.fX, tInfo.fY));
				break;
			}
			case SAVEDATA::FLY:
			{
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CFly>::Create(tInfo.fX, tInfo.fY));
				break;
			}
			case SAVEDATA::HUSKGAURD:
			{
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CHuskGaurd>::Create(tInfo.fX, tInfo.fY));
				break;
			}
			case SAVEDATA::SPIKE:
			{
				CObjMgr::Get_Instance()->Add_Object(OBJID::TRAP, CAbstractFactory<CTrap>::Create(tInfo.fX, tInfo.fY, L"spike", 256, 97));
				break;
			}
			case SAVEDATA::FALSE_KNIGHT:
			{
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CFalseKnight>::Create(tInfo.fX, tInfo.fY));
				break;
			}
			case SAVEDATA::END:
				break;
			default:
				break;
			}
		}
		else
		{
			//만약 에디터 모드일 경우
			CObj* pObj = CAbstractFactory<CMyImage>::Create(tInfo.fX, tInfo.fY);
			pObj->Set_FrameKey(pFrameKey);
			pObj->Set_Size(tInfo.iCX, tInfo.iCY);
			dynamic_cast<CMyImage*>(pObj)->Set_Tag(eTag);
			m_vecImage.push_back(pObj);
		}

	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Image Load", L"Success", MB_OK);

}
