#pragma once

#ifndef __SCROLLMGR_H__
#define __SCROLLMGR_H__


class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	void Set_Scroll_X(float _x) { m_fScrollX += _x; }
	void Set_Scroll_Y(float _y) { m_fScrollY += _y; }

public:
	const float& Get_Scroll_X() const { return m_fScrollX; }
	const float& Get_Scroll_Y() const { return m_fScrollY; }
		
public:
	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CScrollMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CScrollMgr*	m_pInstance;
	float				m_fScrollX;
	float				m_fScrollY;
};


#endif // !__SCROLLMGR_H__
