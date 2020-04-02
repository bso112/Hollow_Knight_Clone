#include "stdafx.h"
#include "FalseKnight.h"
#include "MyTime.h"
#include "BmpMgr.h"
#include "TileMgr.h"

CFalseKnight::CFalseKnight()
{
}


CFalseKnight::~CFalseKnight()
{
}

void CFalseKnight::Initialize()
{

	memcpy(m_pFrameKey, L"knight_die", DIR_LEN);
	m_tInfo.iCX = 120;
	m_tInfo.iCY = 70;
	m_tImgInfo.iCX = 1000;
	m_tImgInfo.iCY = 600;

	m_eFront = FRONT::RIGHT;
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
}

void CFalseKnight::Scene_Change()
{
	if (m_eCurState != m_ePrvState || m_eFront != m_ePrvFront)
	{

		switch (m_eCurState)
		{
		case CFalseKnight::IDLE:
		{
			memcpy(m_pFrameKey, L"knight_die", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case CFalseKnight::ATTACK:
		{

			break;
		}
		case CFalseKnight::SWING_AROUND:
		{

			break;
		}
		case CFalseKnight::DOWN:
		{

			break;
		}
		case CFalseKnight::GROGGY:
		{

			break;
		}
		case CFalseKnight::GROGGY_STAND:
		{

			break;
		}
		case CFalseKnight::GROGGY_HIT:
		{

			break;
		}
		case CFalseKnight::DEAD:
		{

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
	case CFalseKnight::ATTACK:
		break;
	case CFalseKnight::SWING_AROUND:
		break;
	case CFalseKnight::DOWN:
		break;
	case CFalseKnight::GROGGY:
		break;
	case CFalseKnight::GROGGY_STAND:
		break;
	case CFalseKnight::GROGGY_HIT:
		break;
	case CFalseKnight::DEAD:
		break;
	case CFalseKnight::END:
		break;
	default:
		break;
	}

}

void CFalseKnight::Update_State()
{
	//	enum STATE { IDLE, ATTACK, JUMP, SWING_AROUND, DOWN, GROGGY, GROGGY_STAND, GROGGY_HIT, DEAD, END };
	


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
		m_tInfo.fX += m_curJumpVelo.fX * -fDir * m_fDeltaTime;
		m_tInfo.fY += m_curJumpVelo.fY * m_fDeltaTime;

		//점프상태이고, 바닥과 충돌이면 점프해제
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);



		if (collision == CTileMgr::BOTTOM)
		{
			//점프 초기화 
			m_curJumpVelo = JUMP_VELO;
			m_bJump = false;
			++m_iAttCnt;


		}

	}

}

void CFalseKnight::Attack()
{
	++m_iAttCnt;
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
}
