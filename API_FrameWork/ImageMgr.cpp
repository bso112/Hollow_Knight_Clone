#include "stdafx.h"
#include "ImageMgr.h"
#include "MyImage.h"
#include "BmpMgr.h"
#include "PillBug.h"
#include "ScrollMgr.h"
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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/flatform1.bmp", L"flatform1");
	pObj = CAbstractFactory<CMyImage>::Create(100, 100, L"flatform1", 281, 145);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/wall1.bmp", L"wall1");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"wall1", 187, 281);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);


	//몬스터 (위치랑 몬스터 종류만 저장하면 된다)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/pill_bug.bmp", L"pill_bug");
	pObj = CAbstractFactory<CMyImage>::Create(300, 300, L"pill_bug", 70, 70);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::PILLBUG);
	m_vecImage.push_back(pObj);

	m_ivecImageSize = m_vecImage.size();

}

void CImageMgr::Update()
{
}

void CImageMgr::Late_Update()
{
}

void CImageMgr::Render(HDC _DC)
{
	//에디터에서 만든 인스턴스만 렌더한다.
	if (m_ivecImageSize < m_vecImage.size())
	{
		for (int i = m_ivecImageSize; i < m_vecImage.size(); ++i)
			m_vecImage[i]->Render(_DC);
	}
}

void CImageMgr::Release()
{
	for_each(m_vecImage.begin(), m_vecImage.end(), Safe_Delete<CObj*>);
	m_vecImage.clear();
}

CObj* CImageMgr::Get_Image(int _iIndex)
{
	//유효성검사
	if (_iIndex < 0 || _iIndex >= m_ivecImageSize)
		return nullptr;

	CMyImage* newImage = new CMyImage(*dynamic_cast<CMyImage*>(m_vecImage[_iIndex]));

	return newImage;
}

const TCHAR * CImageMgr::Get_ImageName(int _iIndex)
{
	//유효성검사
	if (_iIndex < 0 || _iIndex >= m_ivecImageSize)
		return nullptr;

	return m_vecImage[_iIndex]->Get_pFrameKey();
}



void CImageMgr::Drag_Image(POINT & _pt)
{
	for (auto& pImage : m_vecImage)
	{
		if (PtInRect(&pImage->Get_Rect(), _pt))
		{
			pImage->Set_Pos((float)_pt.x, (float)_pt.y);
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
	int iDrawID = 0;

	//에디터에서 만든 인스턴스만 저장한다.
	if (m_ivecImageSize < m_vecImage.size())
	{
		for (int i = m_ivecImageSize; i < m_vecImage.size(); ++i)
		{
			//이미지의 위치, 크기를 저장한다.
			WriteFile(hFile, &m_vecImage[i]->Get_INFO(), sizeof(INFO), &dwByte, NULL);
			//이미지의 태그 저장.
			WriteFile(hFile, &dynamic_cast<CMyImage*>(m_vecImage[i])->Get_Tag(), sizeof(SAVEDATA::TAG), &dwByte, NULL);
			//프레임 키 저장
			WriteFile(hFile, m_vecImage[i]->Get_pFrameKey(), sizeof(TCHAR) *DIR_LEN, &dwByte, NULL);
		}
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
	int iDrawID = 0;


	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &eTag, sizeof(SAVEDATA::TAG), &dwByte, NULL);
		ReadFile(hFile, pFrameKey, sizeof(TCHAR) * DIR_LEN, &dwByte, NULL);

		if (0 == dwByte)
			break;
		//태그로 구분한다.
		switch (eTag)
		{
		case SAVEDATA::PILLBUG:
		{
			CObj* pObj = CAbstractFactory<CMyImage>::Create(tInfo.fX, tInfo.fY);
			pObj->Set_FrameKey(pFrameKey);
			pObj->Set_Size(tInfo.iCX, tInfo.iCY);
			m_vecImage.push_back(pObj);
			//CAbstractFactory<CPillBug>::Create(tInfo.fX, tInfo.fY);
			break;

		}
		case SAVEDATA::TERRAIN:
		{

			CObj* pObj = CAbstractFactory<CMyImage>::Create(tInfo.fX, tInfo.fY);
			pObj->Set_FrameKey(pFrameKey);
			pObj->Set_Size(tInfo.iCX, tInfo.iCY);
			m_vecImage.push_back(pObj);
			break;
		}
		case SAVEDATA::END:
			break;
		default:
			break;
		}

	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Image Load", L"Success", MB_OK);

}
