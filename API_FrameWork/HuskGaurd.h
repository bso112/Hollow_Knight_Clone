#pragma once
#ifndef __HUSKGAURD_H__
#define __HUSKGAURD_H__

#define SPEED 3.f
#define JUMP_VELO Vector2(200.f, -100.f);
#include "Monster.h"

class CHuskGaurd : public CMonster
{
public:
	CHuskGaurd();
	virtual ~CHuskGaurd();

public:
	enum STATE { IDLE, WAKEUP, WALK, ATTACK, DASH, JUMP, DEAD, END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Late_Update() override;
	virtual void Release() override;

public:
	virtual void Patrol() override;
	virtual void Chase_Target() override;
	virtual void Scene_Change() override;
	virtual void OnDead() override;
	virtual void OnTakeDamage() override;

private:
	void Process();
	void Update_State();
	
private:
	void Jumping();
	void Attack();

public:
	STATE			m_eCurState;
	STATE			m_ePrvState;

	bool			m_bWaked;
	bool			m_bJump;

	
	Vector2			m_vToTarget;
	float			m_fDistToTarget;
	//점프 벡터
	const Vector2	m_JumpVelo;
	Vector2			m_curJumpVelo;

	float			m_fAttCoolDown;

	//패턴 변화를 위한 공격카운트
	int				m_iAttCnt;
};

#endif // !__HUSKGAURD_H__


