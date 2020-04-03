#include "stdafx.h"
#include "FalseKnight.h"
#include "MyTime.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "Weapon.h"
#include "ObjMgr.h"

CFalseKnight::CFalseKnight()
	:m_bWasGroggy(false), m_bHit(false)
{
}


CFalseKnight::~CFalseKnight()
{
}

void CFalseKnight::Initialize()
{


	memcpy(m_pFrameKey, L"knight_idle", DIR_LEN);
	m_tInfo.iCX = 256;
	m_tInfo.iCY = 189;
	m_tImgInfo.iCX = 750;
	m_tImgInfo.iCY = 450;



	m_eFront = FRONT::LEFT;
	m_eCurState = STATE::IDLE;
	m_ePrvState = STATE::END;

	m_tStat = STAT(600);



	m_fSpeed = 2.f;

	m_fDeadWait = 10.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/att_normal.bmp", L"knight_att_normal");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/att_swing_around.bmp", L"knight_att_swing_around");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/Ball.bmp", L"knight_Ball");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/idle.bmp", L"knight_idle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/down.bmp", L"knight_down");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/groggy.bmp", L"knight_groggy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/groggy_hit.bmp", L"knight_groggy_hit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/groggy_stand.bmp", L"knight_groggy_stand");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/die.bmp", L"knight_die");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/jump.bmp", L"knight_jump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/att_normal_left.bmp", L"knight_att_normal_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/att_swing_around_left.bmp", L"knight_att_swing_around_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/idle_left.bmp", L"knight_idle_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/down_left.bmp", L"knight_down_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/groggy_left.bmp", L"knight_groggy_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/groggy_hit_left.bmp", L"knight_groggy_hit_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/groggy_stand_left.bmp", L"knight_groggy_stand_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/die_left.bmp", L"knight_die_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/jump_left.bmp", L"knight_jump_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/FalseKnight/effect/crush.bmp", L"knight_effect_crush");

}

void CFalseKnight::Scene_Change()
{

	//거리가 너무 가까워지면 정신없이 바뀜
	if (m_fDistToTarget > 1.f)
	{
		//이동뱡항 정해주기
		m_vToTarget.fX > 0 ? m_eFront = FRONT::RIGHT : m_eFront = FRONT::LEFT;
	}


	if (m_eCurState != m_ePrvState || m_eFront != m_ePrvFront)
	{

		switch (m_eCurState)
		{
		case CFalseKnight::IDLE:
		{
			TCHAR* frameKey = L"knight_idle";
			if (m_eFront == FRONT::LEFT)
				frameKey = L"knight_idle_left";
			memcpy(m_pFrameKey, frameKey, DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			m_tFrame.bEnd = false;
			m_tFrame.iLoopCnt = 0;
			break;
		}
		case CFalseKnight::ATTACK:
		{
			TCHAR* frameKey = L"knight_att_normal";
			if (m_eFront == FRONT::LEFT)
				frameKey = L"knight_att_normal_left";
			memcpy(m_pFrameKey, frameKey, DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = false;
			m_tFrame.bEnd = false;
			m_tFrame.iLoopCnt = 0;

			break;

		}
		case CFalseKnight::JUMP:
		{
			TCHAR* frameKey = L"knight_jump";
			if (m_eFront == FRONT::LEFT)
				frameKey = L"knight_jump_left";
			memcpy(m_pFrameKey, frameKey, DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 500;
			m_tFrame.bLoop = false;
			m_tFrame.bEnd = false;
			m_tFrame.iLoopCnt = 0;

			break;
		}
		case CFalseKnight::REMOTE_ATTACK:
		{
			TCHAR* frameKey = L"knight_att_normal";
			if (m_eFront == FRONT::LEFT)
				frameKey = L"knight_att_normal_left";
			memcpy(m_pFrameKey, frameKey, DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = false;
			m_tFrame.bEnd = false;
			m_tFrame.iLoopCnt = 0;

			break;
		}
		case CFalseKnight::SWING_AROUND:
		{

			TCHAR* frameKey = L"knight_att_swing_around";
			memcpy(m_pFrameKey, frameKey, DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = true;
			m_tFrame.iLoopCnt = 3;
			m_tFrame.bEnd = false;

			break;
		}
		case CFalseKnight::DOWN:
		{
			TCHAR* frameKey = L"knight_down";
			if (m_eFront == FRONT::LEFT)
				frameKey = L"knight_down_left";
			memcpy(m_pFrameKey, frameKey, DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 150;
			m_tFrame.bLoop = false;
			m_tFrame.bEnd = false;
			m_tFrame.iLoopCnt = 0;

			break;
		}
		case CFalseKnight::GROGGY:
		{
			TCHAR* frameKey = L"knight_groggy";
			if (m_eFront == FRONT::LEFT)
				frameKey = L"knight_groggy_left";
			memcpy(m_pFrameKey, frameKey, DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = false;
			m_tFrame.bEnd = false;
			m_tFrame.iLoopCnt = 0;

			break;
		}
		case CFalseKnight::GROGGY_STAND:
		{
			TCHAR* frameKey = L"knight_groggy_stand";
			if (m_eFront == FRONT::LEFT)
				frameKey = L"knight_groggy_stand_left";
			memcpy(m_pFrameKey, frameKey, DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = false;
			m_tFrame.bEnd = false;
			m_tFrame.iLoopCnt = 0;

			break;
		}
		case CFalseKnight::GROGGY_HIT:
		{
			TCHAR* frameKey = L"knight_groggy_hit";
			if (m_eFront == FRONT::LEFT)
				frameKey = L"knight_groggy_hit_left";
			memcpy(m_pFrameKey, frameKey, DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.bLoop = false;
			m_tFrame.bEnd = false;
			m_tFrame.iLoopCnt = 0;

			break;
		}
		case CFalseKnight::DEAD:
		{
			TCHAR* frameKey = L"knight_die";
			if (m_eFront == FRONT::LEFT)
				frameKey = L"knight_die_left";
			memcpy(m_pFrameKey, frameKey, DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			m_tFrame.bEnd = false;
			m_tFrame.iLoopCnt = 0;
			break;
		}
		case CFalseKnight::END:
			break;
		default:
			break;
		}

		m_ePrvState = m_eCurState;
		m_ePrvFront = m_eFront;
	}
}

int CFalseKnight::Update()
{

	if (m_bDead)
		return OBJ_DEAD;

	if (m_eCurState != STATE::DEAD)
	{
		//타깃으로 가는 방향벡터
		m_vToTarget = Vector2(m_pTarget->Get_INFO().fX, m_pTarget->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY);
		m_fDistToTarget = m_vToTarget.magnitude();
		Update_State();
		Process();

	}
	else //죽었으면
	{
		//일정시간후 파괴
		if (m_dwDeadTimer + m_fDeadWait * 1000 < GetTickCount())
			m_bDead = true;
	}

	Move_Frame();
	Scene_Change();

	return OBJ_NOEVENT;
}


void CFalseKnight::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	//그림의 중심선이 안맞아서 그릴때 보정해준다.
	int offSet = 37;

	if (m_eFront == FRONT::LEFT)
	{
		GdiTransparentBlt(_DC, (int)m_tImgRect.left + iScrollX, (int)m_tImgRect.top + iScrollY - 100
			, m_tImgInfo.iCX, m_tImgInfo.iCY, memDC, m_tImgInfo.iCX * (m_tFrame.iFrameEnd - m_tFrame.iFrameStart), m_tImgInfo.iCY *m_tFrame.iFrameScene, m_tImgInfo.iCX, m_tImgInfo.iCY
			, RGB(30, 30, 30));
	}
	else
	{
		GdiTransparentBlt(_DC, (int)m_tImgRect.left + iScrollX, (int)m_tImgRect.top + iScrollY - 100
			, m_tImgInfo.iCX, m_tImgInfo.iCY, memDC, m_tImgInfo.iCX * m_tFrame.iFrameStart, m_tImgInfo.iCY *m_tFrame.iFrameScene, m_tImgInfo.iCX, m_tImgInfo.iCY
			, RGB(30, 30, 30));

	}
#pragma region 디버그

	TCHAR		szBuff[32] = L"상태: ";
	TCHAR*		state = nullptr;
	switch (m_eCurState)
	{
	case CFalseKnight::IDLE:
		state = L"휴식";
		break;
	case CFalseKnight::ATTACK:
		state = L"공격";
		break;
	case CFalseKnight::REMOTE_ATTACK:
		state = L"원거리공격";
		break;
	case CFalseKnight::JUMP:
		state = L"점프";
		break;
	case CFalseKnight::SWING_AROUND:
		state = L"스윙어라운드";
		break;
	case CFalseKnight::DOWN:
		state = L"다운";
		break;
	case CFalseKnight::GROGGY:
		state = L"그로기";
		break;
	case CFalseKnight::GROGGY_STAND:
		state = L"서기";
		break;
	case CFalseKnight::GROGGY_HIT:
		state = L"맞기";
		break;
	case CFalseKnight::DEAD:
		state = L"죽음";
		break;
	case CFalseKnight::END:
		state = L"엔드";
		break;
	default:
		break;
	}
	lstrcat(szBuff, state);
	TextOut(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, szBuff, lstrlen(szBuff));


	TCHAR		szBuff2[32] = L"";
	swprintf_s(szBuff2, L"체력: %d", (int)m_tStat.m_fHp);
	TextOut(_DC, m_tRect.right + iScrollX, m_tRect.top + iScrollY, szBuff2, lstrlen(szBuff2));






#pragma endregion

}



void CFalseKnight::Late_Update()
{
	//상시중력
	if (!m_bJump)
	{
		m_tInfo.fY += m_Gravity.fY;
		//점프일때는 따로 콜리전 체크해줌
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);

	}

}


void CFalseKnight::Release()
{
}



void CFalseKnight::Process()
{
	switch (m_eCurState)
	{
	case CFalseKnight::IDLE:
		break;
	case CFalseKnight::JUMP:
	{
		if (m_tFrame.iFrameStart == 0)
			CSoundMgr::Get_Instance()->PlaySoundW(L"Fknight_jump.wav", CSoundMgr::CHANNELID::MONSTER_EFFECT);

		m_bJump = true;
	}
	case CFalseKnight::ATTACK:
	{
		Attack();
		break;
	}
	case CFalseKnight::REMOTE_ATTACK:
	{
		RemoteAttack();
		break;
	}
	case CFalseKnight::SWING_AROUND:
	{
		Swing_Around();
		break;
	}
	case CFalseKnight::DOWN:
	{
		//다운되는 소리 재생
		CSoundMgr::Get_Instance()->PlaySoundW(L"boss_stun.wav", CSoundMgr::CHANNELID::MONSTER_EFFECT);
		break;
	}
	case CFalseKnight::GROGGY:
		break;
	case CFalseKnight::GROGGY_STAND:
		break;
	case CFalseKnight::GROGGY_HIT:
	{
		TCHAR* sounds[5] = { L"Fknight_hit_01.wav", L"Fknight_hit_02.wav", L"Fknight_hit_03.wav", L"Fknight_hit_04.wav", L"Fknight_hit_05.wav" };
		int ranNum = rand() % 5;
		CSoundMgr::Get_Instance()->PlaySoundW(sounds[ranNum], CSoundMgr::CHANNELID::MONSTER);
		if (m_tFrame.bEnd)
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER);
		break;
	}
	case CFalseKnight::DEAD:
		break;
	case CFalseKnight::END:
		break;
	default:
		break;
	}

	Jumping();

}

void CFalseKnight::Update_State()
{
	//	enum STATE { IDLE, ATTACK, JUMP, SWING_AROUND, DOWN, GROGGY, GROGGY_STAND, GROGGY_HIT, DEAD, END };


	float fill = m_tStat.m_fHp / m_tStat.m_fMaxHp;
	static int tmp = 0;
	if ((int(fill * 10) == 8 || int(fill * 10) == 6 || int(fill * 10) == 3)
		&& tmp != int(fill * 10))
	{
		tmp = int(fill * 10);
		m_eCurState = STATE::DOWN;
	}


	//다운애니메이션이 끝나면
	if (m_ePrvState == STATE::DOWN && m_tFrame.bEnd)
	{
		m_eCurState = STATE::GROGGY;
		//hp가 10보다 클때만 다운에서 회복 가능.
		if (m_tStat.m_fHp > 70.f)
		{
			//타이머 가동
			m_dwDownTimer = GetTickCount();
			m_bWasGroggy = true;
		}
	}



	//그로기 상태이고 맞았으면,
	if (m_ePrvState == STATE::GROGGY && m_bHit)
	{
		m_eCurState = STATE::GROGGY_HIT;
		m_bHit = false;

		//만약 그로기에서 맞았고, 체력이 0이하가 되면
		if (m_tStat.m_fHp <= 0)
			m_eCurState = STATE::DEAD;
	}

	//맞는 애니메이션이 끝났으면
	if (m_ePrvState == STATE::GROGGY_HIT && m_tFrame.bEnd)
	{
		m_eCurState = STATE::GROGGY;
	}

	//그로기 상태이며 4초가 지났으면
	if (m_ePrvState == STATE::GROGGY && m_dwDownTimer + 4000 < GetTickCount())
	{
		m_eCurState = STATE::GROGGY_STAND;
	}


	//스탠드 애니메이션이 끝나면
	if (m_ePrvState == STATE::GROGGY_STAND && m_tFrame.bEnd)
		m_eCurState = STATE::JUMP;


	//점프 애니메이션이 끝났고 전에 그로기 상태였으면
	if (m_ePrvState == STATE::JUMP &&  m_tFrame.bEnd && m_bWasGroggy)
	{

		m_eCurState = STATE::SWING_AROUND;
		m_bWasGroggy = false;

	}


	//확률에 따른 상태변화. 아이들 애니메이션이 끝났을때 확률체크
	if (m_ePrvState == STATE::IDLE && m_tFrame.bEnd)
	{
		int ranNum = rand() % 100;
		if (ranNum < 30)
			m_eCurState = STATE::JUMP;
		else if (ranNum < 60)
			m_eCurState = STATE::ATTACK;
		else
			m_eCurState = STATE::REMOTE_ATTACK;
	}


	//만약 애니메이션이 모두 끝나면
	if (((m_ePrvState == STATE::JUMP && m_eCurState != STATE::SWING_AROUND && !m_bJump) || m_ePrvState == STATE::ATTACK || m_ePrvState == STATE::REMOTE_ATTACK || m_ePrvState == STATE::SWING_AROUND)
		&& m_tFrame.bEnd)
		m_eCurState = STATE::IDLE;


}

void CFalseKnight::Jumping()
{
	if (m_bJump)
	{

		//중력적용
		m_curJumpVelo += m_Gravity;
		Vector2 vDir = m_vToTarget.Nomalize();
		int fDir = vDir.fX < 0 ? -1 : 1;


		m_fDeltaTime = CMyTime::Get_Instance()->Get_DeltaTime();
		if (m_fDeltaTime > 0.15f)
			m_fDeltaTime = 0.15f;


		//가속도 제한
		float drag = m_curJumpVelo.fY * 0.15f;
		if (drag > 15)
			drag = 15.f;


		// * m_fDeltaTime을 해주면 1초에 m_curJumpVelo만큼 이동한다.

		m_tInfo.fX += m_curJumpVelo.fX * fDir * m_fDeltaTime;

		m_tInfo.fY += m_curJumpVelo.fY * m_fDeltaTime;

		//점프상태이고, 바닥과 충돌이면 점프해제
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);



		if (collision == CTileMgr::BOTTOM)
		{

			CSoundMgr::Get_Instance()->PlaySoundW(L"Fknight_landing.wav", CSoundMgr::CHANNELID::MONSTER_EFFECT);

			//점프 초기화 
			m_curJumpVelo = JUMP_VELO;
			m_bJump = false;
			++m_iAttCnt;


		}

	}

}

void CFalseKnight::Attack()

{
	//어택모션 중간쯤오면 충돌박스 생성
	if (m_eCurState == STATE::ATTACK && m_tFrame.iFrameStart == m_tFrame.iFrameEnd - 2)
	{
		//카메라 흔들기
		//CScrollMgr::Get_Instance()->Shake_Camera(2.f, 0.5f);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Fknight_groundatt.wav", CSoundMgr::EFFECT);

		CObj* weapon = nullptr;
		int dir = 0;
		m_vToTarget.fX > 0 ? dir = 1 : dir = -1;


		//충돌박스 생성
		int iCX = 200;
		int iCY = 200;
		float fX = (dir == 1) ? (float)m_tRect.right + (iCX >> 1) : (float)m_tRect.left - (iCX >> 1);
		float fY = (float)m_tRect.bottom - (iCY >> 1);
		weapon = CAbstractFactory<CWeapon>::Create(fX, fY);

		if (weapon)
		{
			weapon->Set_Size(iCX, iCY);
			dynamic_cast<CWeapon*>(weapon)->Set_Duration(0.0001f);
			dynamic_cast<CWeapon*>(weapon)->Set_Owner(CWeapon::OWNER::MONSTER);
			CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, weapon);

		}

	}
}

void CFalseKnight::RemoteAttack()
{
	//어택모션 중간쯤오면 충돌박스 생성
	if (m_eCurState == STATE::REMOTE_ATTACK && m_tFrame.iFrameStart == m_tFrame.iFrameEnd - 2)
	{
		//카메라 흔들기
		//CScrollMgr::Get_Instance()->Shake_Camera(2.f, 0.5f);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Fknight_groundatt.wav", CSoundMgr::EFFECT);

		CObj* weapon = nullptr;
		int dir = 0;
		m_vToTarget.fX > 0 ? dir = 1 : dir = -1;


		//충돌박스 생성
		int iCX = 200;
		int iCY = 200;
		float fX = (dir == 1) ? (float)m_tRect.right + (iCX >> 1) : (float)m_tRect.left - (iCX >> 1);
		float fY = (float)m_tRect.bottom - (iCY >> 1);
		weapon = CAbstractFactory<CWeapon>::Create(fX, fY);

		if (weapon)
		{
			weapon->Set_Size(iCX, iCY);
			dynamic_cast<CWeapon*>(weapon)->Set_Duration(0.0001f);
			dynamic_cast<CWeapon*>(weapon)->Set_Owner(CWeapon::OWNER::MONSTER);
			CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, weapon);

		}

#pragma region 이펙트생성

		CObj* effect = nullptr;
		FRAME frame;

		frame.iFrameStart = 0;
		frame.iFrameEnd = 5;
		frame.iFrameScene = m_eFront;
		frame.dwFrameTime = GetTickCount();
		frame.dwFrameSpeed = 400;
		frame.bLoop = false;


		int direction = 0;
		m_vToTarget.fX > 0 ? direction = 1 : direction = -1;

		int effectCnt = 6;
		int margin = 0.f;
		for (int i = 0; i < effectCnt; ++i)
		{
			INFO info;
			INFO imgInfo;


			info.iCX = 50;
			info.iCY = 100;
			info.fX = m_tInfo.fX + ((m_tInfo.iCX >> 1) + margin) * direction;
			info.fY = (float)m_tRect.bottom - (info.iCY >> 1);
			imgInfo.iCX = 128;
			imgInfo.iCY = 256;

			effect = CAbstractFactory<CWeapon>::Create(info, imgInfo, L"knight_effect_crush", frame);
			CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, effect);
			dynamic_cast<CWeapon*>(effect)->Set_Duration(2.5f);
			dynamic_cast<CWeapon*>(effect)->Add_Force(Vector2((float)direction, 0), 700, 6.f);
			dynamic_cast<CWeapon*>(effect)->Set_Owner(CWeapon::OWNER::MONSTER);

			margin += 30;
		}


#pragma endregion
	}
}

void CFalseKnight::Swing_Around()
{
	//어택모션 중간쯤오면 충돌박스 생성
	if (m_eCurState == STATE::ATTACK && (m_tFrame.iFrameStart == 3 || m_tFrame.iFrameStart == 7))
	{
		//카메라 흔들기
		//CScrollMgr::Get_Instance()->Shake_Camera(2.f, 0.5f);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Fknight_groundatt.wav", CSoundMgr::EFFECT);

		CObj* weapon = nullptr;
		int dir = 0;
		(m_tFrame.iFrameStart == 7) ? dir = 1 : dir = -1;


		//충돌박스 생성
		int iCX = 200;
		int iCY = 200;
		float fX = (dir == 1) ? (float)m_tRect.right + (iCX >> 1) : (float)m_tRect.left - (iCX >> 1);
		float fY = (float)m_tRect.bottom - (iCY >> 1);
		weapon = CAbstractFactory<CWeapon>::Create(fX, fY);

		if (weapon)
		{
			weapon->Set_Size(iCX, iCY);
			dynamic_cast<CWeapon*>(weapon)->Set_Duration(0.0001f);
			dynamic_cast<CWeapon*>(weapon)->Set_Owner(CWeapon::OWNER::MONSTER);
			CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, weapon);

		}

	}

#pragma region 공 떨어뜨리기


	FRAME frame;

	frame.iFrameStart = 0;
	frame.iFrameEnd = 2;
	frame.iFrameScene = 0;
	frame.dwFrameTime = GetTickCount();
	frame.dwFrameSpeed = 100;
	frame.bLoop = true;
	frame.iLoopCnt = 0;


	//0.3초 간격으로 생성
	static DWORD timer = GetTickCount();
	if (timer + 600 < GetTickCount())
	{

		int ranNum = rand() % 100;

		INFO info;
		INFO imgInfo;

		info.iCX = 73;
		info.iCY = 85;
		//화면의 left top 맞나?
		info.fX = -CScrollMgr::Get_Instance()->Get_Scroll_X() + ranNum * 10;
		info.fY = -CScrollMgr::Get_Instance()->Get_Scroll_Y();
		imgInfo.iCX = 103;
		imgInfo.iCY = 104;

		CObj* ball = CAbstractFactory<CWeapon>::Create(info, imgInfo, L"knight_Ball", frame);
		dynamic_cast<CWeapon*>(ball)->Add_Force(Vector2(0, 1), 100.f, 6.f);
		dynamic_cast<CWeapon*>(ball)->Set_Duration(6.f);
		dynamic_cast<CWeapon*>(ball)->Set_Horizontal();
		dynamic_cast<CWeapon*>(ball)->Set_Owner(CWeapon::OWNER::NONE);
		CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, ball);
		timer = GetTickCount();
	}







#pragma endregion

}

void CFalseKnight::Patrol()
{
}

void CFalseKnight::Chase_Target()
{
}


void CFalseKnight::OnDead()
{
	m_eCurState = STATE::DEAD;
	memcpy(m_pFrameKey, L"knight_die", DIR_LEN);
	m_dwDeadTimer = GetTickCount();
}

void CFalseKnight::OnTakeDamage()
{
	if (m_ePrvState == STATE::GROGGY)
	{
		m_bHit = true;
	}
	else
	{
		m_bHit = false;
	}
}
