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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/jumper_left.bmp", L"jumper_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/jumper_right.bmp", L"jumper_right");

	memcpy(m_pFrameKey, L"jumper_right", sizeof(TCHAR) * DIR_LEN);

	m_Gravity = Vector2(0, 2.f);
	m_curJumpVelo = m_JumpVelo;

	m_tInfo.iCX = 150;
	m_tInfo.iCY = 150;
	m_tStat = STAT(80);

	//������ ��Ұ� ������ �߽���
	m_PartolSpot.x = (LONG)m_tInfo.fX;
	m_PartolSpot.y = (LONG)m_tInfo.fY;

	m_fJumpPower = 15.f;

	//�νĹ���
	m_fRadius = 300;
	//��������
	m_fPatrol = 100;
	//�ӵ�
	m_fSpeed = 1.f;

	m_eCurState = STATE::IDLE;
}

int CJumper::Update()
{
	Move_Frame();

	float target_fX = m_pTarget->Get_INFO().fX;

	//��ŸŸ�� ���ϱ�
	m_fDeltaTime = CMyTime::Get_Instance()->Get_DeltaTime();
	//�����ӻ����� ������ �ʹ� ũ�� �ȵ�.
	if (m_fDeltaTime > 0.15f)
		m_fDeltaTime = 0.15f;




	//�⺻����
	m_eCurState = STATE::IDLE;
	CTileMgr::COLLISION collision = CTileMgr::END;
	CTileMgr::Get_Instance()->Collision_Ex(this, collision);


	if (!m_bJump)
	{
		//�����϶��� ���� �ݸ��� üũ����
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);

		//��� �߷�����
		m_tInfo.fY += m_Gravity.fY;
	}


	//Ÿ����� �Ÿ�����
	m_distToTarget = (Vector2(m_pTarget->Get_INFO().fX, m_pTarget->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY)).magnitude();

	//Ÿ���� ���ݹ����ȿ� ������ ����



	//Ÿ���� �νĹ����ȿ� ������
	if ((m_distToTarget < m_fRadius) || m_bJump)
	{
		//����

		//�߰ݻ��°� �ȴ�.
		Chase_Target();
	}
	//������ �����߸�
	else
	{
		// ����
		Patrol();
	}

	Scene_Change();
	return 0;
}

void CJumper::Late_Update()
{
}



void CJumper::Release()
{
}

void CJumper::Patrol()
{


	//���������� �����
	if (m_tInfo.fX < m_PartolSpot.x - m_fPatrol
		|| m_tInfo.fX > m_PartolSpot.x + m_fPatrol)
	{
		Vector2 dirToPatrol = Vector2((float)m_PartolSpot.x, (float)m_PartolSpot.y) - Vector2(m_tInfo.fX, m_tInfo.fY);
		//���� ��ҿ��� �Ÿ�����
		float distToPatrol = dirToPatrol.magnitude();

		dirToPatrol = dirToPatrol.Nomalize();

		//�����Ҷ�����
		if (distToPatrol > 2)
		{
			//������ҷ� ����.
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
	//2�ʿ� �ѹ��� ����
	if (timer + 2000 < GetTickCount())
	{
		timer = GetTickCount();
		m_bJump = true;
		jumpDir = m_fDir;
	}

	if (m_bJump)
	{
		//�÷��̾� ������ �̵�
		m_tInfo.fX += m_fSpeed * m_fDir;

		m_eCurState = STATE::JUMP;
		//�߷�����
		m_curJumpVelo += m_Gravity;


		m_tInfo.fX += m_curJumpVelo.fX * jumpDir * m_fDeltaTime;
		m_tInfo.fY += m_curJumpVelo.fY * m_fDeltaTime;

		//���������̰�, �ٴڰ� �浹�̸� ��������
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);

		if (collision == CTileMgr::BOTTOM)
		{
			m_eCurState = STATE::WALK;
			//���� �ʱ�ȭ
			m_curJumpVelo = m_JumpVelo;
			m_bJump = false;
		}

	}


}

void CJumper::Scene_Change()
{
	//�Ÿ��� �ʹ� ��������� ���ž��� �ٲ�
	if (m_distToTarget > 1.f)
	{
		//���⿡ ���� ��������Ʈ ��Ʈ �ٲٱ�.
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
			m_tFrame.iFrameEnd = 2;
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
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case JUMP:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}
		case ATTACK:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 500;
			m_tFrame.bLoop = false;
			break;
		}
		case DEAD:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 4;
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
