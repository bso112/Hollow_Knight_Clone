#include "stdafx.h"
#include "BmpMgr.h"
#include "MyBitmap.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;
CBmpMgr::CBmpMgr()
{
}


CBmpMgr::~CBmpMgr()
{
}

void CBmpMgr::Insert_Bmp(const TCHAR * _pFilePath, const TCHAR * _pImageKey)
{
	auto iter = find_if(m_mapBmp.begin(), m_mapBmp.end(), CStringCMP(_pImageKey));

	if (m_mapBmp.end() == iter)
	{
		CMyBitmap*	pBmp = new CMyBitmap;
		pBmp->Load_Bmp(_pFilePath);

		m_mapBmp.emplace(_pImageKey, pBmp);
	}
}

HDC CBmpMgr::Find_Image(const TCHAR * _pImageKey)
{
	auto iter = find_if(m_mapBmp.begin(), m_mapBmp.end(), CStringCMP(_pImageKey));

	if (m_mapBmp.end() == iter)
	{
		//TCHAR* msg = L"비트맵을 찾지 못했습니다.";
		//lstrcat(msg, _pImageKey);
		//MessageBox(g_hWnd, msg, L"비트맵매니저", MB_OK);
		return nullptr;
	}

	return iter->second->Get_MemDC();
}

void CBmpMgr::Release()
{
	for (auto& iter : m_mapBmp)
	{
		if (nullptr != iter.second)
		{
			Safe_Delete(iter.second);
		}
	}
	m_mapBmp.clear();
}
