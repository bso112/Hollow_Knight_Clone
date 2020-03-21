#include "stdafx.h"
#include "ImageMgr.h"
#include "MyImage.h"
#include "BmpMgr.h"
#include "PillBug.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
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
	//�� �ۼ��� ���� �̹�����

	CObj* pObj;

	//��������
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/flatform1.bmp", L"flatform1");
	pObj = CAbstractFactory<CMyImage>::Create(100, 100, L"flatform1", 281, 145);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/wall1.bmp", L"wall1");
	pObj = CAbstractFactory<CMyImage>::Create(200, 200, L"wall1", 187, 281);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::TERRAIN);
	m_vecImage.push_back(pObj);


	//���� (��ġ�� ���� ������ �����ϸ� �ȴ�)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/pill_bug.bmp", L"pill_bug");
	pObj = CAbstractFactory<CMyImage>::Create(300, 300, L"pill_bug", 70, 70);
	dynamic_cast<CMyImage*>(pObj)->Set_Tag(SAVEDATA::PILLBUG);
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

void CImageMgr::Release()
{
	for_each(m_vecImage.begin(), m_vecImage.end(), Safe_Delete<CObj*>);
	m_vecImage.clear();

	for_each(m_vecImageInstance.begin(), m_vecImageInstance.end(), Safe_Delete<CObj*>);
	m_vecImageInstance.clear();
}

CObj* CImageMgr::Get_Image(int _iIndex)
{
	//��ȿ���˻�
	if (_iIndex < 0 || (size_t)_iIndex >= m_vecImage.size())
		return nullptr;

	CMyImage* newImage = new CMyImage(*dynamic_cast<CMyImage*>(m_vecImage[_iIndex]));

	return newImage;
}

const TCHAR * CImageMgr::Get_ImageName(int _iIndex)
{
	//��ȿ���˻�
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

		//�̹����� ��ġ, ũ�⸦ �����Ѵ�.
		WriteFile(hFile, &img->Get_INFO(), sizeof(INFO), &dwByte, NULL);
		//�̹����� �±� ����.
		WriteFile(hFile, &dynamic_cast<CMyImage*>(img)->Get_Tag(), sizeof(SAVEDATA::TAG), &dwByte, NULL);
		//������ Ű ����
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

			
		//���� �������� ���̸�
		if (CSceneMgr::Get_Instance()->Get_CurrentScene() == CSceneMgr::SCENEID::SCENE_STAGE)
		{
			//�±׷� �����Ѵ�.
			switch (eTag)
			{
			case SAVEDATA::PILLBUG:
			{
				CAbstractFactory<CPillBug>::Create(tInfo.fX, tInfo.fY);
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
			case SAVEDATA::END:
				break;
			default:
				break;
			}
		}
		else
		{
			//���� ������ ����� ���
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