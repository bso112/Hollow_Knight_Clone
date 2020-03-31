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
#include "MyImage.h"
#include "SoundMgr.h"

CPlayer::CPlayer()
	: m_JumpVelo(0.f, -70), m_bCombo(false), m_ComboTimer(MAXDWORD), m_ePrvFront(FRONT::FRONT_END),
	m_dwForceTimer(0), m_fForceTime(0.f)
{

}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{

	m_eJumpState = JUMP_STATE::STARTING;
	m_prvPos = Vector2(m_tInfo.fX, m_tInfo.fY);
	m_velocity = Vector2(0, 0);
	m_eFront = FRONT::RIGHT;

	m_fAttCoolDown = 0.5f;
	m_ComboWait = 0.8f;


	m_fHitTime = 1.f;
	//무적시간. HitTime 포함이라서 사실상 1초
	m_fInvincibleTime = 2.f;

	m_tStat.m_fMaxHp = 100;
	m_tStat.m_fHp = m_tStat.m_fMaxHp;

	m_tInfo.fX = 7260.f;
	m_tInfo.fY = 1793.f;
	m_tInfo.iCX = 56;
	m_tInfo.iCY = 97;

	m_tImgInfo.fX = 400.f;
	m_tImgInfo.fY = 1000.f;
	m_tImgInfo.iCX = 192;
	m_tImgInfo.iCY = 96;

	m_fAngle = 0.f;

	m_fSpeed = 7.f;

	m_curJumpVelo = m_JumpVelo;
	m_Gravity = Vector2(0, GRAVITY);

	m_eCurState = STATE::IDLE;
	m_ePrvState = STATE::END;

	m_eTag = OBJTAG::PLAYER;


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_idle.bmp", L"hero_idle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_move.bmp", L"hero_move");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/motion/hero_jump_start.bmp", L"hero_jump_start");
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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/effect/att_td_eff.bmp", L"att_td_eff");


	//피격이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hero/effect/hero_hit_eff.bmp", L"hero_hit_eff");






	memcpy(m_pFrameKey, L"hero_idle", DIR_LEN);

}

int CPlayer::Update()
{
#pragma region 피격효과

	if (m_dwHitTimer + m_fHitTime * 1000 > GetTickCount())
	{
		m_eCurState = STATE::HIT;
		Scene_Change();
		return OBJ_FREEZE;
	}

#pragma endregion

#pragma region 무적시간 세기

	if (m_bInvincible)
	{
		//m_fInvincibleTime만큼 지나면 무적을 false로 바꿈
		if (timerForInvincible + m_fInvincibleTime * 1000 < GetTickCount())
		{
			m_bInvincible = false;
		}
	}
#pragma endregion


#pragma region 중력적용, 델타타임
	//델타타임 구하기
	m_fDeltaTime = CMyTime::Get_Instance()->Get_DeltaTime();
	//프레임사이의 간격이 너무 크면 안됨.
	if (m_fDeltaTime > 0.15f)
		m_fDeltaTime = 0.15f;

	if (!m_bJump)
	{
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);
		m_eCollision = collision;
		//부딪힌게 아무것도 없으면 추락
		if (collision == CTileMgr::END)
		{
			m_eCurState = STATE::FALL;
		}
		else
			//기본상태
			m_eCurState = STATE::IDLE;

		m_tInfo.fY += m_Gravity.fY;
	}
#pragma endregion



#pragma region 넉백


	if (m_dwForceTimer + m_fForceTime * 1000 > GetTickCount())
	{
		m_tInfo.fX += m_velocity.fX * m_fDeltaTime;
		m_tInfo.fY += m_velocity.fY * m_fDeltaTime;
	}
	else
	{
		m_fForceTime = 0.f;
		m_velocity.fX = 0;
		m_velocity.fY = 0;
	}
#pragma endregion




	Move_Frame();
	Key_Check();
	OffSet();
	Jumping();

	Scene_Change();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{

}

void CPlayer::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	GdiTransparentBlt(_DC, (int)m_tImgRect.left + iScrollX, (int)m_tImgRect.top + iScrollY
		, m_tImgInfo.iCX, m_tImgInfo.iCY, hMemDC, m_tImgInfo.iCX * m_tFrame.iFrameScene, m_tImgInfo.iCY *m_tFrame.iFrameStart, m_tImgInfo.iCX, m_tImgInfo.iCY
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
	TextOut(_DC, 500, 200, szBuff, lstrlen(szBuff));

	TCHAR		szBuff2[32] = L"좌표 : ";
	swprintf_s(szBuff, L"X: %d  Y: %d", (int)m_tInfo.fX, (int)m_tInfo.fY);
	TextOut(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, szBuff, lstrlen(szBuff));


#pragma endregion
#pragma region UI


#pragma endregion



}

void CPlayer::Release()
{
}



void CPlayer::Take_Damage(float _fDamage)
{
	if (m_bInvincible)
		return;

	m_bInvincible = true;
	//무적타이머 ON
	timerForInvincible = GetTickCount();

	m_tStat.m_fHp -= _fDamage;
	if (m_tStat.m_fHp < 0)
	{
		m_bDead = true;
		m_tStat.m_fHp = 0;
	}

	Hit();


}

void CPlayer::Add_Force(Vector2 _vDir, float _fForce, float _fTime)
{
	//히트타임 끝나고 밀려남
	m_dwForceTimer = m_dwHitTimer + m_fHitTime * 1000;
	m_velocity = _vDir * _fForce;
	m_fForceTime = _fTime;

}

void CPlayer::OnCollisionEnter(CObj* _pOther, float _fX, float _fY)
{

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
	CSoundMgr::Get_Instance()->PlaySound(L"Att_One.wav", CSoundMgr::CHANNELID::PLAYER_ATT);

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

	INFO info;
	INFO imgInfo;

	float margin = 20.f;

	//이펙트 생성
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_eAttDir = ATT_DIR::UP;
		info.fX = m_tInfo.fX;
		info.fY = m_tInfo.fY - (m_tInfo.iCY >> 1);
		info.iCX = 150;
		info.iCY = 130;
		imgInfo.iCX = 192;
		imgInfo.iCY = 192;
		weapon = CAbstractFactory<CWeapon>::Create(info, imgInfo, L"att_dt_eff", frame);

	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_eAttDir = ATT_DIR::DOWN;
		info.fX = m_tInfo.fX;
		info.fY = m_tInfo.fY + (m_tInfo.iCY >> 1);
		info.iCX = 150;
		info.iCY = 130;
		imgInfo.iCX = 192;
		imgInfo.iCY = 192;
		weapon = CAbstractFactory<CWeapon>::Create(info, imgInfo, L"att_td_eff", frame);
	}
	else
	{
		//공격 방향
		int iDir = m_eFront == FRONT::RIGHT ? 1 : -1;

		info.fX = m_tInfo.fX + ((m_tInfo.iCX >> 1) + margin) * iDir;
		info.fY = m_tInfo.fY;
		info.iCX = 150;
		info.iCY = 90;
		imgInfo.iCX = 192;
		imgInfo.iCY = 96;

		if (m_bCombo)
		{
			weapon = CAbstractFactory<CWeapon>::Create(info, imgInfo, L"att_n2_eff", frame);
		}
		else
		{
			weapon = CAbstractFactory<CWeapon>::Create(info, imgInfo, L"att_n1_eff", frame);
		}
	}

	if (weapon)
	{
		//0.2초동안만 지속
		dynamic_cast<CWeapon*>(weapon)->Set_Duration(0.2f);
		dynamic_cast<CWeapon*>(weapon)->Set_Owner(CWeapon::OWNER::PLAYER);
		CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, weapon);
	}




}

void CPlayer::Hit()
{

	CSoundMgr::Get_Instance()->PlaySound(L"Hero_damage.wav", CSoundMgr::CHANNELID::PLAYER);
	m_eCurState = STATE::HIT;
	m_dwHitTimer = GetTickCount();
	FRAME frame;
	frame.iFrameStart = 0;
	frame.iFrameEnd = 1;
	frame.iFrameScene = 0;
	frame.dwFrameTime = GetTickCount();
	frame.dwFrameSpeed = 200;
	frame.bLoop = false;
	CObj* pEffect = CAbstractFactory<CMyImage>::Create(m_tInfo.fX, m_tInfo.fY, L"hero_hit_eff", frame, 800, 300);
	dynamic_cast<CMyImage*>(pEffect)->Set_Duration(1.f);
	CImageMgr::Get_Instance()->Add_Image(pEffect);
}



void CPlayer::Key_Check()
{

	//어떤 키를 누르건 화살표를 같이 누르면 무조건 이동, 방향변경
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		if (m_eCollision == CTileMgr::BOTTOM)
			CSoundMgr::Get_Instance()->PlaySound(L"Hero_walk_footsteps_stone.wav", CSoundMgr::CHANNELID::PLAYER_FOOTSTEP);
		//왼쪽보기
		m_eFront = FRONT::LEFT;
		m_tInfo.fX -= m_fSpeed;
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		if (m_eCollision == CTileMgr::BOTTOM)
			CSoundMgr::Get_Instance()->PlaySound(L"Hero_walk_footsteps_stone.wav", CSoundMgr::CHANNELID::PLAYER_FOOTSTEP);
		//오른쪽보기
		m_eFront = FRONT::RIGHT;
		m_tInfo.fX += m_fSpeed;
	}
	else
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::PLAYER_FOOTSTEP);


	//공격 애니메이션을 위한 트리거
	static bool bAttack = false;
	//공격 애니메이션을 재생에 쓸 타이머
	static DWORD attAnimTimer = 0;
	//공격이 예약되었는가 (쿨타임 때문에 조작감 이상해서 예약기능 넣음)
	static bool reserved = false;


	//공격키 누르면 무조건 공격
	if (CKeyMgr::Get_Instance()->Key_Pressing('X') || reserved)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('X'))
			reserved = true;

		static DWORD attTimer = GetTickCount();
		if (attTimer + m_fAttCoolDown * 1000 < GetTickCount())
		{
			Attack();
			attTimer = GetTickCount();

			//1초동안 어택애니메이션 재생
			bAttack = true;
			attAnimTimer = GetTickCount();
			reserved = false;
		}

	}
	//공격중에는 점프 못함
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		if (m_eCurState != STATE::FALL)
			m_bJump = true;
	}


	//애니메이션 상태변화. 밑에 있을수록 우선순위가 낮다.

	//어택애니메이션 재생
	if (bAttack)
	{
		if (attAnimTimer + 500 > GetTickCount())
			m_eCurState = STATE::ATTACK;
		else
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_ATT);
			bAttack = false;
		}
	}
	//공격중에는 점프 못함. 점프중일때 공격가능
	else if (m_bJump)
	{
		//점프 시작
		m_eJumpState = JUMP_STATE::STARTING;
		m_eCurState = STATE::JUMP;

	}
	//공격도 아니고 점프도 아닐때
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		//추락할때는 움직이는 모션 못함
		if (m_eCurState != STATE::FALL)
			m_eCurState = STATE::WALK;


	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		if (m_eCurState != STATE::FALL)
			m_eCurState = STATE::WALK;

	}






}

void CPlayer::Scene_Change()
{

	//상태가 바뀔때, 방향이 바뀔때 한번만 실행
	if (m_eCurState != m_ePrvState || (m_ePrvFront != m_eFront))
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
				m_tFrame.dwFrameSpeed = 100;
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
				m_tFrame.dwFrameSpeed = 100;
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
				m_tFrame.dwFrameSpeed = 100;
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
				m_tFrame.dwFrameSpeed = 100;
				m_tFrame.bLoop = false;

			}
			break;
		}
		case CPlayer::HIT:
		{
			memcpy(m_pFrameKey, L"hero_hit", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = false;
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
			m_tFrame.bLoop = true;
			break;
		}
		case CPlayer::DEAD:
		{
			memcpy(m_pFrameKey, L"hero_dead", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 150;
			m_tFrame.bLoop = false;
			break;
		}
		case CPlayer::END:
			break;
		default:
			break;
		}

		//상태가 바뀌었을때 한번만 설정해줘야하니. 
		m_ePrvState = m_eCurState;
		m_ePrvFront = m_eFront;
	}


	//점프에 따른 점프상태 변경
	if (m_eCurState == STATE::JUMP && ((m_ePrvJumpState != m_eJumpState) || (m_ePrvFront != m_eFront)))
	{
		switch (m_eJumpState)
		{
		case CPlayer::STARTING:
		{
			memcpy(m_pFrameKey, L"hero_jump_start", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 70;
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
			m_tFrame.dwFrameSpeed = 70;
			m_tFrame.bLoop = true;
			break;
		}
		case CPlayer::LANDING:
		{
			memcpy(m_pFrameKey, L"hero_jump_landing", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = false;
			break;
		}
		case CPlayer::JUMP_END:
			break;
		default:
			break;
		}

		m_ePrvJumpState = m_eJumpState;
		m_ePrvFront = m_eFront;
	}
}

void CPlayer::Jumping()
{

	if (m_bJump)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::PLAYER_FOOTSTEP);

		//스페이스 누르고있으면 더 높이 점프
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
			m_curJumpVelo.fY -= 3.7f;

		//중력적용
		m_curJumpVelo += m_Gravity;

		// * m_fDeltaTime을 해주면 1초에 m_curJumpVelo만큼 이동한다.
		m_tInfo.fX += m_curJumpVelo.fX * 0.15f;
		m_tInfo.fY += m_curJumpVelo.fY * 0.15f;

		//점프상태이고, 바닥과 충돌이면 점프해제
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);
		m_eCollision = collision;

		if (collision == CTileMgr::BOTTOM)
		{
			//점프 초기화 
			m_curJumpVelo = m_JumpVelo;
			m_eJumpState = JUMP_STATE::LANDING;
			m_bJump = false;

		}
		else if (m_curJumpVelo.fY > -15)
		{
			//추락중
			m_eJumpState = JUMP_STATE::FALLING;
		}

	}


}

void CPlayer::OffSet()
{
	int iOffSetX = WINCX >> 1;
	int iOffSetY = 450;
	float iScrollX = CScrollMgr::Get_Instance()->Get_Scroll_X();
	float iScrollY = CScrollMgr::Get_Instance()->Get_Scroll_Y();

	if (iOffSetX < (m_tInfo.fX + iScrollX))
		CScrollMgr::Get_Instance()->Set_Scroll_X(iOffSetX - (m_tInfo.fX + iScrollX));
	if (iOffSetX > (m_tInfo.fX + iScrollX))
		CScrollMgr::Get_Instance()->Set_Scroll_X(iOffSetX - (m_tInfo.fX + iScrollX));

	if (iOffSetY < (m_tInfo.fY + iScrollY))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(iOffSetY - (m_tInfo.fY + iScrollY));
	if (iOffSetY > (m_tInfo.fY + iScrollY))
		CScrollMgr::Get_Instance()->Set_Scroll_Y(iOffSetY - (m_tInfo.fY + iScrollY));
}


