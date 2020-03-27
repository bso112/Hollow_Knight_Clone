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
	virtual void Take_Damage(float _fDamage) { m_tStat.m_fHp -= _fDamage; if (m_tStat.m_fHp < 0) m_tStat.m_fHp = 0; m_eCurState = STATE::HIT; }

private:
	STATE			m_eCurState;
	STATE			m_ePrvState;
	//Ÿ����� �����Ÿ�
	float			m_distToTarget;

};

