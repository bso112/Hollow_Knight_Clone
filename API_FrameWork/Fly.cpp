#include "stdafx.h"
#include "Fly.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "MyTime.h"
#include "TileMgr.h"
CFly::CFly()
{
}


CFly::~CFly()
{
}

void CFly::Initialize()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Fly/move.bmp", L"fly_move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Fly/dead.bmp", L"fly_dead");

	memcpy(m_pFrameKey, L"fly_move", sizeof(TCHAR) * DIR_LEN);

	m_tInfo.iCX = 45;
	m_tInfo.iCY = 67;
	m_tImgInfo.iCX = 192;
	m_tImgInfo.iCY = 192;

	m_tStat = STAT(50);

	//스폰된 장소가 정찰의 중심점
	m_PartolSpot.x = (LONG)m_tInfo.fX;
	m_PartolSpot.y = (LONG)m_tInfo.fY;

	//인식범위
	m_fRadius = 300;
	//정찰범위
	m_fPatrol = 100;
	//속도
	m_fSpeed = 1.f;
}

int CFly::Update()
{

	if (m_bDead)
		return OBJ_DEAD;
#pragma region 넉백

	//델타타임 구하기
	m_fDeltaTime = CMyTime::Get_Instance()->Get_DeltaTime();
	//프레임사이의 간격이 너무 크면 안됨.
	if (m_fDeltaTime > 0.15f)
		m_fDeltaTime = 0.15f;

	if (m_dwForceTimer + m_fForceTime * 1000> GetTickCount())
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


	CTileMgr::COLLISION collision = CTileMgr::END;
	CTileMgr::Get_Instance()->Collision_Ex(this, collision);

	if (m_eCurState != STATE::DEAD)
	{
		float target_fX = m_pTarget->Get_INFO().fX;

		//타깃과의 거리측정
		m_distToTarget = (Vector2(m_pTarget->Get_INFO().fX, m_pTarget->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY)).magnitude();

		//기본상태
		m_eCurState = STATE::IDLE;

		//타겟이 인식범위안에 들어오면
		if (m_tInfo.fX + m_fRadius >= target_fX
			&& m_tInfo.fX - m_fRadius <= target_fX)
		{
			//놀라고

			//추격상태가 된다.
			Chase_Target();
		}
		else
			//아니면 정찰
			Patrol();
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

void CFly::Late_Update()
{
}


void CFly::Release()
{
}

void CFly::Patrol()
{
	//순찰장소로 간다.

	Vector2 dirToPatrol = Vector2((float)m_PartolSpot.x, (float)m_PartolSpot.y) - Vector2(m_tInfo.fX, m_tInfo.fY);
	//순찰 장소와의 거리측정
	float distToPatrol = dirToPatrol.magnitude();

	dirToPatrol = dirToPatrol.Nomalize();

	//도착할때까지
	if (distToPatrol > 2)
	{
		//순찰장소로 간다.
		m_tInfo.fX += dirToPatrol.fX * m_fSpeed;
		m_tInfo.fY += dirToPatrol.fY * m_fSpeed;

		dirToPatrol.fX < 0 ? m_fDir = -abs(m_fDir) : m_fDir = abs(m_fDir);

	}


}

void CFly::Chase_Target()
{

	//플레이어 쫒기
	m_eCurState = STATE::RUN;


	float fX = m_pTarget->Get_INFO().fX - m_tInfo.fX;
	float fY = m_pTarget->Get_INFO().fY - m_tInfo.fY;
	float dia = sqrtf(fX * fX + fY * fY);

	//거리가 5보다 작으면
	if (dia < 5.f)
	{
		//멈춤
		return;
	}
	float theta = acosf(fX / dia);


	//타깃이 더 아래애 있으면 
	if (m_tInfo.fY < m_pTarget->Get_INFO().fY)
		theta = (2 * PI) - theta;

	float fDirX = cosf(theta);
	float fDirY = sinf(theta);

	float chasing_speed = m_fSpeed + 1;
	m_tInfo.fX += fDirX * chasing_speed;
	//윈도우 좌표계는 데카르트 좌표계의 반대니까.
	m_tInfo.fY -= fDirY * chasing_speed;

	//부호 뽑아내기
	m_fDir = fDirX / abs(fDirX);


}


void CFly::Scene_Change()
{

	//거리가 너무 가까워지면 정신없이 바뀜
	if (m_distToTarget > 1.f)
	{
		//방향에 따라 스프라이트 시트 바꾸기.
		if (m_fDir < 0)
			m_eFront = FRONT::LEFT;
		else
			m_eFront = FRONT::RIGHT;
	}


	if (m_eCurState != m_ePrvState)
	{
		switch (m_eCurState)
		{
		case RUN:
		{
			memcpy(m_pFrameKey, L"fly_move", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}

		case DEAD:
		{
			memcpy(m_pFrameKey, L"fly_dead", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}
		case END:
			break;
		default:
			break;
		}

		m_ePrvState = m_eCurState;
	}
}

void CFly::OnDead()
{
	m_eCurState = STATE::DEAD;
	memcpy(m_pFrameKey, L"fly_dead", DIR_LEN);
	m_dwDeadTimer = GetTickCount();
}

void CFly::OnTakeDamage()
{
}

