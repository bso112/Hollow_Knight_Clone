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
	// CMonster��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Release() override;

protected:
	virtual void Patrol() override;
	virtual void Chase_Target() override;
	virtual void Scene_Change() override;
	virtual void Take_Damage(float _fDamage)
	{
		m_tStat.m_fHp -= _fDamage;
		if (m_tStat.m_fHp < 0)
		{
			m_tStat.m_fHp = 0;
			m_eCurState = STATE::DEAD;
		}
		m_eCurState = STATE::HIT;
	}

protected:
	// CMonster��(��) ���� ��ӵ�
	virtual void OnDead() override;


private:
	STATE			m_eCurState;
	STATE			m_ePrvState;

	Vector2			m_Velocity;
	//Ÿ����� �����Ÿ�
	float			m_distToTarget;
};



#endif
