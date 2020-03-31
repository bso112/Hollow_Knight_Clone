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

	//������ ��Ұ� ������ �߽���
	m_PartolSpot.x = (LONG)m_tInfo.fX;
	m_PartolSpot.y = (LONG)m_tInfo.fY;

	//�νĹ���
	m_fRadius = 300;
	//��������
	m_fPatrol = 100;
	//�ӵ�
	m_fSpeed = 1.f;
}

int CFly::Update()
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


	CTileMgr::COLLISION collision = CTileMgr::END;
	CTileMgr::Get_Instance()->Collision_Ex(this, collision);

	if (m_eCurState != STATE::DEAD)
	{
		float target_fX = m_pTarget->Get_INFO().fX;

		//Ÿ����� �Ÿ�����
		m_distToTarget = (Vector2(m_pTarget->Get_INFO().fX, m_pTarget->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY)).magnitude();

		//�⺻����
		m_eCurState = STATE::IDLE;

		//Ÿ���� �νĹ����ȿ� ������
		if (m_tInfo.fX + m_fRadius >= target_fX
			&& m_tInfo.fX - m_fRadius <= target_fX)
		{
			//����

			//�߰ݻ��°� �ȴ�.
			Chase_Target();
		}
		else
			//�ƴϸ� ����
			Patrol();
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

void CFly::Late_Update()
{
}


void CFly::Release()
{
}

void CFly::Patrol()
{
	//������ҷ� ����.

	Vector2 dirToPatrol = Vector2((float)m_PartolSpot.x, (float)m_PartolSpot.y) - Vector2(m_tInfo.fX, m_tInfo.fY);
	//���� ��ҿ��� �Ÿ�����
	float distToPatrol = dirToPatrol.magnitude();

	dirToPatrol = dirToPatrol.Nomalize();

	//�����Ҷ�����
	if (distToPatrol > 2)
	{
		//������ҷ� ����.
		m_tInfo.fX += dirToPatrol.fX * m_fSpeed;
		m_tInfo.fY += dirToPatrol.fY * m_fSpeed;

		dirToPatrol.fX < 0 ? m_fDir = -abs(m_fDir) : m_fDir = abs(m_fDir);

	}


}

void CFly::Chase_Target()
{

	//�÷��̾� �i��
	m_eCurState = STATE::RUN;


	float fX = m_pTarget->Get_INFO().fX - m_tInfo.fX;
	float fY = m_pTarget->Get_INFO().fY - m_tInfo.fY;
	float dia = sqrtf(fX * fX + fY * fY);

	//�Ÿ��� 5���� ������
	if (dia < 5.f)
	{
		//����
		return;
	}
	float theta = acosf(fX / dia);


	//Ÿ���� �� �Ʒ��� ������ 
	if (m_tInfo.fY < m_pTarget->Get_INFO().fY)
		theta = (2 * PI) - theta;

	float fDirX = cosf(theta);
	float fDirY = sinf(theta);

	float chasing_speed = m_fSpeed + 1;
	m_tInfo.fX += fDirX * chasing_speed;
	//������ ��ǥ��� ��ī��Ʈ ��ǥ���� �ݴ�ϱ�.
	m_tInfo.fY -= fDirY * chasing_speed;

	//��ȣ �̾Ƴ���
	m_fDir = fDirX / abs(fDirX);


}


void CFly::Scene_Change()
{

	//�Ÿ��� �ʹ� ��������� ���ž��� �ٲ�
	if (m_distToTarget > 1.f)
	{
		//���⿡ ���� ��������Ʈ ��Ʈ �ٲٱ�.
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

