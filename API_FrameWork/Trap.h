#pragma once
#ifndef __TRAP_H__
#define __TRAP_H__


#include "Obj.h"
class CTrap : public CObj
{
public:
	CTrap();
	~CTrap();

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	virtual void OnCollisionEnter(CObj* _pOther, float _fX, float _fY) override;


};

#endif // !__TRAP_H__

