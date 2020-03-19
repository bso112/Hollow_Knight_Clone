#include "stdafx.h"
#include "MenuPointer.h"
#include "BmpMgr.h"

CMenuPointer::CMenuPointer()
{
}


CMenuPointer::~CMenuPointer()
{
}

void CMenuPointer::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/menu_pointer.bmp", L"menu_pointer");

	m_tInfo.iCX = 72;
	m_tInfo.iCY = 70;


	m_pFrameKey = L"menu_pointer";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 10;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();



}

int CMenuPointer::Update()
{
	Update_Rect();
	Move_Frame();
	return 0;
}

void CMenuPointer::Late_Update()
{
}

void CMenuPointer::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	//BitBlt(_DC
	//	, (int)m_tRect.left, (int)m_tRect.top
	//	, m_tInfo.iCX, m_tInfo.iCY
	//	, hMemDC
	//	, m_tFrame.iFrameStart * m_tInfo.iCX, m_tFrame.iFrameScene * m_tInfo.iCY
	//	, SRCCOPY);
	////

	GdiTransparentBlt(_DC
		, (int)m_tRect.left, (int)m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * m_tInfo.iCX, m_tFrame.iFrameScene * m_tInfo.iCY
		, m_tInfo.iCX, m_tInfo.iCY
		, RGB(0, 0, 0));
}

void CMenuPointer::Release()
{
}
