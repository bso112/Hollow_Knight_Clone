#pragma once
#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Obj.h"
class CMyImage;

//데미지를 줄 수 있는 무기, 효과
class CWeapon : public CObj
{
public:
	CWeapon();
	~CWeapon();

public:
	enum OWNER { PLAYER, MONSTER, NONE, END };

	// CObj을(를) 통해 상속됨
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
	//vDir 방향으로 1초에 _fForce만큼이동하는 속력으로 _fTime 초만큼 이동한다.
	void Add_Force(Vector2 _vDir, float _fForce, float _fTime);
	void Set_Horizontal() { m_bHorizontal = true; }

private:
	OWNER m_eOwner;
	float m_fDamage;
	//손잡이 위치
	Vector2 m_vOrigin;
	//손잡이에 대한 상대적 위치
	Vector2 m_vPosition;

	//이펙트가 살아있는 시간
	float m_fDuration;
	DWORD m_dwTimer;

	Vector2 m_velocity;
	//힘 타이머
	DWORD			m_dwForceTimer;
	//힘을 주는 시간
	float			m_fForceTime;

	//가로 스프라이트인가?
	bool m_bHorizontal;
};

#endif // !__WEAPON_H__

