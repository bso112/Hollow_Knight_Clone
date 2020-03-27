#pragma once
#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Obj.h"
class CMyImage;

//���� ����ȿ��, ���� ����
class CWeapon : public CObj
{
public:
	CWeapon();
	~CWeapon();

public:
	enum OWNER { PLAYER, MONSTER, END };

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	virtual void OnCollisionEnter(CObj* _pOther, float _fX, float _fY) override;

public:
	void Set_Owner(OWNER _owner) { m_eOwner = _owner; }
	void Swing(float fAngle, float fSpeed);
	void Set_Effect(CMyImage* _pEffect) { m_pEffect = _pEffect; }
	void Set_Duration(float _fDuration) { m_fDuration = _fDuration; }
	void Shoot();


private:
	OWNER m_eOwner;
	float m_fDamage;
	//������ ��ġ
	Vector2 m_vOrigin;
	//�����̿� ���� ����� ��ġ
	Vector2 m_vPosition;

	//���Ⱑ ����ִ� �ð�
	float m_fDuration;
	DWORD m_dwTimer;

	CMyImage* m_pEffect;

};

#endif // !__WEAPON_H__

