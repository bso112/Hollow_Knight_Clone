#pragma once

#ifndef __MAINGAME_H__
#define __MAINGAME_H__

class CObj;
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();		// �ʱ�ȭ
	void Update();			// �� �����Ӹ��� ���� ����
	void Late_Update();		// ���°� ���ŵ� �� ��ü�� �ɼ�? ���� ���� üũ
	void Render();			// �� ���ο����� ���
	void Release();			// �޸� ���� �Լ�

private:
	HDC			m_DC;		// DC�� �����ϱ� ���� ����

	CObj*		m_pPlayer;
	list<CObj*>	m_listBullet;

	list<CObj*>	m_listObj[OBJID::END];

	DWORD		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[16];

	CObj*		m_pCursor;
};


#endif // !__MAINGAME_H__
