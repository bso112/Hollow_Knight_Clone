#pragma once

#ifndef __TILEMGR_H__
#define __TILEMGR_H__


class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	enum COLLISION { LEFT, RIGHT, TOP, BOTTOM, END};
public:
	//Ÿ���� �׸���
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	//���콺�� Ŭ��������, �ش� Ÿ���� �׸���.
	void Picking_Tile(POINT& _pt);
	void Delete_Tile(POINT& _pt);
	////Ÿ���� ��� �ִ°�?
	//bool IsStepOnTile(CObj* _pObj, float& _fY);
	////�ؿ� Ÿ���� �ִ°�?
	//bool RayCast_Down_Tile(CObj* _pObj, float & _fY, float _maxdist);
	////Ÿ�ϰ� �浹ó��. �浹�� Ÿ���� ��Ʈ������ �Ѱ��ش�.
	//COLLISION Collision_Tile(CObj* _pObj, RECT& _rc);
	COLLISION Collision_Tile(Vector2 _origin, Vector2 _dst, INFO _info, RECT& _rc);

	bool Collision_Ex(CObj* _pObj, CTileMgr::COLLISION& _collision);

public:
	//�׸� Ÿ���� �����Ѵ�.
	void Save_Tile();
	//�׸� Ÿ���� �ε��Ѵ�.
	void Load_Tile();

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CTileMgr*	m_pInstance;
	//Ÿ���� ����
	vector<CObj*>		m_vecTile;

	vector<CObj*>		m_vecCollTile;
};


#endif // !__TILEMGR_H__
