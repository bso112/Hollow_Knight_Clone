#include "stdafx.h"
#include "Player.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "MyImage.h"
#include "ImageMgr.h"
#include "CollisionMgr.h"
#include "MyTime.h"
#include "Weapon.h"

CPlayer::CPlayer()
	: m_fDis(0.f), m_JumpVelo(0.f, -70), m_bCombo(false), m_ComboTimer(MAXDWORD)
{
	ZeroMemory(&m_tPosin, sizeof(m_tPosin));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{

	m_ComboWait = 0.2f;
	m_eJumpState = JUMP_STATE::STARTING;
	m_prvPos = Vector2(m_tInfo.fX, m_tInfo.fY);
	m_velocity = Vector2();
	m_eFront = FRONT::RIGHT;


	m_tStat.m_fMaxHp = 100;
	m_tStat.m_fHp = m_tStat.m_fMaxHp;

	m_tInfo.fX = 400.f;
	m_tInfo.fY = 1000.f;
	m_tInfo.iCX = 192;
	m_tInfo.iCY = 96;

	m_fAngle = 0.f;
	m_fDis = 100.f;

	m_fSpeed = 5.f;

	m_curJumpVelo = m_JumpVelo;
	m_Gravity = Vector2(0, 5.f);

	m_eCurState = STATE::IDLE;
	m_ePrvState = STATE::END;

	m_eTag = OBJTAG::PLAYER;


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_idle.bmp", L"hero_idle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_move.bmp", L"hero_move");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_jump_start.bmp", L"hero_jump_start");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_jump_highest.bmp", L"hero_jump_highest");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_jump_falling.bmp", L"hero_jump_falling");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_jump_landing.bmp", L"hero_jump_landing");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_hit.bmp", L"hero_hit");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_att_normal.bmp", L"hero_att_normal");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_att_normal2.bmp", L"hero_att_normal2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_att_down_to_top.bmp", L"hero_att_down_to_top");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_att_top_to_down.bmp", L"hero_att_top_to_down");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_dead.bmp", L"hero_dead");

	//���� ����Ʈ
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/effect/att_dt_eff.bmp", L"att_dt_eff");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/effect/att_n1_eff.bmp", L"att_n1_eff");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/effect/att_n2_eff.bmp", L"att_n2_eff");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/effect/att_td_eff.bmp", L"att_td_eff");


	//ü�� UI
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/healthUI_skull.bmp", L"healthUI_skull");
	//CObj* healthUI = CAbstractFactory<CMyImage>::Create(500, 500, L"healthUI_skull", 50, 17);
	//CImageMgr::Get_Instance()->Add_Image(healthUI);




	memcpy(m_pFrameKey, L"hero_idle", DIR_LEN);

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
		}

		m_tInfo.fY += m_Gravity.fY;


	}


	//�⺻ ���´� idle. ���⼭ idle�� �ٲ㵵 �ؿ��� ���¸� �ٲٸ� �� ���°� �ȴ�.
	bool isAnimEnd = Move_Frame();
	//if (isAnimEnd)
	//	m_eCurState = STATE::IDLE;
	Key_Check();
	OffSet();
	Jumping();

	Scene_Change();
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
		, m_tInfo.iCX, m_tInfo.iCY, hMemDC, m_tInfo.iCX * m_tFrame.iFrameScene, m_tInfo.iCY *m_tFrame.iFrameStart, m_tInfo.iCX, m_tInfo.iCY
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

	//SelectObject(_DC, GetStockObject(WHITE_PEN));
	//Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
#pragma endregion
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

void CPlayer::Attack()
{
	m_eCurState = STATE::ATTACK;


	//�޺� �Ǵ�
	if (m_ComboTimer + m_ComboWait * 1000 < GetTickCount())
	{
		m_bCombo = true;
	}
	else
		m_bCombo = false;

	m_ComboTimer = GetTickCount();

	CObj* weapon = nullptr;
	FRAME frame;

	switch (m_eFront)
	{
	case CPlayer::LEFT:
	{
		frame.iFrameStart = 0;
		frame.iFrameEnd = 1;
		frame.iFrameScene = 0;
		frame.dwFrameTime = GetTickCount();
		frame.dwFrameSpeed = 50;
		frame.bLoop = false;
		break;
	}
	case CPlayer::RIGHT:
	{
		frame.iFrameStart = 0;
		frame.iFrameEnd = 1;
		frame.iFrameScene = 1;
		frame.dwFrameTime = GetTickCount();
		frame.dwFrameSpeed = 50;
		frame.bLoop = false;
		break;
	}
	default:
		break;
	}



	//����Ʈ ����
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_eAttDir = ATT_DIR::UP;
		weapon = CAbstractFactory<CWeapon>::Create(m_tInfo.fX, m_tInfo.fY - (m_tInfo.iCY >> 1), L"att_dt_eff", frame, 192, 192);

	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_eAttDir = ATT_DIR::DOWN;
		weapon = CAbstractFactory<CWeapon>::Create(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), L"att_td_eff", frame, 192, 192);
	}
	else
	{
		//���� ����
		int iDir = m_eFront == FRONT::RIGHT ? 1 : -1;

		if (m_bCombo)
			weapon = CAbstractFactory<CWeapon>::Create(m_tInfo.fX + (m_tInfo.iCX >> 1) * iDir, m_tInfo.fY, L"att_n2_eff", frame, 192, 96);
		else
			weapon = CAbstractFactory<CWeapon>::Create(m_tInfo.fX + (m_tInfo.iCX >> 1) * iDir, m_tInfo.fY, L"att_n1_eff", frame, 192, 96);
	}

	if (weapon)
	{
		//2�ʵ��ȸ� ����
		dynamic_cast<CWeapon*>(weapon)->Set_Duration(0.1f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, weapon);
	}




}



void CPlayer::Key_Check()
{

	//���ʹ�ư ��������
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		//�߶��Ҷ��� �����̴� ��� ����
		if (m_eCurState != STATE::FALL)
			m_eCurState = STATE::WALK;
		//���ʺ���
		m_eFront = FRONT::LEFT;
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		m_tInfo.fX -= m_fSpeed;

	if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT))
		m_eCurState = STATE::IDLE;


	//�����ʹ�ư ��������
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		if (m_eCurState != STATE::FALL)
			m_eCurState = STATE::WALK;
		//�����ʺ���
		m_eFront = FRONT::RIGHT;
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		m_tInfo.fX += m_fSpeed;

	if (CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT))
		m_eCurState = STATE::IDLE;

	//����
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		if (m_eCurState != STATE::FALL)
			m_bJump = true;
		//���� ����
		m_eJumpState = JUMP_STATE::STARTING;
		m_eCurState = STATE::JUMP;
	}

	//����Ű ������ ����
	if (CKeyMgr::Get_Instance()->Key_Down('X'))
	{
		Attack();
	}

}

void CPlayer::Scene_Change()
{
	//���°� �ٲ� �ѹ��� ����
	if (m_eCurState != m_ePrvState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
		{
			memcpy(m_pFrameKey, L"hero_idle", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case CPlayer::WALK:
		{
			memcpy(m_pFrameKey, L"hero_move", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case CPlayer::ATTACK:
		{
			//�� ���� �Ʒ� ���� ����
			if (m_eAttDir == ATT_DIR::UP)
			{
				memcpy(m_pFrameKey, L"hero_att_down_to_top", DIR_LEN);
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iFrameScene = m_eFront;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.dwFrameSpeed = 40;
				m_tFrame.bLoop = false;

				//�ʱ�ȭ
				m_eAttDir = ATT_DIR::ATT_END;
			}
			else if (m_eAttDir == ATT_DIR::DOWN)
			{
				memcpy(m_pFrameKey, L"hero_att_top_to_down", DIR_LEN);
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iFrameScene = m_eFront;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.dwFrameSpeed = 40;
				m_tFrame.bLoop = false;

				//�ʱ�ȭ
				m_eAttDir = ATT_DIR::ATT_END;
			}
			//�޺���
			else if (m_bCombo)
			{
				//�޺����� ���
				memcpy(m_pFrameKey, L"hero_att_normal2", DIR_LEN);
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iFrameScene = m_eFront;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.dwFrameSpeed = 40;
				m_tFrame.bLoop = false;

				//2�� ������ ����.
				m_ComboTimer = GetTickCount();
			}
			else
			{
				//���� ���ݸ��
				memcpy(m_pFrameKey, L"hero_att_normal", DIR_LEN);
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iFrameScene = m_eFront;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.dwFrameSpeed = 40;
				m_tFrame.bLoop = false;
			}
		}
		case CPlayer::HIT:
		{

			break;
		}
		case CPlayer::FALL:
		{
			memcpy(m_pFrameKey, L"hero_jump_falling", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
		}
		case CPlayer::DEAD:
			break;
		case CPlayer::END:
			break;
		default:
			break;
		}

		//���°� �ٲ������ �ѹ��� ����������ϴ�. 
		m_ePrvState = m_eCurState;
	}


	//������ ���� �������� ����
	if (m_eCurState == STATE::JUMP && m_ePrvJumpState != m_eJumpState)
	{
		switch (m_eJumpState)
		{
		case CPlayer::STARTING:
		{
			memcpy(m_pFrameKey, L"hero_jump_start", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}
		case CPlayer::HIGHEST:
		{
			memcpy(m_pFrameKey, L"hero_jump_highest", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}
		case CPlayer::FALLING:
		{
			memcpy(m_pFrameKey, L"hero_jump_falling", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}
		case CPlayer::LANDING:
		{
			memcpy(m_pFrameKey, L"hero_jump_landing", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}
		case CPlayer::JUMP_END:
			break;
		default:
			break;
		}

		m_ePrvJumpState = m_eJumpState;

	}
}

void CPlayer::Jumping()
{

	if (m_bJump)
	{

		m_eCurState = STATE::JUMP;

		//�����̽� ������������ �� ���� ����
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
			m_curJumpVelo.fY -= 3.7f;		


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
			m_eJumpState = JUMP_STATE::LANDING;
			//���� �ʱ�ȭ
			m_bJump = false;
		}
		else if (m_curJumpVelo.fY > 0)
		{
			//�߶���
			m_eJumpState = JUMP_STATE::FALLING;
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
	if (iOffSetX > (m_tInfo.fX + iScrollX))
		CScrollMgr::Get_Instance()->Set_Scroll_X(iOffSetX - (m_tInfo.fX + iScrollX));

	if (iOffSetY < (m_tInfo.fY + iScrollY))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(iOffSetY - (m_tInfo.fY + iScrollY));
	if (iOffSetY > (m_tInfo.fY + iScrollY))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(iOffSetY - (m_tInfo.fY + iScrollY));
}


