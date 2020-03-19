#include "stdafx.h"
#include "MyEdit.h"
#include "Line.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"

CMyEdit* CMyEdit::m_pInstance = nullptr;

CMyEdit::CMyEdit()
{
	ZeroMemory(m_tLinePos, sizeof(m_tLinePos));
}


CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
}

int CMyEdit::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_Scroll_X(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_Scroll_X(-5.f);


	Add_Line();

	if (CKeyMgr::Get_Instance()->Key_Down('A'))
		Save_Line();
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
		Load_Line();

	return 0;
}

void CMyEdit::Late_Update()
{
}

void CMyEdit::Render(HDC _DC)
{
	for (auto& pLine : m_listLine)
		pLine->Render(_DC);
}

void CMyEdit::Release()
{
	for_each(m_listLine.begin(), m_listLine.end(), Safe_Delete<CLine*>);
	m_listLine.clear();
}

void CMyEdit::Save_Line()
{
	HANDLE hFile = CreateFile(L"../Data/Line.dat", GENERIC_WRITE, NULL, NULL
		, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Save File", L"Fail", MB_OKCANCEL);
		return;
	}

	DWORD dwByte = 0;
	for (auto& pLine : m_listLine)
	{
		WriteFile(hFile, &pLine->Get_Info(), sizeof(LINEINFO), &dwByte, nullptr);
	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Save Success", L"Success", MB_OKCANCEL);
}

void CMyEdit::Load_Line()
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

void CMyEdit::Add_Line()
{
	POINT	pt = {};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= (LONG)(CScrollMgr::Get_Instance()->Get_Scroll_X());

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if (!m_tLinePos[0].fX && !m_tLinePos[0].fY)
		{
			m_tLinePos[0].fX = (float)pt.x;
			m_tLinePos[0].fY = (float)pt.y;
		}
		else
		{
			m_tLinePos[1].fX = (float)pt.x;
			m_tLinePos[1].fY = (float)pt.y;

			m_listLine.emplace_back(new CLine(m_tLinePos[0], m_tLinePos[1]));

			//// ##1. 선들을 따로 그린다.
			//ZeroMemory(m_tLinePos, sizeof(m_tLinePos));

			// ##2. 선들을 이어 그린다.
			m_tLinePos[0].fX = m_tLinePos[1].fX;
			m_tLinePos[0].fY = m_tLinePos[1].fY;
		}
	}
}
