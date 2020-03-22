#pragma once
#ifndef	__MYIMAGE_H__
#define __MYIMAGE_H__

#include "Obj.h"
class CMyImage : public CObj
{
public:
	CMyImage();
	~CMyImage();
	
	CMyImage(CMyImage& _obj);

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;


public:
	void Set_Tag(SAVEDATA::TAG _eTag) { m_eTag = _eTag; }
	void Set_Frame(FRAME _frame) { m_tFrame = _frame; }
	const SAVEDATA::TAG& Get_Tag() { return m_eTag; }
private:
	SAVEDATA::TAG m_eTag;


};

#endif

