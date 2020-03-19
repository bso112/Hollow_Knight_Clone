#pragma once

#ifndef __MYBUTTON_H__
#define __MYBUTTON_H__


#include "Obj.h"
class CMyButton : public CObj
{
public:
	CMyButton();
	virtual ~CMyButton();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	void Set_FrameKey(TCHAR* _pFrameKey) { m_pFrameKey = _pFrameKey; }
	void Set_Text(TCHAR* _pText) { m_pText = _pText; }

private:
	TCHAR*		m_pFrameKey;
	TCHAR*		m_pText;
private:
	//버튼의 이미지는 스프라이트 이미지이므로 어떤 영역을 그릴지 선택해야한다. 그것을 위한 변수다.
	int			m_iDrawID;


};


#endif // !__MYBUTTON_H__

