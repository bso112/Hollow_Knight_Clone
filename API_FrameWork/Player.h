#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "Obj.h"

class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	enum STATE { IDLE, WALK, ATTACK, HIT, JUMP, FALL, DEAD, END };
	//플레이어 정면
	enum FRONT { LEFT, RIGHT, FRONT_END};
	//공격 방향
	enum ATT_DIR { UP, DOWN, ATT_END};
	//점프 상태
	enum JUMP_STATE {STARTING, FALLING, LANDING, JUMP_END};


public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;



public:
	void Take_Damage(float _fDamage);


public:
	virtual void OnCollisionEnter(CObj* _pOther, float _fX, float _fY) override;
	virtual void OnCollisionStay(CObj* _pOther, float _fX, float _fY) override;
	virtual void OnCollisionExit(CObj* _pOther, float _fX, float _fY) override;

private:
	void Attack();


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

	// 점프. 실제로 점프(움직임)을 하는가?
	//m_eState == JUMP는 점프 애니메이션을 위한 것. 점프 애니메이션이 끝나야 다른 상태로 됨.
	bool			m_bJump;

	//점프 벡터
	const Vector2	m_JumpVelo;
	Vector2			m_curJumpVelo;
	Vector2			m_Gravity;
	float			m_fDeltaTime;

	//플레이어 상태
	STATE			m_eCurState;
	STATE			m_ePrvState;

	//플레이어의 스텟
	STAT			m_tStat;
	FRAME			m_tHealthUI;

	//플레이어 전 위치
	Vector2			m_prvPos;
	//플레이어 속도
	Vector2			m_velocity;

	TCHAR*			m_debug;

	//플레이어 정면
	FRONT			m_ePrvFront;
	FRONT			m_eFront;
	//공격 방향
	ATT_DIR			m_eAttDir;
	//콤보냐
	bool			m_bCombo;

	DWORD			m_ComboTimer;
	//콤보 사이의 최대 간격
	float			m_ComboWait;

	//점프상태
	JUMP_STATE			m_ePrvJumpState;
	JUMP_STATE			m_eJumpState;



};


#endif // !__PLAYER_H__
