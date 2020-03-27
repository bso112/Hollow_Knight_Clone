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
	// CMonster을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Release() override;
	virtual void Scene_Change() override;


protected:
	virtual void Patrol() override;
	virtual void Chase_Target() override;


};

#define __PILLBUG_H__
