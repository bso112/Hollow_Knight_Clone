#include "stdafx.h"
#include "MyImage.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
#include "MyTime.h"
CMyImage::CMyImage()
	:m_eTag(SAVEDATA::END), m_fDuration(FLT_MAX), m_bHorizontal(false)
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

	//이동
	if (m_dwForceTimer + m_fForceTime * 1000 > GetTickCount())
	{
		m_tInfo.fX += m_velocity.fX * CMyTime::Get_Instance()->Get_DeltaTime();
		m_tInfo.fY += m_velocity.fY * CMyTime::Get_Instance()->Get_DeltaTime();
	}
	else
	{
		m_fForceTime = 0.f;
		m_velocity.fX = 0;
		m_velocity.fY = 0;
	}


	Move_Frame();

	return OBJ_NOEVENT;
}

void CMyImage::Late_Update()
{
	if (CSceneMgr::Get_Instance()->Get_CurrentScene() == CSceneMgr::SCENEID::SCENE_EDIT)
		return;
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
	if (m_bHorizontal)
	{
		GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY
			, m_tInfo.iCX, m_tInfo.iCY, memDC, m_tInfo.iCX * m_tFrame.iFrameStart, m_tInfo.iCY *m_tFrame.iFrameScene, m_tInfo.iCX, m_tInfo.iCY
			, RGB(30, 30, 30));
	}
	else
	{

		GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY
			, m_tInfo.iCX, m_tInfo.iCY, memDC, m_tInfo.iCX * m_tFrame.iFrameScene, m_tInfo.iCY *m_tFrame.iFrameStart, m_tInfo.iCX, m_tInfo.iCY
			, RGB(30, 30, 30));
	}
}

void CMyImage::Release()
{
}


void CMyImage::Add_Force(Vector2 _vDir, float _fForce, float _fTime)
{
	//히트타임 끝나고 밀려남
	m_dwForceTimer = GetTickCount();
	m_velocity = _vDir * _fForce;
	m_fForceTime = _fTime;
}
