#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "ScrewBullet.h"
#include "ObjMgr.h"
#include "GuideBullet.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "MyImage.h"
#include "ImageMgr.h"

CPlayer::CPlayer()
	: m_fDis(0.f), m_bGround(false)
{
	ZeroMemory(&m_tPosin, sizeof(m_tPosin));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{

	m_tStat.m_fMaxHp = 100;
	m_tStat.m_fHp = m_tStat.m_fMaxHp;

	m_tInfo.fX = 400.f;
	m_tInfo.fY = 1500.f;
	m_tInfo.iCX = 75;
	m_tInfo.iCY = 75;

	m_fAngle = 0.f;
	m_fDis = 100.f;

	m_fSpeed = 5.f;

	m_bJump = false;
	m_fJumpPower = 15.f;
	m_fJumpAccel = 0.f;

	m_eCurState = STATE::IDLE;
	m_ePrvState = STATE::END;

	m_eTag = OBJTAG::PLAYER;


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_left.bmp", L"player_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_right.bmp", L"player_right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/player_hit.bmp", L"player_hit");

	////체력 UI
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/healthUI_skull.bmp", L"healthUI_skull");
	//CObj* healthUI = CAbstractFactory<CMyImage>::Create(500, 500, L"healthUI_skull", 50, 17);
	//CImageMgr::Get_Instance()->Add_Image(healthUI);
	
	

	memcpy(m_pFrameKey, L"player_right", DIR_LEN);

}

int CPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

#pragma region 체력바 설정

	//float fill = m_tStat.m_fHp / m_tStat.m_fMaxHp;

	//if (fill > 0.75)
	//{
	//	m_tHealthUI.iFrameStart = 0;
	//	m_tHealthUI.iFrameEnd = 0;
	//	m_tHealthUI.iFrameScene = 0;
	//	m_tHealthUI.dwFrameTime = GetTickCount();
	//	m_tHealthUI.dwFrameSpeed = 1000;
	//}
	//else if (fill > 0.5)
	//{
	//	m_tHealthUI.iFrameStart = 1;
	//	m_tHealthUI.iFrameEnd = 1;
	//}
	//else if (fill > 0.25)
	//{
	//	m_tHealthUI.iFrameStart = 2;
	//	m_tHealthUI.iFrameEnd = 2;
	//}
	//else
	//{
	//	m_tHealthUI.iFrameStart = 3;
	//	m_tHealthUI.iFrameEnd = 3;
	//}



#pragma endregion

	

	//상시중력
	static float fAccel = 0;
	float fY = 0.f;
	if (!m_bJump && !CTileMgr::Get_Instance()->IsStepOnTile(this, fY))
	{
		m_tInfo.fY -= .5 * fAccel
			- 9.8f * fAccel * fAccel * 0.5f;
		fAccel += 0.2f;
	}
	else
		fAccel = 0.f;


	Key_Check();
	OffSet();
	Jumping();
	Scene_Change();
	Move_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	m_tPosin.x = LONG(m_tInfo.fX + (cosf(m_fAngle * PI / 180.f) * m_fDis));
	m_tPosin.y = LONG(m_tInfo.fY - (sinf(m_fAngle * PI / 180.f) * m_fDis));
}

void CPlayer::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY
		, m_tInfo.iCX, m_tInfo.iCY, hMemDC, m_tInfo.iCX * m_tFrame.iFrameStart, m_tInfo.iCY *m_tFrame.iFrameScene, m_tInfo.iCX, m_tInfo.iCY
		, RGB(30, 30, 30));

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"healthUI_skull");
	

	if (m_eCurState == STATE::HIT)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"player_hit");

		GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY
			, 120, 70, hMemDC, 0, 0, 120, 70
			, RGB(30, 30, 30));
	}


}

void CPlayer::Release()
{
}



void CPlayer::OnCollisionEnter(CObj* _pOther, float _fX, float _fY)
{
	float fX = _fX;
	float fY = _fY;
	CObj* tile = _pOther;

	//타일과 부딪혔을때는 더이상 나아가지 못한다.
	if (_pOther->Get_Tag() == OBJTAG::TILE)
	{

		//땅에 있다. 따라서 점프 초기화(이거 점프에 두면 안됨..)

		//X축 충돌영역이 더 많으면 상하로 충돌한 것이다.
		if (fX < fY)
		{
			//만약 타일이 위에 있다면
			if (tile->Get_INFO().fY < m_tInfo.fY)
				m_tInfo.fY = tile->Get_Rect().bottom + (m_tInfo.iCY >> 1);
			//만약 타일이 아래 있다면
			else
			{
				//땅에 있다. 따라서 점프 초기화(이거 점프에 두면 안됨..)
				m_bJump = false;
				m_fJumpAccel = 0.f;

				m_tInfo.fY = tile->Get_Rect().top - (m_tInfo.iCY >> 1);
			}
		}
		else
		{
			////만약 타일이 왼쪽에 있다면
			//if (tile->Get_INFO().fX < m_tInfo.fX)
			//	m_tInfo.fX = tile->Get_Rect().right + (m_tInfo.iCX >> 1);
			//else
			//	//만약 타일이 오른쪽에 있다면
			//	m_tInfo.fX = tile->Get_Rect().left - (m_tInfo.iCX >> 1);
		}
	}
	else if (_pOther->Get_Tag() == OBJTAG::MONSTER)
	{
		m_eCurState = STATE::HIT;
	}

}

void CPlayer::OnCollisionEnter(CObj * _pOther)
{

}



void CPlayer::Key_Check()
{
	if (m_eCurState != STATE::HIT)
		//기본 IDEL상태
		m_eCurState = STATE::IDLE;



	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		memcpy(m_pFrameKey, L"player_left", DIR_LEN);
		m_eCurState = STATE::WALK;
		m_tInfo.fX -= m_fSpeed;
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		memcpy(m_pFrameKey, L"player_right", DIR_LEN);
		m_eCurState = STATE::WALK;
		m_tInfo.fX += m_fSpeed;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		m_bJump = true;
	}

	//공격키 누르면 공격상태
	if (CKeyMgr::Get_Instance()->Key_Pressing('X'))
		m_eCurState = STATE::ATTACK;

	//점프상태면 state는 계속 점프
	if (m_bJump)
	{
		m_eCurState = STATE::JUMP;
	}
}

void CPlayer::Scene_Change()
{
	if (m_eCurState != m_ePrvState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case CPlayer::WALK:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case CPlayer::ATTACK:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case CPlayer::HIT:
		{

			break;
		}
		case CPlayer::JUMP:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case CPlayer::DEAD:
			break;
		case CPlayer::END:
			break;
		default:
			break;
		}

		m_ePrvState = m_eCurState;
	}
}

void CPlayer::Jumping()
{

	if (m_bJump)
	{
		m_tInfo.fY -= m_fJumpPower * m_fJumpAccel
			- 9.8f * m_fJumpAccel * m_fJumpAccel * 0.5f;
		m_fJumpAccel += 0.2f;
	}

}

void CPlayer::OffSet()
{
	int iOffSetX = WINCX >> 1;
	int iOffSetY = WINCY >> 1;
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	if (iOffSetX < (m_tInfo.fX + iScrollX))
		CScrollMgr::Get_Instance()->Set_Scroll_X(iOffSetX - (m_tInfo.fX + iScrollX));
	if (iOffSetX > (m_tInfo.fX + iScrollX))
		CScrollMgr::Get_Instance()->Set_Scroll_X(iOffSetX - (m_tInfo.fX + iScrollX));

	if (iOffSetY < (m_tInfo.fY + iScrollY))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(iOffSetY - (m_tInfo.fY + iScrollY));
	if (iOffSetY > (m_tInfo.fY + iScrollY))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(iOffSetY - (m_tInfo.fY + iScrollY));
}


