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


	// ���ο��� ��������! -> new
	// �����Ǿ� �ִٸ� new�� ���� �ʴ´�! -> if�� ���ϰڴ�.
	// ��ü�� �������� �ʾƵ� �Լ��� ȣ���� �� �־�� �Ѵ�! -> static

public:	// �̱���
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
