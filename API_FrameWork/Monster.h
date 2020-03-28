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
	//정면
	enum FRONT { LEFT, RIGHT, FRONT_END };

public:
	virtual void Initialize()  = 0;
	virtual int Update()  = 0;
	virtual void Late_Update()  = 0;
	virtual void Render(HDC _DC);
	virtual void Release() = 0 ;

protected:
	virtual void Patrol() = 0;
	virtual void Chase_Target() = 0;
	//스프라이트 시트 체인지
	virtual void Scene_Change() = 0;

protected:
	virtual void OnDead() = 0;
	virtual void OnTakeDamage() = 0;

public:
	//vDir 방향으로 1초에 _fForce만큼이동하는 속력으로 _fTime 초만큼 이동한다.
	void Add_Force(Vector2 _vDir, float _fForce, float _fTime);
	
protected:
	//인식범위에 플레이어가 들어왔는가?
	bool IsAlert();

public:
	virtual void Take_Damage(float _fDamage) 
	{
		m_tStat.m_fHp -= _fDamage; 
		if (m_tStat.m_fHp < 0)
		{
			m_tStat.m_fHp = 0;
			OnDead();
		}
		OnTakeDamage();
	}

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

	FRONT m_eFront;
	FRONT m_ePrvFront;

	//죽음 ~ 오브젝트 소멸까지 대기시간
	float m_fDeadWait;
	DWORD m_dwDeadTimer;

	//중력
	Vector2			m_Gravity;
	float			m_fDeltaTime;

	//밀려나는 벡터
	Vector2			m_velocity;
	//넉백 타이머
	DWORD			m_dwForceTimer;
	//힘을 주는 시간
	float			m_fForceTime;


	
};


#endif // !__MONSTER_H__
