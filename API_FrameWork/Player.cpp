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
#include "CollisionMgr.h"
#include "MyTime.h"

CPlayer::CPlayer()
	: m_fDis(0.f), m_JumpVelo(0.f, -70)
{
	ZeroMemory(&m_tPosin, sizeof(m_tPosin));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{

	m_prvPos = Vector2(m_tInfo.fX, m_tInfo.fY);
	m_velocity = Vector2();

	m_tStat.m_fMaxHp = 100;
	m_tStat.m_fHp = m_tStat.m_fMaxHp;

	m_tInfo.fX = 400.f;
	m_tInfo.fY = 1000.f;
	m_tInfo.iCX = 75;
	m_tInfo.iCY = 75;

	m_fAngle = 0.f;
	m_fDis = 100.f;

	m_fSpeed = 5.f;

	m_curJumpVelo = m_JumpVelo;
	m_Gravity = Vector2(0, 5.f);

	m_eCurState = STATE::IDLE;
	m_ePrvState = STATE::END;

	m_eTag = OBJTAG::PLAYER;


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_left.bmp", L"player_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_right.bmp", L"player_right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/player_hit.bmp", L"player_hit");

	////ü�� UI
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/healthUI_skull.bmp", L"healthUI_skull");
	//CObj* healthUI = CAbstractFactory<CMyImage>::Create(500, 500, L"healthUI_skull", 50, 17);
	//CImageMgr::Get_Instance()->Add_Image(healthUI);



	memcpy(m_pFrameKey, L"player_right", DIR_LEN);

}

int CPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	//��ŸŸ�� ���ϱ�
	m_fDeltaTime = CMyTime::Get_Instance()->Get_DeltaTime();
	//�����ӻ����� ������ �ʹ� ũ�� �ȵ�.
	if (m_fDeltaTime > 0.15f)
		m_fDeltaTime = 0.15f;


	Vector2 currPos(m_tInfo.fX, m_tInfo.fY);

	////�ӵ� ���ϱ�
	m_velocity = currPos - m_prvPos;

	//���� ��ġ�� ����
	m_prvPos = currPos;

	if (!m_bJump)
	{
		//�����϶��� ���� �ݸ��� üũ����
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);

		static float CurrGravity = 5.f;

		//�߶��϶� �ִϸ��̼� ����
		if (collision == CTileMgr::END)
		{
			m_eCurState = STATE::FALL;

		}
		else
		{
			CurrGravity = m_Gravity.fY;
			m_eCurState = STATE::IDLE;
		}

		m_tInfo.fY += m_Gravity.fY;


	}


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

#pragma region DEBUG

	switch (m_eCurState)
	{
	case CPlayer::IDLE:
		m_debug = L"�޽�";
		break;
	case CPlayer::WALK:
		m_debug = L"�ȱ�";
		break;
	case CPlayer::ATTACK:
		m_debug = L"����";
		break;
	case CPlayer::HIT:
		m_debug = L"�ǰ�";
		break;
	case CPlayer::JUMP:
		m_debug = L"����";
		break;
	case CPlayer::FALL:
		m_debug = L"�߶�";
		break;
	case CPlayer::DEAD:
		m_debug = L"����";
		break;
	case CPlayer::END:
		m_debug = L"��";
		break;
	default:
		break;
	}
	TCHAR		szBuff[32] = L"���� : ";
	lstrcat(szBuff, m_debug);
	TextOut(_DC, 500, 500, szBuff, lstrlen(szBuff));

#pragma endregion
	//SelectObject(_DC, GetStockObject(WHITE_PEN));
	//Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	TextOut(_DC, 500, 200, m_debug, lstrlen(m_debug));


#pragma region UI

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"healthUI_skull");
	if (m_eCurState == STATE::HIT)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"player_hit");

		GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY
			, 120, 70, hMemDC, 0, 0, 120, 70
			, RGB(30, 30, 30));
	}

#pragma endregion



}

void CPlayer::Release()
{
}



void CPlayer::OnCollisionEnter(CObj* _pOther, float _fX, float _fY)
{

	static int cnt = 0;
	
	if (_pOther->Get_Tag() == OBJTAG::MONSTER)
	{
		++cnt;
	}


}

void CPlayer::OnCollisionStay(CObj * _pOther, float _fX, float _fY)
{
	static int cnt = 0;

	if (_pOther->Get_Tag() == OBJTAG::MONSTER)
	{
		++cnt;
	}

}

void CPlayer::OnCollisionExit(CObj * _pOther, float _fX, float _fY)
{
	static int cnt2 = 0;

	if (_pOther->Get_Tag() == OBJTAG::MONSTER)
	{
		++cnt2;
	}
}



void CPlayer::Key_Check()
{


	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		memcpy(m_pFrameKey, L"player_left", DIR_LEN);
		if (m_eCurState != STATE::FALL)
			m_eCurState = STATE::WALK;
		m_tInfo.fX -= m_fSpeed;
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		memcpy(m_pFrameKey, L"player_right", DIR_LEN);
		if (m_eCurState != STATE::FALL)
			m_eCurState = STATE::WALK;
		m_tInfo.fX += m_fSpeed;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		if (m_eCurState != STATE::FALL)
			m_bJump = true;
	}

	//����Ű ������ ���ݻ���
	if (CKeyMgr::Get_Instance()->Key_Pressing('X'))
		m_eCurState = STATE::ATTACK;

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
			m_tFrame.bLoop = true;
			break;
		}
		case CPlayer::WALK:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case CPlayer::ATTACK:
		{

		}
		case CPlayer::HIT:
		{

			break;
		}
		case CPlayer::JUMP:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}
		case CPlayer::FALL:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
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
		//�����̽� ������������ �� ���� ����
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
			m_curJumpVelo.fY -= 3.7f;

		//�߶��̸�
		if (m_curJumpVelo.fY > 0)
		{
			m_eCurState = STATE::FALL;
		}
		else
			m_eCurState = STATE::JUMP;

		//�߷�����
		m_curJumpVelo += m_Gravity;

		// * m_fDeltaTime�� ���ָ� 1�ʿ� m_curJumpVelo��ŭ �̵��Ѵ�.
		m_tInfo.fX += m_curJumpVelo.fX * m_fDeltaTime;
		m_tInfo.fY += m_curJumpVelo.fY * m_fDeltaTime;

		//���������̰�, �ٴڰ� �浹�̸� ��������
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);

		if (collision == CTileMgr::BOTTOM)
		{
			//���� �ʱ�ȭ
			m_curJumpVelo = m_JumpVelo;
			m_bJump = false;
		}

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
	if (iOffSetX >(m_tInfo.fX + iScrollX))
		CScrollMgr::Get_Instance()->Set_Scroll_X(iOffSetX - (m_tInfo.fX + iScrollX));

	if (iOffSetY < (m_tInfo.fY + iScrollY))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(iOffSetY - (m_tInfo.fY + iScrollY));
	if (iOffSetY >(m_tInfo.fY + iScrollY))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(iOffSetY - (m_tInfo.fY + iScrollY));
}


