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

CMyImage::CMyImage(CMyImage& _image)
{
	memcpy(m_pFrameKey, _image.Get_pFrameKey(), sizeof(TCHAR) * DIR_LEN);
	m_tInfo.iCX = _image.Get_INFO().iCX;
	m_tInfo.iCY = _image.Get_INFO().iCY;
	m_eTag = _image.Get_Tag();
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
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	Update_Rect();
	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//GdiTransparentBlt(_DC, m_tRect.left + iScrollX ,m_tRect.top + iScrollY, m_tInfo.iCX, m_tInfo.iCY, memDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY, RGB(30, 30, 30));
	
	GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY
		, m_tInfo.iCX, m_tInfo.iCY, memDC, m_tInfo.iCX * m_tFrame.iFrameStart, m_tInfo.iCY *m_tFrame.iFrameScene, m_tInfo.iCX, m_tInfo.iCY
		, RGB(30, 30, 30));
}

void CMyImage::Release()
{
}

