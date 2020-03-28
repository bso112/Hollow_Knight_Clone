#include "stdafx.h"
#include "Mouse.h"
#include "BmpMgr.h"
CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/cursor.bmp", L"cursor");
	memcpy(m_pFrameKey, L"cursor", DIR_LEN);
}

int CMouse::Update()
{
	// GetCursorPos() 
	// ���콺 Ŀ���� ��ġ ��ǥ�� ������ �Լ��̴�.

	// ScreenToClient()
	// ��ü ��ũ�� �����ۼ� Ŭ���̾� ��ǥ�� ��ȯ���ִ� �Լ��̴�.
	// ������� ���� ��� Ŭ���̾� ��ǥ�� �ƴ� ������ ��ü ������ ��ǥ�� ����� �ȴ�.

	POINT	pt = {};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	ShowCursor(false);

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;
	
	Update_Rect();

	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC, (int)m_tInfo.fX, (int)m_tInfo.fY, 37, 37, hMemDC, 0, 0, 37, 37, RGB(30, 30, 30));

}

void CMouse::Release()
{
}
