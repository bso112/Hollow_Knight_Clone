#pragma once
#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Obj.h"
class CMyImage;

//�������� �� �� �ִ� ����, ȿ��
class CWeapon : public CObj
{
public:
	CWeapon();
	~CWeapon();

public:
	enum OWNER { PLAYER, MONSTER, NONE, END };

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
	void Set_Duration(float _fDuration) { m_fDuration = _fDuration; }
	void Shoot();
	//vDir �������� 1�ʿ� _fForce��ŭ�̵��ϴ� �ӷ����� _fTime �ʸ�ŭ �̵��Ѵ�.
	void Add_Force(Vector2 _vDir, float _fForce, float _fTime);
	void Set_Horizontal() { m_bHorizontal = true; }

private:
	OWNER m_eOwner;
	float m_fDamage;
	//������ ��ġ
	Vector2 m_vOrigin;
	//�����̿� ���� ����� ��ġ
	Vector2 m_vPosition;

	//����Ʈ�� ����ִ� �ð�
	float m_fDuration;
	DWORD m_dwTimer;

	Vector2 m_velocity;
	//�� Ÿ�̸�
	DWORD			m_dwForceTimer;
	//���� �ִ� �ð�
	float			m_fForceTime;

	//���� ��������Ʈ�ΰ�?
	bool m_bHorizontal;
};

#endif // !__WEAPON_H__

