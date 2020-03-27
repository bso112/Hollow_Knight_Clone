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

	//공격 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/effect/att_dt_eff.bmp", L"att_dt_eff");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/effect/att_n1_eff.bmp", L"att_n1_eff");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/effect/att_n2_eff.bmp", L"att_n2_eff");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/effect/att_td_eff.bmp", L"att_td_eff");


	//체력 UI
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/healthUI_skull.bmp", L"healthUI_skull");
	//CObj* healthUI = CAbstractFactory<CMyImage>::Create(500, 500, L"healthUI_skull", 50, 17);
	//CImageMgr::Get_Instance()->Add_Image(healthUI);




	memcpy(m_pFrameKey, L"hero_idle", DIR_LEN);

}

int CPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	//델타타임 구하기
	m_fDeltaTime = CMyTime::Get_Instance()->Get_DeltaTime();
	//프레임사이의 간격이 너무 크면 안됨.
	if (m_fDeltaTime > 0.15f)
		m_fDeltaTime = 0.15f;


	Vector2 currPos(m_tInfo.fX, m_tInfo.fY);

	////속도 구하기
	m_velocity = currPos - m_prvPos;

	//이전 위치를 갱신
	m_prvPos = currPos;

	if (!m_bJump)
	{
		//점프일때는 따로 콜리전 체크해줌
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);

		static float CurrGravity = 5.f;

		//추락일때 애니메이션 실행
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


	//기본 상태는 idle. 여기서 idle로 바꿔도 밑에서 상태를 바꾸면 그 상태가 된다.
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
		m_debug = L"휴식";
		break;
	case CPlayer::WALK:
		m_debug = L"걷기";
		break;
	case CPlayer::ATTACK:
		m_debug = L"공격";
		break;
	case CPlayer::HIT:
		m_debug = L"피격";
		break;
	case CPlayer::JUMP:
		m_debug = L"점프";
		break;
	case CPlayer::FALL:
		m_debug = L"추락";
		break;
	case CPlayer::DEAD:
		m_debug = L"죽음";
		break;
	case CPlayer::END:
		m_debug = L"끝";
		break;
	default:
		break;
	}
	TCHAR		szBuff[32] = L"상태 : ";
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


	//콤보 판단
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



	//이펙트 생성
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
		//공격 방향
		int iDir = m_eFront == FRONT::RIGHT ? 1 : -1;

		if (m_bCombo)
			weapon = CAbstractFactory<CWeapon>::Create(m_tInfo.fX + (m_tInfo.iCX >> 1) * iDir, m_tInfo.fY, L"att_n2_eff", frame, 192, 96);
		else
			weapon = CAbstractFactory<CWeapon>::Create(m_tInfo.fX + (m_tInfo.iCX >> 1) * iDir, m_tInfo.fY, L"att_n1_eff", frame, 192, 96);
	}

	if (weapon)
	{
		//2초동안만 지속
		dynamic_cast<CWeapon*>(weapon)->Set_Duration(0.1f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, weapon);
	}




}



void CPlayer::Key_Check()
{

	//왼쪽버튼 눌렀을때
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		//추락할때는 움직이는 모션 못함
		if (m_eCurState != STATE::FALL)
			m_eCurState = STATE::WALK;
		//왼쪽보기
		m_eFront = FRONT::LEFT;
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		m_tInfo.fX -= m_fSpeed;

	if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT))
		m_eCurState = STATE::IDLE;


	//오른쪽버튼 눌렀을때
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		if (m_eCurState != STATE::FALL)
			m_eCurState = STATE::WALK;
		//오른쪽보기
		m_eFront = FRONT::RIGHT;
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		m_tInfo.fX += m_fSpeed;

	if (CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT))
		m_eCurState = STATE::IDLE;

	//점프
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		if (m_eCurState != STATE::FALL)
			m_bJump = true;
		//점프 시작
		m_eJumpState = JUMP_STATE::STARTING;
		m_eCurState = STATE::JUMP;
	}

	//공격키 누르면 공격
	if (CKeyMgr::Get_Instance()->Key_Down('X'))
	{
		Attack();
	}

}

void CPlayer::Scene_Change()
{
	//상태가 바뀔때 한번만 실행
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
			//위 공격 아래 공격 구분
			if (m_eAttDir == ATT_DIR::UP)
			{
				memcpy(m_pFrameKey, L"hero_att_down_to_top", DIR_LEN);
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iFrameScene = m_eFront;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.dwFrameSpeed = 40;
				m_tFrame.bLoop = false;

				//초기화
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

				//초기화
				m_eAttDir = ATT_DIR::ATT_END;
			}
			//콤보냐
			else if (m_bCombo)
			{
				//콤보공격 모션
				memcpy(m_pFrameKey, L"hero_att_normal2", DIR_LEN);
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iFrameScene = m_eFront;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.dwFrameSpeed = 40;
				m_tFrame.bLoop = false;

				//2격 했으면 리셋.
				m_ComboTimer = GetTickCount();
			}
			else
			{
				//보통 공격모션
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

		//상태가 바뀌었을때 한번만 설정해줘야하니. 
		m_ePrvState = m_eCurState;
	}


	//점프에 따른 점프상태 변경
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

		//스페이스 누르고있으면 더 높이 점프
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
			m_curJumpVelo.fY -= 3.7f;		


		//중력적용
		m_curJumpVelo += m_Gravity;

		// * m_fDeltaTime을 해주면 1초에 m_curJumpVelo만큼 이동한다.
		m_tInfo.fX += m_curJumpVelo.fX * m_fDeltaTime;
		m_tInfo.fY += m_curJumpVelo.fY * m_fDeltaTime;

		//점프상태이고, 바닥과 충돌이면 점프해제
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);

		
		if (collision == CTileMgr::BOTTOM)
		{
			//점프 초기화 
			m_curJumpVelo = m_JumpVelo;
			m_eJumpState = JUMP_STATE::LANDING;
			//점프 초기화
			m_bJump = false;
		}
		else if (m_curJumpVelo.fY > 0)
		{
			//추락중
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


