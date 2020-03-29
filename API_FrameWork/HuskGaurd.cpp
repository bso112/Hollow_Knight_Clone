#include "stdafx.h"
#include "HuskGaurd.h"
#include "MyTime.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "Weapon.h"
#include "ObjMgr.h"

CHuskGaurd::CHuskGaurd()
	:m_bWaked(false), m_bJump(false)
{
}


CHuskGaurd::~CHuskGaurd()
{
}

void CHuskGaurd::Initialize()
{
	m_fAttCoolDown = 0.2f;
	m_curJumpVelo = JUMP_VELO;

	memcpy(m_pFrameKey, L"pillBug_move", DIR_LEN);
	m_tInfo.iCX = 120;
	m_tInfo.iCY = 70;
	m_tImgInfo.iCX = 1024;
	m_tImgInfo.iCY = 640;

	m_eFront = FRONT::RIGHT;
	m_eCurState = STATE::IDLE;
	m_ePrvState = STATE::END;

	m_tStat = STAT(300);
	m_fRadius = 300.f;
	m_fAttRange = 300.f;

	m_fSpeed = SPEED;

	m_fDeadWait = 10.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/HuskGaurd/attack.bmp", L"huskGaurd_attack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/HuskGaurd/dash.bmp", L"huskGaurd_dash");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/HuskGaurd/jump.bmp", L"huskGaurd_jump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/HuskGaurd/dead.bmp", L"huskGaurd_dead");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/HuskGaurd/move.bmp", L"huskGaurd_move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/HuskGaurd/standby.bmp", L"huskGaurd_standby");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/HuskGaurd/wakeup.bmp", L"huskGaurd_wakeup");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/HuskGaurd/jump_effect/jump_eff.bmp", L"jump_eff");



}

void CHuskGaurd::Scene_Change()
{


	if (m_eCurState != m_ePrvState || m_eFront != m_ePrvFront)
	{
		
		switch (m_eCurState)
		{
		case CHuskGaurd::IDLE:
		{
			memcpy(m_pFrameKey, L"huskGaurd_standby", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = false;
			break;
		}
		case CHuskGaurd::WAKEUP:
		{
			memcpy(m_pFrameKey, L"huskGaurd_wakeup", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = false;
			break;
		}
		case CHuskGaurd::WALK:
		{
			memcpy(m_pFrameKey, L"huskGaurd_move", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.bLoop = true;
			break;
		}
		case CHuskGaurd::ATTACK:
		{
			memcpy(m_pFrameKey, L"huskGaurd_attack", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = false;
			break;
		}
		case CHuskGaurd::DASH:
		{
			memcpy(m_pFrameKey, L"huskGaurd_dash", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = true;
			break;
		}
		case CHuskGaurd::JUMP:
		{
			memcpy(m_pFrameKey, L"huskGaurd_jump", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.bLoop = false;
			break;
		}
		case CHuskGaurd::DEAD:
		{
			memcpy(m_pFrameKey, L"huskGaurd_dead", DIR_LEN);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameScene = m_eFront;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 150;
			m_tFrame.bLoop = false;
			break;
		}
		case CHuskGaurd::END:
			break;
		default:
			break;
		}

		m_ePrvState = m_eCurState;
		m_ePrvFront = m_eFront;
	}
}

int CHuskGaurd::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_eCurState != STATE::DEAD)
	{
		Process();

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

void CHuskGaurd::Late_Update()
{
}



void CHuskGaurd::Release()
{
}

void CHuskGaurd::Patrol()
{
}

void CHuskGaurd::Chase_Target()
{
	//�Ÿ��� �ʹ� ��������� ���ž��� �ٲ�
	if (m_fDistToTarget > 1.f)
	{
		//�̵����� �����ֱ�
		m_vToTarget.fX > 0 ? m_eFront = FRONT::RIGHT : m_eFront = FRONT::LEFT;
	}

	Vector2 dir = m_vToTarget.Nomalize();

	m_tInfo.fX += dir.fX * m_fSpeed;

}


void CHuskGaurd::OnDead()
{
	m_eCurState = STATE::DEAD;
	memcpy(m_pFrameKey, L"knight_die", DIR_LEN);
	m_dwDeadTimer = GetTickCount();
}

void CHuskGaurd::OnTakeDamage()
{
}

void CHuskGaurd::Process()
{
	//����߷�
	if (!m_bJump)
		m_tInfo.fY += m_Gravity.fY;

	//Ÿ������ ���� ���⺤��
	m_vToTarget = Vector2(m_pTarget->Get_INFO().fX, m_pTarget->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY);
	m_fDistToTarget = m_vToTarget.magnitude();

	if (m_bWaked)
	{
		switch (m_eCurState)
		{
		case CHuskGaurd::IDLE:
		{

			break;
		}
		case CHuskGaurd::WALK:
		{
			m_fSpeed = SPEED;
			Chase_Target();
			break;
		}
		case CHuskGaurd::ATTACK:
		{
			Attack();
			break;
		}
		case CHuskGaurd::DASH:
		{
			m_fSpeed += 3;
			Chase_Target();
			break;
		}
		case CHuskGaurd::JUMP:
		{
			m_bJump = true;
			break;
		}
		case CHuskGaurd::END:
			break;
		default:
			break;
		}


	}


	Jumping();
}

void CHuskGaurd::Update_State()
{

	//Ÿ���� �νĹ����ȿ� ������
	if (m_fDistToTarget < m_fRadius && !m_bWaked)
	{
		m_eCurState = STATE::WAKEUP;
		m_bWaked = true;
	}

	//Ÿ���� �ν�������
	if (m_bWaked)
	{
		//Ÿ���� ���ݹ��� ���̸�
		if (m_fDistToTarget > m_fAttRange)
		{
			m_eCurState = STATE::WALK;

			if (m_fDistToTarget > m_fAttRange + 100)
				m_eCurState = STATE::DASH;

		}
		else
		{
			static DWORD attTimer = GetTickCount();
			if (attTimer + m_fAttCoolDown * 1000 < GetTickCount())
			{
				//�������� �� �� �ϳ�
				int behaviour = rand() % 2;
				if (behaviour == 1)
					m_eCurState = STATE::ATTACK;
				else
					m_eCurState = STATE::JUMP;

				attTimer = GetTickCount();
			}
		}
	}

}

void CHuskGaurd::Jumping()
{
	if (m_bJump)
	{
		//�߷�����
		m_curJumpVelo += m_Gravity;

		// * m_fDeltaTime�� ���ָ� 1�ʿ� m_curJumpVelo��ŭ �̵��Ѵ�.
		m_tInfo.fX += m_curJumpVelo.fX * CMyTime::Get_Instance()->Get_DeltaTime();
		m_tInfo.fY += m_curJumpVelo.fY * CMyTime::Get_Instance()->Get_DeltaTime();

		//���������̰�, �ٴڰ� �浹�̸� ��������
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);


		if (collision == CTileMgr::BOTTOM)
		{
			//���� �ʱ�ȭ 
			m_curJumpVelo = m_JumpVelo;
			m_bJump = false;

#pragma region ����Ʈ����

			CObj* weapon = nullptr;
			FRAME frame;

			switch (m_eFront)
			{
			case CHuskGaurd::LEFT:
			{
				frame.iFrameStart = 0;
				frame.iFrameEnd = 1;
				frame.iFrameScene = 0;
				frame.dwFrameTime = GetTickCount();
				frame.dwFrameSpeed = 50;
				frame.bLoop = false;
				break;
			}
			case CHuskGaurd::RIGHT:
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

			int dir = 0;
			m_vToTarget.fX > 0 ? dir = 1 : dir = -1;

			int margin = (m_tInfo.iCX >> 1) * dir;
			
			info.iCX = 256;
			info.iCY = 256;
			info.fX = m_tInfo.fX + margin;
			info.fY = (float)m_tRect.bottom - (info.iCY >>1);

			weapon = CAbstractFactory<CWeapon>::Create(m_tInfo.fX + margin, m_tInfo.fY);

#pragma endregion


		}

	}

}

void CHuskGaurd::Attack()
{
	CObj* weapon = nullptr;


	int dir = 0;
	m_vToTarget.fX > 0 ? dir = 1 : dir = -1;

	float margin = 20 * dir;

	weapon = CAbstractFactory<CWeapon>::Create(m_tInfo.fX + margin, m_tInfo.fY);

	if (weapon)
	{
		//�ǰݹ��� ����
		weapon->Set_Size(100, 300);
		//0.1�ʵ��ȸ� ����
		dynamic_cast<CWeapon*>(weapon)->Set_Duration(0.1f);
		dynamic_cast<CWeapon*>(weapon)->Set_Owner(CWeapon::OWNER::MONSTER);
		CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, weapon);
	}
}

