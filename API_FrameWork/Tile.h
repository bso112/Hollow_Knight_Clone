#pragma once

#ifndef __TILE_H__
#define __TILE_H__

#include "Obj.h"
class CTile : public CObj
{
public:
	CTile();
	virtual ~CTile();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	//�� �������� Ÿ���� ��������Ʈ ��Ʈ �ϳ� �ȿ� �������̹Ƿ� Set_DrawID�� �׸� Ÿ���� ������ �����Ѵ�.
	void Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID; }

public:
	int Get_DrawID() const { return m_iDrawID; }

private:
	int		m_iDrawID;
};

#endif // !__TILE_H__

