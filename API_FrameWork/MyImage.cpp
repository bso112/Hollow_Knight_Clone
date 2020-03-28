#include "stdafx.h"
#include "MyImage.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CMyImage::CMyImage()
	:m_eTag(SAVEDATA::END), m_fDuration(FLT_MAX)
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
	m_dwTimer = GetTickCount();
}

int CMyImage::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Move_Frame();

	return OBJ_NOEVENT;
}

void CMyImage::Late_Update()
{
	//지속시간동안 생존
	if (m_dwTimer + m_fDuration * 1000 < GetTickCount())
		m_bDead = true;
}

void CMyImage::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	Update_Rect();
	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	
	GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY
		, m_tInfo.iCX, m_tInfo.iCY, memDC, m_tInfo.iCX * m_tFrame.iFrameScene, m_tInfo.iCY *m_tFrame.iFrameStart, m_tInfo.iCX, m_tInfo.iCY
		, RGB(30, 30, 30));
}

void CMyImage::Release()
{
}

