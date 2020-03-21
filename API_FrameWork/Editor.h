#pragma once

#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Scene.h"

class CMyImage;

class CEditor : public CScene
{
public:
	CEditor();
	virtual ~CEditor();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	void Key_Check();
	
private:
	//�̹��� ��ŷ�� ���� �̹��� ���� �ε���
	int m_iSelected;

};


#endif // !__EDITOR_H__