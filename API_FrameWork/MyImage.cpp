#include "stdafx.h"
#include "MyImage.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CMyImage::CMyImage()
	:m_eTag(SAVEDATA::END)
{
}


CMyImage::~CMyImage()
{
}



void CMyImage::Initialize()
{
}

int CMyImage::Update()
{
	return 0;
}

void CMyImage::Late_Update()
{
}

void CMyImage::Render(HDC _DC)
{
	int iScrollX = CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = CScrollMgr::Get_Instance()->Get_Scroll_Y();

	Update_Rect();
	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX ,m_tRect.top + iScrollY, m_tInfo.iCX, m_tInfo.iCY, memDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY, RGB(30, 30, 30));
}

void CMyImage::Release()
{
}

