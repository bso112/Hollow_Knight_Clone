#include "stdafx.h"
#include "TileMgr.h"
#include "Tile.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;
CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
}


CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	//디파인 잡은 TILEX *  TILEY만큼 타일을 그린다. 
	//y축, 즉 행은 열을 모두 생성한 다음에 +1된다.
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			//타일의 인덱스에 따른 타일의 좌표를 구한다.
			float fX = (TILECX >> 1) + (float)(TILECX * j);
			float fY = (TILECY >> 1) + (float)(TILECY * i);

			//그 좌표에 타일을 만든다.
			CObj*	pObj = CAbstractFactory<CTile>::Create(fX, fY);
			//그 타일을 저장한다.
			m_vecTile.emplace_back(pObj);
		}
	}

}

void CTileMgr::Update()
{

}
void CTileMgr::Late_Update()
{

}

void CTileMgr::Render(HDC _DC)
{
	for (auto& tile : m_setCollTile)
	{
		tile->Render(_DC);
	}

}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();
	m_setCollTile.clear();
}

void CTileMgr::Picking_Tile(POINT & _pt)
{
	//마우스포인트를 받아 타일의 인덱스로 변환한다.
	int x = _pt.x / TILECX;
	int y = _pt.y / TILECY;

	//현재 선택된 타일의 인덱스이다. (2차원 배열이 아닌 vector에 일렬로 저장했으므로 이렇게 계산한다)
	int iIndex = x + y * TILEX;

	//유효성 검사
	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_isColider(true);
	m_setCollTile.insert(m_vecTile[iIndex]);
	
}

//타일을 콜라이더가 아닌것으로 만든다.
void CTileMgr::UnPiking_Tile(POINT& _pt)
{
	//마우스포인트를 받아 타일의 인덱스로 변환한다.
	int x = _pt.x / TILECX;
	int y = _pt.y / TILECY;

	//현재 선택된 타일의 인덱스이다. (2차원 배열이 아닌 vector에 일렬로 저장했으므로 이렇게 계산한다)
	int iIndex = x + y * TILEX;

	//유효성 검사
	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_isColider(false);

}


CTileMgr::COLLISION CTileMgr::Collision_Tile(Vector2 _origin, Vector2 _dst, INFO _info, RECT& _rc)
{
	//점을 움직일 속도(정확도)
	float fSpeed = 2.f;

	//dst로 가는 방향구하기
	Vector2 dir = _dst - _origin;
	//거리가 가깝거나 너무 멀면 리턴
	if (dir.magnitude() < 1 || dir.magnitude() > 500)
		return COLLISION::END;
	dir = dir.Nomalize();


	//점을 시작점으로 셋팅
	INFO info = _info;
	info.fX = _origin.fX;
	info.fY = _origin.fY;

	//충돌에 쓸 렉트
	RECT rc = {};
	rc.left = LONG(info.fX - (info.iCX >> 1));
	rc.top = LONG(info.fY - (info.iCY >> 1));
	rc.right = LONG(info.fX + (info.iCX >> 1));
	rc.bottom = LONG(info.fY + (info.iCY >> 1));


	Vector2 vec = _dst - Vector2(info.fX, info.fY);


	//도착지에 도착할때까지
	while (vec.magnitude() > 1)
	{
		vec = _dst - Vector2(info.fX, info.fY);

		//혹여나 거리가 너무 멀어지면 리턴
		if (vec.magnitude() > 500)
			return COLLISION::END;

		//렉트를 이동시킴
		info.fX += dir.fX * fSpeed;
		info.fY += dir.fY * fSpeed;

		//렉트 업데이트
		rc.left = LONG(info.fX - (info.iCX >> 1));
		rc.top = LONG(info.fY - (info.iCY >> 1));
		rc.right = LONG(info.fX + (info.iCX >> 1));
		rc.bottom = LONG(info.fY + (info.iCY >> 1));

		RECT collision;

		float minDist = FLT_MAX;
		CObj* minTile = nullptr;
		//가장 거리가 가까운 타일을 구한다.
		for (auto& tile : m_setCollTile)
		{
			float distX = abs(tile->Get_INFO().fX - _dst.fX);
			float distY = abs(tile->Get_INFO().fY - _dst.fY);
			float dist = sqrtf(distX * distX + distY * distY);
			if (minDist > dist)
			{
				minDist = dist;
				minTile = tile;
			}
			
		}

		//만약 충돌이면
		if (IntersectRect(&collision, &rc, &minTile->Get_Rect()))
		{
			_rc = minTile->Get_Rect();

			LONG iCX = collision.right - collision.left;
			LONG iCY = collision.bottom - collision.top;

			//상하충돌이면
			if (iCX > iCY)
			{
				//만약 타일이 아래에 있으면 밟고 있는것이다.
				if (minTile->Get_Rect().top <= rc.bottom)
				{
					return COLLISION::BOTTOM;
				}
				else
				{
					return COLLISION::TOP;
				}
			}
			//좌우충돌이면
			else
			{
				//타일이 왼쪽에 있으면
				if (minTile->Get_Rect().right <= rc.left)
					return COLLISION::LEFT;
				//오른쪽에 있으면
				else
					return COLLISION::RIGHT;
			}
		}
	}


	return COLLISION::END;
}

bool CTileMgr::Collision_Ex(CObj * _pObj, CTileMgr::COLLISION& _collision)
{


	float minDist = FLT_MAX;
	CObj* minTile = nullptr;

	/*
	문제: 타일의 크기가 작다보니 한번에 여러개의 타일과 충돌한다.
	해결: 거리가 가장가까운 하나의 타일과만 충돌하자.
	
	*/
	//가장 거리가 가까운 타일을 구한다.
	for (auto& tile : m_setCollTile)
	{
		float distX = abs(tile->Get_INFO().fX - _pObj->Get_INFO().fX);
		float distY = abs(tile->Get_INFO().fY - _pObj->Get_INFO().fY);
		float dist = sqrtf(distX * distX + distY * distY);
		if (minDist > dist)
		{
			minDist = dist;
			minTile = tile;
		}

	}

	float fX = 0.f;
	float fY = 0.f;

	if (CCollisionMgr::Check_Rect(minTile, _pObj, &fX, &fY))
	{
		if (fX > fY)
		{
			if (minTile->Get_INFO().fY > _pObj->Get_INFO().fY)
			{
				_pObj->Set_PosY(-fY);
				_collision = COLLISION::BOTTOM;
			}
			else
			{
				_pObj->Set_PosY(fY);
				_collision = COLLISION::TOP;
			}
		}
		else
		{
			if (minTile->Get_INFO().fX > _pObj->Get_INFO().fX)
			{
				_pObj->Set_PosX(-fX);
				_collision = COLLISION::RIGHT;
			}
			else
			{
				_pObj->Set_PosX(fX);
				_collision = COLLISION::LEFT;
			}

		}
	}
	
	
	return false;
}

//그린 타일을 저장한다.
void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;
	int iDrawID = 0;

	for (auto& pTile : m_vecTile)
	{
		CTile* tile = dynamic_cast<CTile*>(pTile);

		//타일의 위치, 크기를 저장한다.
		WriteFile(hFile, &pTile->Get_INFO(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &dynamic_cast<CTile*>(pTile)->Get_isColider(), sizeof(bool), &dwByte, NULL);


	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Save", L"Success", MB_OK);
}

void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	//현재 타일을 모두 지운다.
	Release();

	DWORD dwByte = 0;
	INFO tTemp = {};
	bool bColider = false;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &bColider, sizeof(bool), &dwByte, NULL);
		if (0 == dwByte)
			break;

		//타일을 생성하고 위치를 셋팅해준다.
		CObj* pObj = CAbstractFactory<CTile>::Create(tTemp.fX, tTemp.fY);
		//콜라이더 타일이면
		if (bColider)
		{
			//콜라이더로 셋팅해준다.
			dynamic_cast<CTile*>(pObj)->Reverse_isCollider();
			m_setCollTile.insert(pObj);

		}
		m_vecTile.emplace_back(pObj);


	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Load", L"Success", MB_OK);
}
