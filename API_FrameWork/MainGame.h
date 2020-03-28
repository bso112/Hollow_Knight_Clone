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
	void Initialize();		// 초기화
	void Update();			// 매 프레임마다 상태 갱신
	void Late_Update();		// 상태가 갱신된 후 객체의 옵션? 변경 등을 체크
	void Render();			// 매 프로엠마다 출력
	void Release();			// 메모리 해제 함수

private:
	HDC			m_DC;		// DC를 보관하기 위한 변수

	CObj*		m_pPlayer;
	list<CObj*>	m_listBullet;

	list<CObj*>	m_listObj[OBJID::END];

	DWORD		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[16];

	CObj*		m_pCursor;
};


#endif // !__MAINGAME_H__
