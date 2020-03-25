#pragma once
#ifndef __JUMPER_H__
#define __JUMPER_H__

#include "Monster.h"

class CJumper : public CMonster
{
public:
	CJumper();
	~CJumper();

public:
	enum STATE { IDLE, WALK, JUMP, ATTACK, DEAD, END };

public:

	// CMonster을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

protected:
	virtual void Patrol() override;
	virtual void Chase_Target() override;
	virtual void Scene_Change() override;


private:
	STATE			m_eCurState;
	STATE			m_ePrvState;
	bool			m_bJump;
	float			m_fJumpPower;
	float			m_fJumpAccel;
	
	//점프 벡터
	const Vector2	m_JumpVelo;
	Vector2			m_curJumpVelo;
	Vector2			m_Gravity;
	float			m_fDeltaTime;

	//타깃과의 유지거리
	float			m_distToTarget;

};

#endif
