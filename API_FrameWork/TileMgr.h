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
	//타일을 그린다
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	//마우스로 클릭했을때, 해당 타일을 그린다.
	void Picking_Tile(POINT& _pt);
	void Delete_Tile(POINT& _pt);
	////타일을 밟고 있는가?
	//bool IsStepOnTile(CObj* _pObj, float& _fY);
	////밑에 타일이 있는가?
	//bool RayCast_Down_Tile(CObj* _pObj, float & _fY, float _maxdist);
	////타일과 충돌처리. 충돌한 타일의 렉트정보를 넘겨준다.
	//COLLISION Collision_Tile(CObj* _pObj, RECT& _rc);
	COLLISION Collision_Tile(Vector2 _origin, Vector2 _dst, INFO _info, RECT& _rc);

	bool Collision_Ex(CObj* _pObj, CTileMgr::COLLISION& _collision);

public:
	//그린 타일을 저장한다.
	void Save_Tile();
	//그린 타일을 로드한다.
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
	//타일의 벡터
	vector<CObj*>		m_vecTile;

	vector<CObj*>		m_vecCollTile;
};


#endif // !__TILEMGR_H__
