#include "stdafx.h"
#include "Jumper.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "MyTime.h"

CJumper::CJumper()
	:m_bJump(false), m_JumpVelo(20.f, -70.f)
{
}


CJumper::~CJumper()
{
}

void CJumper::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Jumper/jumper_left.bmp", L"jumper_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Jumper/jumper_right.bmp", L"jumper_right");

	memcpy(m_pFrameKey, L"jumper_right", sizeof(TCHAR) * DIR_LEN);

	m_Gravity = Vector2(0, 2.f);
	m_curJumpVelo = m_JumpVelo;

	m_tInfo.iCX = 75;
	m_tInfo.iCY = 110;
	m_tImgInfo.iCX = 200;
	m_tImgInfo.iCY = 200;

	m_tStat = STAT(80);

	//스폰된 장소가 정찰의 중심점
	m_PartolSpot.x = (LONG)m_tInfo.fX;
	m_PartolSpot.y = (LONG)m_tInfo.fY;

	m_fJumpPower = 15.f;

	//인식범위
	m_fRadius = 300;
	//정찰범위
	m_fPatrol = 100;
	//속도
	m_fSpeed = 1.f;

	m_eCurState = STATE::IDLE;
}

int CJumper::Update()
{


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


	Move_Frame();

	float target_fX = m_pTarget->Get_INFO().fX;

	//델타타임 구하기
	m_fDeltaTime = CMyTime::Get_Instance()->Get_DeltaTime();
	//프레임사이의 간격이 너무 크면 안됨.
	if (m_fDeltaTime > 0.15f)
		m_fDeltaTime = 0.15f;




	//기본상태
	m_eCurState = STATE::IDLE;
	CTileMgr::COLLISION collision = CTileMgr::END;
	CTileMgr::Get_Instance()->Collision_Ex(this, collision);


	if (!m_bJump)
	{
		//점프일때는 따로 콜리전 체크해줌
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);

		//상시 중력적용
		m_tInfo.fY += m_Gravity.fY;
	}


	//타깃과의 거리측정
	m_distToTarget = (Vector2(m_pTarget->Get_INFO().fX, m_pTarget->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY)).magnitude();

	//타깃이 공격범위안에 들어오면 공격



	//타겟이 인식범위안에 들어오면
	if ((m_distToTarget < m_fRadius) || m_bJump)
	{
		//놀라고

		//추격상태가 된다.
		Chase_Target();
	}
	//점프가 끝나야만
	else
	{
		// 정찰
		Patrol();
	}

	Scene_Change();
	return 0;
}

void CJumper::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC, (int)m_tImgRect.left + iScrollX, (int)m_tImgRect.top + iScrollY
		, m_tImgInfo.iCX, m_tImgInfo.iCY, memDC, m_tImgInfo.iCX * m_tFrame.iFrameStart, m_tImgInfo.iCY *m_tFrame.iFrameScene, m_tImgInfo.iCX, m_tImgInfo.iCY
		, RGB(30, 30, 30));
}

void CJumper::Late_Update()
{
}



void CJumper::Release()
{
}

void CJumper::Patrol()
{


	//순찰범위를 벗어나면
	if (m_tInfo.fX < m_PartolSpot.x - m_fPatrol
		|| m_tInfo.fX > m_PartolSpot.x + m_fPatrol)
	{
		Vector2 dirToPatrol = Vector2((float)m_PartolSpot.x, (float)m_PartolSpot.y) - Vector2(m_tInfo.fX, m_tInfo.fY);
		//순찰 장소와의 거리측정
		float distToPatrol = dirToPatrol.magnitude();

		dirToPatrol = dirToPatrol.Nomalize();

		//도착할때까지
		if (distToPatrol > 2)
		{
			//순찰장소로 간다.
			m_tInfo.fX += dirToPatrol.fX * m_fSpeed;

			dirToPatrol.fX < 0 ? m_fDir = -abs(m_fDir) : m_fDir = abs(m_fDir);

		}


	}

}

void CJumper::Chase_Target()
{

	if (m_pTarget->Get_INFO().fX - m_tInfo.fX < 0)
		m_fDir = -1;
	else
		m_fDir = 1;



	static DWORD timer = GetTickCount();

	static float jumpDir = 0.f;
	//2초에 한번씩 점프
	if (timer + 2000 < GetTickCount())
	{
		timer = GetTickCount();
		m_bJump = true;
		jumpDir = m_fDir;
	}

	if (m_bJump)
	{
		//플레이어 쪽으로 이동
		m_tInfo.fX += m_fSpeed * m_fDir;

		m_eCurState = STATE::JUMP;
		//중력적용
		m_curJumpVelo += m_Gravity;


		m_tInfo.fX += m_curJumpVelo.fX * jumpDir * m_fDeltaTime;
		m_tInfo.fY += m_curJumpVelo.fY * m_fDeltaTime;

		//점프상태이고, 바닥과 충돌이면 점프해제
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);

		if (collision == CTileMgr::BOTTOM)
		{
			m_eCurState = STATE::WALK;
			//점프 초기화
			m_curJumpVelo = m_JumpVelo;
			m_bJump = false;
		}

	}


}

void CJumper::Scene_Change()
{
	//거리가 너무 가까워지면 정신없이 바뀜
	if (m_distToTarget > 1.f)
	{
		//방향에 따라 스프라이트 시트 바꾸기.
		if (m_fDir < 0)
			memcpy(m_pFrameKey, L"jumper_left", sizeof(TCHAR) * DIR_LEN);
		else
			memcpy(m_pFrameKey, L"jumper_right", sizeof(TCHAR) * DIR_LEN);
	}



	if (m_eCurState != m_ePrvState)
	{
		switch (m_eCurState)
		{
		case IDLE:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case WALK:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 150;
			m_tFrame.bLoop = true;
			break;
		}
		case JUMP:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}
		case DEAD:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 3;
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

void CJumper::OnDead()
{
}

void CJumper::OnTakeDamage()
{
}
