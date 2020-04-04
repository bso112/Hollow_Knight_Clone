#pragma once
#include "Monster.h"

#define JUMP_VELO Vector2(250.f, -150.f);

class CFalseKnight : public CMonster
{
public:
	CFalseKnight();
	virtual ~CFalseKnight();

public:

	enum STATE { IDLE, ATTACK, REMOTE_ATTACK, JUMP, SWING_AROUND, DOWN, GROGGY, GROGGY_STAND, GROGGY_HIT, DEAD, EMPTY, END };

public:
	// CMonster을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	
private:
	void Process();
	void Update_State();

protected:
	virtual void Patrol() override;
	virtual void Chase_Target() override;
	virtual void Scene_Change() override;
	virtual void OnDead() override;
	virtual void OnTakeDamage() override;

private:
	void Jumping();
	void Attack();

	void RemoteAttack();
	void Swing_Around();


public:
	STATE			m_eCurState;
	STATE			m_ePrvState;

	bool			m_bJump;
	Vector2			m_curJumpVelo;
	Vector2			m_vToTarget;
	float			m_fDistToTarget;

	int				m_iAttCnt;



	bool			m_bHit;
	bool			m_bWasGroggy;

	DWORD			m_dwDownTimer;



};

