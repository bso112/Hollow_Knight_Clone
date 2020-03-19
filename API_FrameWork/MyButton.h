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
	//��ư�� �̹����� ��������Ʈ �̹����̹Ƿ� � ������ �׸��� �����ؾ��Ѵ�. �װ��� ���� ������.
	int			m_iDrawID;


};


#endif // !__MYBUTTON_H__

