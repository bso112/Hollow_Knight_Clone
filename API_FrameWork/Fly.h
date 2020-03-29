#pragma once
#ifndef __FLY_H__
#define __FLY_H__

#include "Monster.h"
class CFly : public CMonster
{
public:
	CFly();
	virtual ~CFly();

	enum STATE { IDLE, WALK, RUN, HIT, DEAD, END };

public:
	// CMonster을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Release() override;

protected:
	virtual void Patrol() override;
	virtual void Chase_Target() override;
	virtual void Scene_Change() override;

protected:
	// CMonster을(를) 통해 상속됨
	virtual void OnDead()		override;
	virtual void OnTakeDamage() override;


private:
	STATE			m_eCurState;
	STATE			m_ePrvState;

	Vector2			m_Velocity;
	//타깃과의 유지거리
	float			m_distToTarget;

	// CMonster을(를) 통해 상속됨
};



#endif
