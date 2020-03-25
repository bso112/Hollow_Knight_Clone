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
	//������ ���� TILEX *  TILEY��ŭ Ÿ���� �׸���. 
	//y��, �� ���� ���� ��� ������ ������ +1�ȴ�.
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			//Ÿ���� �ε����� ���� Ÿ���� ��ǥ�� ���Ѵ�.
			float fX = (TILECX >> 1) + (float)(TILECX * j);
			float fY = (TILECY >> 1) + (float)(TILECY * i);

			//�� ��ǥ�� Ÿ���� �����.
			CObj*	pObj = CAbstractFactory<CTile>::Create(fX, fY);
			//�� Ÿ���� �����Ѵ�.
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
	for (auto& tile : m_vecCollTile)
	{
		tile->Render(_DC);
	}

}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();
	m_vecCollTile.clear();
}

void CTileMgr::Picking_Tile(POINT & _pt)
{
	//���콺����Ʈ�� �޾� Ÿ���� �ε����� ��ȯ�Ѵ�.
	int x = _pt.x / TILECX;
	int y = _pt.y / TILECY;

	//���� ���õ� Ÿ���� �ε����̴�. (2���� �迭�� �ƴ� vector�� �Ϸķ� ���������Ƿ� �̷��� ����Ѵ�)
	int iIndex = x + y * TILEX;

	//��ȿ�� �˻�
	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_isColider(true);
	m_vecCollTile.push_back(m_vecTile[iIndex]);

}

//Ÿ���� �ݶ��̴��� �ƴѰ����� �����.
void CTileMgr::Delete_Tile(POINT& _pt)
{
	//���콺����Ʈ�� �޾� Ÿ���� �ε����� ��ȯ�Ѵ�.
	int x = _pt.x / TILECX;
	int y = _pt.y / TILECY;

	//���� ���õ� Ÿ���� �ε����̴�. (2���� �迭�� �ƴ� vector�� �Ϸķ� ���������Ƿ� �̷��� ����Ѵ�)
	int iIndex = x + y * TILEX;

	//��ȿ�� �˻�
	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_isColider(false);
	//������Ʈ �Ŵ������� ���������ٵ�?

}

////Ÿ���� ��� �ִ°�?
//bool CTileMgr::IsStepOnTile(CObj * _pObj, float& _fY)
//{
//	float fX;
//	float fY;
//	for (auto& tile : m_vecCollTile)
//	{
//		//�ݶ��̴� Ÿ���̸�
//		if (((CTile*)tile)->Get_isColider())
//		{
//			//�浹�߰�
//			if (CCollisionMgr::Check_Rect(_pObj, tile, &fX, &fY))
//				//�����浹�̰�
//				if (fX > fY)
//					//���� Ÿ���� �Ʒ��� ������ ��� �ִ°��̴�.
//					if (tile->Get_Rect().top < _pObj->Get_Rect().bottom)
//					{
//						_fY = tile->Get_Rect().top - (_pObj->Get_INFO().iCY >> 1);
//						return true;
//					}
//		}
//
//	}
//	return false;
//}
//
////�Ʒ��� Ÿ���� �ִ°�?
//bool CTileMgr::RayCast_Down_Tile(CObj* _pObj, float & _fY, float _maxdist)
//{
//	//obj�� ��Ʈ�� �����ͼ� �� ��Ʈ�� bootom�� maxdist��ŭ �ø� ��Ʈ(����)�� �����.
//	RECT tmp = _pObj->Get_Rect();
//	RECT rc = { tmp.left, tmp.top, tmp.right, tmp.bottom + _maxdist };
//	RECT* pRay = &rc;
//
//	RECT tmp2 = {};
//	float minDist = FLT_MAX;
//	CObj* minTile = nullptr;
//	for (auto& tile : m_vecCollTile)
//	{
//		//������ �������� �Ʒ��� �ִ� Ÿ�ϸ� �˻�
//		if (tile->Get_Rect().top > pRay->top)
//		{
//			//��Ʈ�� �浹�� Ÿ�� ��
//			if (IntersectRect(&tmp2, pRay, &tile->Get_Rect()))
//			{
//				//��Ʈ�� ���� �Ÿ��� ����� Ÿ���� ���Ѵ�.
//				float dist = (float)abs(tile->Get_Rect().top - pRay->top);
//				if (minDist > dist)
//				{
//					minDist = dist;
//					minTile = tile;
//				}
//			}
//		}
//
//	}
//	//����ó��
//	if (minTile == nullptr)
//		return false;
//	//����. mintile�� top�� �Ѱ��ش�.
//	_fY = minTile->Get_Rect().top;
//	return true;
//}
//
//CTileMgr::COLLISION CTileMgr::Collision_Tile(CObj * _pObj, RECT& _rc)
//{
//	float fX;
//	float fY;
//	for (auto& tile : m_vecCollTile)
//	{
//
//		//�浹�ϸ�
//		if (CCollisionMgr::Check_Rect(_pObj, tile, &fX, &fY))
//		{
//			//���� ���� �浹�� Ÿ���� ��Ʈ�� �Ѱ��ش�.
//			_rc = tile->Get_Rect();
//
//			//�����浹�̸�
//			if (fX > fY)
//			{
//				//���� Ÿ���� �Ʒ��� ������ ��� �ִ°��̴�.
//				if (tile->Get_Rect().top <= _pObj->Get_Rect().bottom)
//				{
//					return COLLISION::BOTTOM;
//				}
//				else
//				{
//					return COLLISION::TOP;
//				}
//			}
//			//�¿��浹�̸�
//			else
//			{
//				//Ÿ���� ���ʿ� ������
//				if (tile->Get_Rect().right <= _pObj->Get_Rect().left)
//					return COLLISION::LEFT;
//				//�����ʿ� ������
//				else
//					return COLLISION::RIGHT;
//			}
//		}
//
//
//
//	}
//	return COLLISION::END;
//}

CTileMgr::COLLISION CTileMgr::Collision_Tile(Vector2 _origin, Vector2 _dst, INFO _info, RECT& _rc)
{
	//���� ������ �ӵ�(��Ȯ��)
	float fSpeed = 2.f;

	//dst�� ���� ���ⱸ�ϱ�
	Vector2 dir = _dst - _origin;
	//�Ÿ��� �����ų� �ʹ� �ָ� ����
	if (dir.magnitude() < 1 || dir.magnitude() > 500)
		return COLLISION::END;
	dir = dir.Nomalize();


	//���� ���������� ����
	INFO info = _info;
	info.fX = _origin.fX;
	info.fY = _origin.fY;

	//�浹�� �� ��Ʈ
	RECT rc = {};
	rc.left = LONG(info.fX - (info.iCX >> 1));
	rc.top = LONG(info.fY - (info.iCY >> 1));
	rc.right = LONG(info.fX + (info.iCX >> 1));
	rc.bottom = LONG(info.fY + (info.iCY >> 1));


	Vector2 vec = _dst - Vector2(info.fX, info.fY);


	//�������� �����Ҷ�����
	while (vec.magnitude() > 1)
	{
		vec = _dst - Vector2(info.fX, info.fY);

		//Ȥ���� �Ÿ��� �ʹ� �־����� ����
		if (vec.magnitude() > 500)
			return COLLISION::END;

		//��Ʈ�� �̵���Ŵ
		info.fX += dir.fX * fSpeed;
		info.fY += dir.fY * fSpeed;

		//��Ʈ ������Ʈ
		rc.left = LONG(info.fX - (info.iCX >> 1));
		rc.top = LONG(info.fY - (info.iCY >> 1));
		rc.right = LONG(info.fX + (info.iCX >> 1));
		rc.bottom = LONG(info.fY + (info.iCY >> 1));

		RECT collision;

		float minDist = FLT_MAX;
		CObj* minTile = nullptr;
		//���� �Ÿ��� ����� Ÿ���� ���Ѵ�.
		for (auto& tile : m_vecCollTile)
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

		//���� �浹�̸�
		if (IntersectRect(&collision, &rc, &minTile->Get_Rect()))
		{
			_rc = minTile->Get_Rect();

			LONG iCX = collision.right - collision.left;
			LONG iCY = collision.bottom - collision.top;

			//�����浹�̸�
			if (iCX > iCY)
			{
				//���� Ÿ���� �Ʒ��� ������ ��� �ִ°��̴�.
				if (minTile->Get_Rect().top <= rc.bottom)
				{
					return COLLISION::BOTTOM;
				}
				else
				{
					return COLLISION::TOP;
				}
			}
			//�¿��浹�̸�
			else
			{
				//Ÿ���� ���ʿ� ������
				if (minTile->Get_Rect().right <= rc.left)
					return COLLISION::LEFT;
				//�����ʿ� ������
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
	����: Ÿ���� ũ�Ⱑ �۴ٺ��� �ѹ��� �������� Ÿ�ϰ� �浹�Ѵ�.
	�ذ�: �Ÿ��� ���尡��� �ϳ��� Ÿ�ϰ��� �浹����.
	
	*/
	//���� �Ÿ��� ����� Ÿ���� ���Ѵ�.
	for (auto& tile : m_vecCollTile)
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

//�׸� Ÿ���� �����Ѵ�.
void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile3.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;
	int iDrawID = 0;

	for (auto& pTile : m_vecTile)
	{
		CTile* tile = dynamic_cast<CTile*>(pTile);

		//Ÿ���� ��ġ, ũ�⸦ �����Ѵ�.
		WriteFile(hFile, &pTile->Get_INFO(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &dynamic_cast<CTile*>(pTile)->Get_isColider(), sizeof(bool), &dwByte, NULL);


	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Save", L"Success", MB_OK);
}

void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile3.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	//���� Ÿ���� ��� �����.
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

		//Ÿ���� �����ϰ� ��ġ�� �������ش�.
		CObj* pObj = CAbstractFactory<CTile>::Create(tTemp.fX, tTemp.fY);
		//�ݶ��̴� Ÿ���̸�
		if (bColider)
		{
			//�ݶ��̴��� �������ش�.
			dynamic_cast<CTile*>(pObj)->Reverse_isCollider();
			m_vecCollTile.push_back(pObj);

		}
		m_vecTile.emplace_back(pObj);


	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Load", L"Success", MB_OK);
}
