#pragma once

#ifndef __PILLBUG_H__
#endif

#include "Monster.h"
class CPillBug : public CMonster
{
public:
	CPillBug();
	~CPillBug();

public:
	enum STATE { MOVE, DEAD, END };


public:
	// CMonster을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Release() override;
	virtual void Scene_Change() override;

public:
	// CMonster을(를) 통해 상속됨
	virtual void OnDead() override;
	virtual void OnTakeDamage() override;

protected:
	virtual void Patrol() override;
	virtual void Chase_Target() override;

public:
	STATE			m_eCurState;
	STATE			m_ePrvState;




	// CMonster을(를) 통해 상속됨

};

#define __PILLBUG_H__
