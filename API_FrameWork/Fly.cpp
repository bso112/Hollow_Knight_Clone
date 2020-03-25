#include "stdafx.h"
#include "Fly.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CFly::CFly()
{
}


CFly::~CFly()
{
}

void CFly::Initialize()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/fly_left.bmp", L"fly_left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/fly_right.bmp", L"fly_right");

	memcpy(m_pFrameKey, L"fly_right", sizeof(TCHAR) * DIR_LEN);

	m_tInfo.iCX = 120;
	m_tInfo.iCY = 115;
	m_tStat = STAT(80);

	//������ ��Ұ� ������ �߽���
	m_PartolSpot.x = (LONG)m_tInfo.fX;
	m_PartolSpot.y = (LONG)m_tInfo.fY;

	//�νĹ���
	m_fRadius = 400;
	//��������
	m_fPatrol = 100;
	//�ӵ�
	m_fSpeed = 1.f;
}

int CFly::Update()
{
	Move_Frame();

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

	Scene_Change();
	return 0;
}

void CFly::Late_Update()
{
}

void CFly::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY,
		m_tInfo.iCX, m_tInfo.iCY, memDC, m_tInfo.iCX * m_tFrame.iFrameStart, m_tInfo.iCY * m_tFrame.iFrameScene,
		m_tInfo.iCX, m_tInfo.iCY, RGB(30, 30, 30));
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
	if (m_distToTarget > 5.f)
	{
		//���⿡ ���� ��������Ʈ ��Ʈ �ٲٱ�.
		if (m_fDir < 0)
			memcpy(m_pFrameKey, L"fly_left", sizeof(TCHAR) * DIR_LEN);
		else
			memcpy(m_pFrameKey, L"fly_right", sizeof(TCHAR) * DIR_LEN);
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
			break;
		}
		case WALK:
		{

		}
		case RUN:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		case HIT:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 500;
			break;
		}
		case DEAD:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
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
