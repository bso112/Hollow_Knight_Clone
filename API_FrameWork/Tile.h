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
	//이 예제에서 타일은 스프라이트 시트 하나 안에 두종류이므로 Set_DrawID로 그릴 타일의 종류를 구분한다.
	void Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID; }
	void Set_isColider() { m_bColider = !m_bColider; }
	bool& Get_isColider() { return m_bColider; }
public:
	int Get_DrawID() const { return m_iDrawID; }

private:
	int		m_iDrawID;
	//콜라이더 타일인가?
	bool	m_bColider;
};

#endif // !__TILE_H__

