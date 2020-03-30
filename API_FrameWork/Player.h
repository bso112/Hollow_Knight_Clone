#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "Obj.h"
#include "TileMgr.h"

class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	enum STATE { IDLE, WALK, ATTACK, HIT, JUMP, FALL, DEAD, END };
	//�÷��̾� ����
	enum FRONT { LEFT, RIGHT, FRONT_END};
	//���� ����
	enum ATT_DIR { UP, DOWN, ATT_END};
	//���� ����
	enum JUMP_STATE {STARTING, FALLING, LANDING, JUMP_END};


public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;



public:
	void Take_Damage(float _fDamage);
	//vDir �������� 1�ʿ� _fForce��ŭ�̵��ϴ� �ӷ����� _fTime �ʸ�ŭ �̵��Ѵ�.
	void Add_Force(Vector2 _vDir, float _fForce, float _fTime);

public:
	const STAT& Get_Stat() { return m_tStat; }


public:
	virtual void OnCollisionEnter(CObj* _pOther, float _fX, float _fY) override;
	virtual void OnCollisionStay(CObj* _pOther, float _fX, float _fY) override;
	virtual void OnCollisionExit(CObj* _pOther, float _fX, float _fY) override;

private:
	void Attack();
	void Hit();

private:
	void Key_Check();
	void Scene_Change();
	void Jumping();
	void OffSet();

	


	

private:
	template <typename T>
	CObj* Create_Bullet()
	{
		CObj* pObj = CAbstractFactory<T>::Create((float)m_tPosin.x, (float)m_tPosin.y, m_fAngle);
		return pObj;
	}

private:

	// ����. ������ ����(������)�� �ϴ°�?
	//m_eState == JUMP�� ���� �ִϸ��̼��� ���� ��. ���� �ִϸ��̼��� ������ �ٸ� ���·� ��.
	bool			m_bJump;

	//���� ����
	const Vector2	m_JumpVelo;
	Vector2			m_curJumpVelo;
	Vector2			m_Gravity;
	float			m_fDeltaTime;

	//�÷��̾� ����
	STATE			m_eCurState;
	STATE			m_ePrvState;

	//�÷��̾��� ����
	STAT			m_tStat;
	FRAME			m_tHealthUI;

	//�÷��̾� �� ��ġ
	Vector2			m_prvPos;
	//�÷��̾� �ӵ�
	Vector2			m_velocity;

	TCHAR*			m_debug;

	//�÷��̾� ����
	FRONT			m_ePrvFront;
	FRONT			m_eFront;
	//���� ����
	ATT_DIR			m_eAttDir;
	//�޺���
	bool			m_bCombo;

	DWORD			m_ComboTimer;
	//�޺� ������ �ִ� ����
	float			m_ComboWait;

	//��������
	JUMP_STATE		m_ePrvJumpState;
	JUMP_STATE		m_eJumpState;

	//�˹� Ÿ�̸�
	DWORD			m_dwForceTimer;
	//���� �ִ� �ð�
	float			m_fForceTime;

	//�ǰ� ����Ʈ Ÿ�̸�
	DWORD			m_dwHitTimer;
	//�ǰ� �ð�
	float			m_fHitTime;

	
	float			m_fAttCoolDown;
	CTileMgr::COLLISION m_eCollision;

};


#endif // !__PLAYER_H__
