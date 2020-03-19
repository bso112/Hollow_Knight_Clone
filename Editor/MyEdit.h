#pragma once

#ifndef __MYEDIT_H__
#define __MYEDIT_H__

class CLine;
class CMyEdit
{
private:
	CMyEdit();
	~CMyEdit();

public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Save_Line();
	void Load_Line();
	void Add_Line();

public:
	static CMyEdit* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CMyEdit;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CMyEdit*		m_pInstance;
	list<CLine*>		m_listLine;
	LINEPOS				m_tLinePos[2];
};


#endif // !__MYEDIT_H__
