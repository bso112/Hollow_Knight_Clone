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
	//����
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
	//��������Ʈ ��Ʈ ü����
	virtual void Scene_Change() = 0;

protected:
	virtual void OnDead() = 0;
	virtual void OnTakeDamage() = 0;

public:
	//vDir �������� 1�ʿ� _fForce��ŭ�̵��ϴ� �ӷ����� _fTime �ʸ�ŭ �̵��Ѵ�.
	void Add_Force(Vector2 _vDir, float _fForce, float _fTime);
	
protected:
	//�νĹ����� �÷��̾ ���Դ°�?
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
	//�νĹ���
	float m_fRadius;
	//��������
	float m_fPatrol;
	//���� ������
	POINT m_PartolSpot;
	//���ϴ� ����(x��) ..��������Ʈ ��Ʈ ������ ����
	float m_fDir;

	FRONT m_eFront;
	FRONT m_ePrvFront;

	//���� ~ ������Ʈ �Ҹ���� ���ð�
	float m_fDeadWait;
	DWORD m_dwDeadTimer;

	//�߷�
	Vector2			m_Gravity;
	float			m_fDeltaTime;

	//�з����� ����
	Vector2			m_velocity;
	//�˹� Ÿ�̸�
	DWORD			m_dwForceTimer;
	//���� �ִ� �ð�
	float			m_fForceTime;


	
};


#endif // !__MONSTER_H__
