#pragma once

#ifndef __OBJMGR_H__
#define __OBJMGR_H__

class CObj;
class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();


public:
	void Add_Object(OBJID::ID _eID, CObj* _pObj);
	int Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	CObj* Get_Player() 
	{ 
		if (m_listObj[OBJID::PLAYER].empty())
			return nullptr; 
		else 
			return m_listObj[OBJID::PLAYER].front(); 
	}

	CObj* Get_Target(CObj* _Dst, OBJID::ID _eID);


	// 내부에서 생성하자! -> new
	// 생성되어 있다면 new를 하지 않는다! -> if로 비교하겠다.
	// 객체를 생성하지 않아도 함수를 호출할 수 있어야 한다! -> static

public:	// 싱글톤
	static CObjMgr* Get_Instance()
	{
		if(!m_pInstance)
			m_pInstance = new CObjMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}
	void Delete_ID(OBJID::ID _eID);

private:
	list<CObj*>			m_listObj[OBJID::END];
	static CObjMgr*		m_pInstance;
};


#endif // !__OBJMGR_H__
