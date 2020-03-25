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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Flatform/flatform1.bmp", L"flatform1");
	pObj = CAbstractFactory<CMyImage>::Create(100, 100, L"flatform1", 281, 145);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Flatform/flatform2.bmp", L"flatform2");
	pObj = CAbstractFactory<CMyImage>::Create(100, 100, L"flatform2", 120, 66);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Flatform/flatform3.bmp", L"flatform3");
	pObj = CAbstractFactory<CMyImage>::Create(100, 100, L"flatform3", 93, 59);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Wall/clif1.bmp", L"clif1");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"clif1", 183, 282);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Wall/clif2.bmp", L"clif2");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"clif2", 183, 282);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Wall/clif3.bmp", L"clif3");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"clif3", 282, 183);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Wall/black.bmp", L"black");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"black", 100, 100);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Wall/black2.bmp", L"black2");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"black2", 300, 300);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Ground/ground1.bmp", L"ground1");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"ground1", 175, 159);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Ground/ground2.bmp", L"ground2");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"ground2", 396, 178);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Ground/ground3.bmp", L"ground3");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"ground3", 345, 176);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Ground/ground.bmp", L"ground");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"ground", 450, 114);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Ground/spike.bmp", L"spike");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"spike", 256, 97);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Egg/egg1.bmp", L"egg1");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"egg1", 258, 197);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Roof/roof1.bmp", L"roof1");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"roof1", 777, 194);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Roof/roof2.bmp", L"roof2");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"roof2", 709, 205);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Roof/root1.bmp", L"root3");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"root3", 259, 254);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);


	//몬스터 (위치랑 몬스터 종류만 저장하면 된다)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/pill_bug.bmp", L"pill_bug");
	pObj = CAbstractFactory<CMyImage>::Create(300, 300, L"pill_bug", 140, 70);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::PILLBUG);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/chaser_left.bmp", L"chaser_left");
	pObj = CAbstractFactory<CMyImage>::Create(300, 300, L"chaser_left", 100, 100);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::CHASER);
	m_vecImage.push_back(pObj);

	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/jumper_left.bmp", L"jumper_left");
	pObj = CAbstractFactory<CMyImage>::Create(300, 300, L"jumper_left", 200, 200);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::JUMPER);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/jumper_right.bmp", L"jumper_right");
	pObj = CAbstractFactory<CMyImage>::Create(300, 300, L"jumper_right", 200, 200);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::JUMPER);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/fly_left.bmp", L"fly_left");
	pObj = CAbstractFactory<CMyImage>::Create(300, 300, L"fly_left", 120, 115);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::FLY);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/fly_right.bmp", L"fly_right");
	pObj = CAbstractFactory<CMyImage>::Create(300, 300, L"fly_right", 120, 115);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::FLY);
	m_vecImage.push_back(pObj);

}

void CImageMgr::Update()
{
}

void CImageMgr::Late_Update()
{
}

void CImageMgr::Render(HDC _DC)
{

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
	if (_iIndex < 0 || _iIndex >= m_vecImage.size())
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
	HANDLE hFile = CreateFile(L"../Data/Background3.dat", GENERIC_WRITE
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
	HANDLE hFile = CreateFile(L"../Data/Background3.dat", GENERIC_READ
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
