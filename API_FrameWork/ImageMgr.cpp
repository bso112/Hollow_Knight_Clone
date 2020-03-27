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
	//맵 작성에 쓰일 이미지들

	CObj* pObj;

	//지형지물
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_backlayer1.bmp", L"tutorial_backlayer1");
	pObj = CAbstractFactory<CMyImage>::Create(9448>>1, 2160>>1, L"tutorial_backlayer1", 9448, 2160);
	m_vecBackground.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_backlayer2.bmp", L"tutorial_backlayer2");
	pObj = CAbstractFactory<CMyImage>::Create(9448 >> 1, 2160 >> 1, L"tutorial_backlayer2", 9448, 2160);
	m_vecBackground.push_back(pObj);


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Field/Tutorial/tutorial_mainlayer.bmp", L"tutorial_mainlayer");
	pObj = CAbstractFactory<CMyImage>::Create(9448 >> 1, 2160 >> 1, L"tutorial_mainlayer", 9448, 2160);
	m_vecBackground.push_back(pObj);


}

void CImageMgr::Update()
{
}

void CImageMgr::Late_Update()
{
}

void CImageMgr::Render(HDC _DC)
{

	for (auto& img : m_vecBackground)
		img->Render(_DC);

	for (auto& img : m_vecImageInstance)
		img->Render(_DC);



}

void CImageMgr::Remove_Image(POINT _pt)
{
	auto& iter = m_vecImageInstance.begin();
	auto& iter_end = m_vecImageInstance.end();
	for (; iter != iter_end;)
	{
		if (PtInRect(&(*iter)->Get_Rect(), _pt))
		{
			iter = m_vecImageInstance.erase(iter);
			iter_end = m_vecImageInstance.end();

		}
		else
			++iter;
	}

}

void CImageMgr::Release()
{
	for_each(m_vecImage.begin(), m_vecImage.end(), Safe_Delete<CObj*>);
	m_vecImage.clear();

	for_each(m_vecImageInstance.begin(), m_vecImageInstance.end(), Safe_Delete<CObj*>);
	m_vecImageInstance.clear();

	for_each(m_vecBackground.begin(), m_vecBackground.end(), Safe_Delete<CObj*>);
	m_vecBackground.clear();
}

CObj* CImageMgr::Get_Image(int _iIndex)
{
	//유효성검사
	if (_iIndex < 0 || (size_t)_iIndex >= m_vecImage.size())
		return nullptr;

	CMyImage* newImage = new CMyImage(*dynamic_cast<CMyImage*>(m_vecImage[_iIndex]));

	return newImage;
}

const TCHAR * CImageMgr::Get_ImageName(int _iIndex)
{
	//유효성검사
	if (_iIndex < 0 || (size_t)_iIndex >= m_vecImage.size())
		return nullptr;

	return m_vecImage[_iIndex]->Get_pFrameKey();
}



void CImageMgr::Drag_Image(POINT & _pt)
{
	for (auto& pImage : m_vecImageInstance)
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

	for (auto& img : m_vecImageInstance)
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
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER,  CAbstractFactory<CPillBug>::Create(tInfo.fX, tInfo.fY));
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
				m_vecImageInstance.push_back(pObj);
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
			m_vecImageInstance.push_back(pObj);
		}

	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Image Load", L"Success", MB_OK);

}
