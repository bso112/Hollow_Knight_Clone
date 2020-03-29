#include "stdafx.h"
#include "HuskGaurd.h"
#include "MyTime.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "Weapon.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"

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

	m_tInfo.iCX = 213;
	m_tInfo.iCY = 333;
	m_tImgInfo.iCX = 1024;
	m_tImgInfo.iCY = 640;

	m_eFront = FRONT::RIGHT;
	m_eCurState = STATE::IDLE;
	m_ePrvState = STATE::END;

	m_tStat = STAT(300);
	m_fRadius = 500.f;
	m_fAttRange = 500.f;

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

	memcpy(m_pFrameKey, L"huskGaurd_standby", DIR_LEN);


}

void CHuskGaurd::Scene_Change()
{

	//�Ÿ��� �ʹ� ��������� ���ž��� �ٲ�
	if (m_fDistToTarget > 1.f)
	{
		//�̵����� �����ֱ�
		m_vToTarget.fX > 0 ? m_eFront = FRONT::RIGHT : m_eFront = FRONT::LEFT;
	}

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
			m_tFrame.bLoop = true;
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
		Update_State();
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

void CHuskGaurd::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll_X();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_Scroll_Y();

	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	//�׸��� �߽ɼ��� �ȸ¾Ƽ� �׸��� �������ش�.
	float offSet = 50;

	GdiTransparentBlt(_DC, (int)m_tImgRect.left + iScrollX, (int)m_tImgRect.top + iScrollY - offSet
		, m_tImgInfo.iCX, m_tImgInfo.iCY, memDC, m_tImgInfo.iCX * m_tFrame.iFrameScene, m_tImgInfo.iCY *m_tFrame.iFrameStart, m_tImgInfo.iCX, m_tImgInfo.iCY
		, RGB(30, 30, 30));

#pragma region �����

	TCHAR		szBuff[32] = L"����: ";
	TCHAR*		state = nullptr;
	switch (m_eCurState)
	{
	case CHuskGaurd::IDLE:
		state = L"�޽�";
		break;
	case CHuskGaurd::WAKEUP:
		state = L"��";
		break;
	case CHuskGaurd::WALK:
		state = L"�ȱ�";
		break;
	case CHuskGaurd::ATTACK:
		state = L"����";
		break;
	case CHuskGaurd::DASH:
		state = L"���";
		break;
	case CHuskGaurd::JUMP:
		state = L"����";
		break;
	case CHuskGaurd::DEAD:
		state = L"����";
		break;
	case CHuskGaurd::END:
		break;
	default:
		break;
	}
	lstrcat(szBuff, state);
	TextOut(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, szBuff, lstrlen(szBuff));


#pragma endregion

}

void CHuskGaurd::Late_Update()
{
	//����߷�
	if (!m_bJump)
	{
		m_tInfo.fY += m_Gravity.fY;
		//�����϶��� ���� �ݸ��� üũ����
		CTileMgr::COLLISION collision = CTileMgr::END;
		CTileMgr::Get_Instance()->Collision_Ex(this, collision);

	}
}



void CHuskGaurd::Release()
{
}

void CHuskGaurd::Patrol()
{
}

void CHuskGaurd::Chase_Target()
{

	Vector2 dir = m_vToTarget.Nomalize();

	m_tInfo.fX += dir.fX * m_fSpeed;

}


void CHuskGaurd::OnDead()
{
	m_eCurState = STATE::DEAD;
	m_dwDeadTimer = GetTickCount();
}

void CHuskGaurd::OnTakeDamage()
{
}

void CHuskGaurd::Process()
{

	//Ÿ������ ���� ���⺤��
	m_vToTarget = Vector2(m_pTarget->Get_INFO().fX, m_pTarget->Get_INFO().fY) - Vector2(m_tInfo.fX, m_tInfo.fY);
	m_fDistToTarget = m_vToTarget.magnitude();

	if (m_bWaked)
	{
		switch (m_eCurState)
		{
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

	//Ÿ���� �νĹ����ȿ� ������ ����
	if (m_fDistToTarget < m_fRadius && !m_bWaked)
	{
		m_eCurState = STATE::WAKEUP;
		//���� �ִϸ��̼� �������� ���� �ൿ����
		if(m_tFrame.iFrameEnd != 0 &&  m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			m_bWaked = true;
	}

	if (m_bWaked)
	{
		//Ÿ���� ���ݹ��� ���̸�
		if (abs(m_vToTarget.fX) > m_fAttRange)
		{
			m_eCurState = STATE::WALK;

			if (abs(m_vToTarget.fX) > m_fAttRange + 100)
				m_eCurState = STATE::DASH;

		}
		//����
		else
		{
			static DWORD mode_swith_delay = 0;
			//������ 5�ʿ� �ѹ� �ٲ�
			static DWORD att_mode_timer = GetTickCount();
			if (att_mode_timer + mode_swith_delay < GetTickCount())
			{
				//�������� �� �� �ϳ�
				int behaviour = rand() % 2;
				if (behaviour == 1)
					m_eCurState = STATE::ATTACK;
				else
					m_eCurState = STATE::JUMP;

				mode_swith_delay = 5000;
				att_mode_timer = GetTickCount();
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
		Vector2 vDir = m_vToTarget.Nomalize();
		float fDir = vDir.fX < 0 ? -1 : 1;


		m_fDeltaTime = CMyTime::Get_Instance()->Get_DeltaTime();
		if (m_fDeltaTime > 0.15f)
			m_fDeltaTime = 0.15f;

		// * m_fDeltaTime�� ���ָ� 1�ʿ� m_curJumpVelo��ŭ �̵��Ѵ�.
		m_tInfo.fX += m_curJumpVelo.fX * fDir * m_fDeltaTime;
		m_tInfo.fY += m_curJumpVelo.fY * m_fDeltaTime;

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

			frame.iFrameStart = 0;
			frame.iFrameEnd = 5;
			frame.iFrameScene = m_eFront;
			frame.dwFrameTime = GetTickCount();
			frame.dwFrameSpeed = 50;
			frame.bLoop = false;

			INFO info;
			INFO imgInfo;

			int dir = 0;
			m_vToTarget.fX > 0 ? dir = 1 : dir = -1;

			int margin = (m_tInfo.iCX >> 1) * dir;

			info.iCX = 112;
			info.iCY = 196;
			info.fX = m_tInfo.fX + margin;
			info.fY = (float)m_tRect.bottom - (info.iCY >> 1);
			imgInfo.iCX = 128;
			imgInfo.iCX = 256;

			weapon = CAbstractFactory<CWeapon>::Create(info, imgInfo, L"jump_eff", frame);
			CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, weapon);
			dynamic_cast<CWeapon*>(weapon)->Add_Force(Vector2(dir, 0), 50, 1.f);
			dynamic_cast<CWeapon*>(weapon)->Set_Owner(CWeapon::OWNER::MONSTER);

#pragma endregion


		}

	}

}

void CHuskGaurd::Attack()
{
	//���ø�� �߰������ �浹�ڽ� ����
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd - m_tFrame.iFrameStart)
	{
		CObj* weapon = nullptr;
		int dir = 0;
		m_vToTarget.fX > 0 ? dir = 1 : dir = -1;

		float margin = (m_tInfo.iCX >> 1) * dir;

		int iCX = 300;
		int iCY = 250;
		float fX = dir == 1 ? m_tRect.right + (iCX >> 1) : m_tRect.left - (iCX >> 1);
		float fY = m_tRect.bottom - (iCY >> 1);
		weapon = CAbstractFactory<CWeapon>::Create(fX, fY);

		if (weapon)
		{
			weapon->Set_Size(iCX, iCY);
			//�ǰݹ��� ����
			//0.1�ʵ��ȸ� ����
			dynamic_cast<CWeapon*>(weapon)->Set_Duration(0.05f);
			dynamic_cast<CWeapon*>(weapon)->Set_Owner(CWeapon::OWNER::MONSTER);
			CObjMgr::Get_Instance()->Add_Object(OBJID::WEAPON, weapon);
		}
	}

}

