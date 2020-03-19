#include "stdafx.h"
#include "LineMgr.h"
#include "Line.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
}


CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
	Load_Line();
}

void CLineMgr::Render(HDC _DC)
{
	for (auto& pLine : m_listLine)
		pLine->Render(_DC);
}

void CLineMgr::Release()
{
	for_each(m_listLine.begin(), m_listLine.end(), Safe_Delete<CLine*>);
	m_listLine.clear();
}

bool CLineMgr::Collision_Line(float _x, float* _y)
{
	if (m_listLine.empty())
		return false;

	CLine*	pTarget = nullptr;

	for (auto& pLine : m_listLine)
	{
		if (_x >= pLine->Get_Info().tLeftPos.fX
			&& _x <= pLine->Get_Info().tRightPos.fX)
			pTarget = pLine;
	}

	if (!pTarget)
		return false;

	float	x1 = pTarget->Get_Info().tLeftPos.fX;
	float	x2 = pTarget->Get_Info().tRightPos.fX;
	float	y1 = pTarget->Get_Info().tLeftPos.fY;
	float	y2 = pTarget->Get_Info().tRightPos.fY;

	*_y = ((y2 - y1) / (x2 - x1)) * (_x - x1) + y1;
	return true;
}

void CLineMgr::Load_Line()
{
	HANDLE hFile = CreateFile(__T("../Data/Line.dat"), GENERIC_READ, NULL, NULL
		, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, __T("Load File"), __T("Fail"), MB_OK);
		return;
	}
	LINEINFO tTemp = {};
	DWORD dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(LINEINFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_listLine.emplace_back(new CLine(tTemp));
	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Load Success", L"Success", MB_OK);
}
