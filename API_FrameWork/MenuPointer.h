#pragma once

#ifndef __MENUPOINTER_H__
#define __MENUPOINTER_H__

#include "Obj.h"
class CMenuPointer : public CObj
{
public:
	CMenuPointer();
	~CMenuPointer();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // 

