#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "Obj.h"
class CPlayer :	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	enum STATE { IDLE, WALK, ATTACK, HIT, JUMP, DEAD, END };


public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	void Set_Bullet(list<CObj*>* _pBullet) { m_pBullet = _pBullet; }
	void Set_Shield(list<CObj*>* _pShield) { m_pShield = _pShield; }

public:
	virtual void OnCollisionEnter(CObj* _pOther, float _fX, float _fY);




private:
	void Key_Check();
	void Scene_Change();
	void Jumping();
	void OffSet();

private:
	template <typename T>
	CObj* Create_Bullet()
	{
		CObj* pObj = CAbstractFactory<T>::Create((float)m_tPosin.x, (float)m_tPosin.y, m_fAngle);
		return pObj;
	}

private:
	list<CObj*>*	m_pBullet;
	list<CObj*>*	m_pShield;

	POINT			m_tPosin;
	float			m_fDis;

	// มกวม
	bool			m_bJump;
	float			m_fJumpPower;
	float			m_fJumpAccel;
	

	STATE			m_eCurState;
	STATE			m_ePrvState;

};


#endif // !__PLAYER_H__
