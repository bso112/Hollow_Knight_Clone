#include "stdafx.h"
#include "PillBug.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "MyTime.h"

CPillBug::CPillBug()
{
}


CPillBug::~CPillBug()
{
}

void CPillBug::Initialize()
{


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/PillBug/move.bmp", L"pillBug_move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/PillBug/dead.bmp", L"pillBug_dead");

	memcpy(m_pFrameKey, L"pillBug_move", DIR_LEN);
	m_tInfo.iCX = 120;
	m_tInfo.iCY = 70;
	m_tImgInfo.iCX = 256;
	m_tImgInfo.iCY = 256;

	m_eFront = FRONT::RIGHT;
	m_eCurState = STATE::MOVE;
	m_ePrvState = STATE::END;

	m_tStat = STAT(30);

	//������ ��Ұ� ������ �߽���
	m_PartolSpot.x = (LONG)m_tInfo.fX;
	m_PartolSpot.y = (LONG)m_tInfo.fY;

	m_fPatrol = 1000;

	m_fSpeed = 2.f;

	m_fDeadWait = 2.f;


}

int CPillBug::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

#pragma region �˹�

	//��ŸŸ�� ���ϱ�
	m_fDeltaTime = CMyTime::Get_Instance()->Get_DeltaTime();
	//�����ӻ����� ������ �ʹ� ũ�� �ȵ�.
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


	if (m_eCurState != STATE::DEAD)
	{
		Patrol();
		m_tInfo.fY += m_Gravity.fY;
	}
	else //�׾�����
	{
		//�����ð��� �ı�
		if (m_dwDeadTimer + m_fDeadWait * 1000 < GetTickCount())
			m_bDead = true;
	}
		


	Move_Frame();
	Scene_Change();

	return OBJ_NOEVENT;
}

void CPillBug::Late_Update()
{
}



void CPillBug::Release()
{
}

void CPillBug::Patrol()
{
	m_eCurState = STATE::MOVE;



	//�����϶��� ���� �ݸ��� üũ����
	CTileMgr::COLLISION collision = CTileMgr::END;
	CTileMgr::Get_Instance()->Collision_Ex(this, collision);

	m_tInfo.fX += m_fSpeed;

	if (collision == CTileMgr::COLLISION::LEFT)
	{
		m_eFront = FRONT::RIGHT;
		m_fSpeed *= -1.f;
	}

	else if (collision == CTileMgr::COLLISION::RIGHT)
	{
		m_eFront = FRONT::LEFT;
		m_fSpeed *= -1.f;
	}
}

void CPillBug::Chase_Target()
{
}

void CPillBug::OnTakeDamage()
{
}

void CPillBug::OnDead()
{
	m_eCurState = STATE::DEAD;
	memcpy(m_pFrameKey, L"pillBug_dead", DIR_LEN);
	m_dwDeadTimer = GetTickCount();
}

void CPillBug::Scene_Change()
{

	if (m_eCurState != m_ePrvState || m_eFront != m_ePrvFront)
	{
		switch (m_eCurState)
		{
		case STATE::MOVE:
		{
			memcpy(m_pFrameKey, L"pillBug_move", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case STATE::DEAD:
		{
			memcpy(m_pFrameKey, L"pillBug_dead", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = m_eCurState;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}

		}
		m_ePrvState = m_eCurState;
		m_ePrvFront = m_eFront;
	}
}
