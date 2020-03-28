#pragma once
#include "Monster.h"
class CFalseKnight : public CMonster
{
public:
	CFalseKnight();
	virtual ~CFalseKnight();

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
	virtual void OnDead() override;
	virtual void OnTakeDamage() override;



};

