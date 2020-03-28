#pragma once
#ifndef	__MYIMAGE_H__
#define __MYIMAGE_H__

#include "Obj.h"
class CMyImage : public CObj
{
public:
	CMyImage();
	CMyImage(TCHAR* _pFrameKey) { memcpy(m_pFrameKey, _pFrameKey, sizeof(TCHAR) * DIR_LEN); }
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
	const SAVEDATA::TAG& Get_Tag() { return m_eTag; }
	void Set_Duration(float _fDuration) { m_fDuration = _fDuration; }
private:
	SAVEDATA::TAG m_eTag;
	//이미지가 살아있는 시간
	float m_fDuration;
	DWORD m_dwTimer;


};

#endif

