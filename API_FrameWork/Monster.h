#pragma once

#ifndef __MONSTER_H__
#define __MONSTER_H__


#include "Obj.h"
class CMonster : public CObj
{
public:
	CMonster();
	virtual ~CMonster();

public:
	virtual void Initialize()  = 0;
	virtual int Update()  = 0;
	virtual void Late_Update()  = 0;
	virtual void Render(HDC _DC)  = 0;
	virtual void Release() = 0 ;

protected:
	virtual void Patrol() = 0;
	virtual void Chase_Target() = 0;
	//스프라이트 시트 체인지
	virtual void Scene_Change() = 0;
	
protected:
	//인식범위에 플레이어가 들어왔는가?
	bool IsAlert();

public:
	virtual void Take_Damage(float _fDamage) { m_tStat.m_fHp -= _fDamage; if (m_tStat.m_fHp < 0) m_tStat.m_fHp = 0; }

public:
	virtual void OnCollisionEnter(CObj* _pOther, float _fX, float _fY) override;

protected:
	STAT m_tStat;
	//인식범위
	float m_fRadius;
	//정찰범위
	float m_fPatrol;
	//정찰 기준점
	POINT m_PartolSpot;
	//향하는 방향(x축) ..스프라이트 시트 구분을 위함
	float m_fDir;
	
};


#endif // !__MONSTER_H__
