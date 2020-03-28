#pragma once
#include "Monster.h"
class CChaser : public CMonster
{
public:
	CChaser();
	virtual ~CChaser();

public:
	enum STATE { IDLE, WALK, RUN, HIT, DEAD, END };


	// CMonster��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Release() override;

public:
	virtual void Patrol() override;
	virtual void Chase_Target() override;
	virtual void Scene_Change() override;

protected:
	// CMonster��(��) ���� ��ӵ�
	virtual void OnDead() override
	{
		m_eCurState = STATE::DEAD;
	}
	virtual void OnTakeDamage() override
	{
		m_eCurState = STATE::HIT;
	};

private:
	STATE			m_eCurState;
	STATE			m_ePrvState;
	//Ÿ����� �����Ÿ�
	float			m_distToTarget;


	// CMonster��(��) ���� ��ӵ�

};

