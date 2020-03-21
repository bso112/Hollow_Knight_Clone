#pragma once

#ifndef __OBJ_H__
#define __OBJ_H__


class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void Late_Update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;

protected:
	void Update_Rect();

public:
	void Set_Pos(float _x, float _y);
	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Dead() { m_bDead = true; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void Set_PosX(float _x) { m_tInfo.fX += _x; }
	void Set_PosY(float _y) { m_tInfo.fY += _y; }
	void Set_FrameKey(TCHAR* _key) { memcpy(m_pFrameKey, _key, sizeof(TCHAR) * DIR_LEN); }
	void Set_Size(int _iCX, int _iCY) { m_tInfo.iCX = _iCX; m_tInfo.iCY = _iCY; }
public:
	void Move_Frame();
	

public:
	const RECT& Get_Rect() const { return m_tRect; }
	const INFO& Get_INFO() const { return m_tInfo; }
	const bool& Get_Dead() const { return m_bDead; }
	const TCHAR* Get_pFrameKey() const { return m_pFrameKey; }
	const OBJTAG::TAG Get_Tag() const { return m_eTag; }

public:
	virtual void OnCollisionEnter(CObj* _pOther);
	//충돌영역을 전달하는 콜리전엔터
	virtual void OnCollisionEnter(CObj* _pOther, float _fX, float _fY);
protected:
	INFO	m_tInfo;
	RECT	m_tRect;

	float	m_fSpeed;
	bool	m_bDead;

	float	m_fAngle;

	CObj*	m_pTarget;

	FRAME	m_tFrame;
	TCHAR	m_pFrameKey[DIR_LEN];

	OBJTAG::TAG m_eTag;


};


#endif // !__OBJ_H__
