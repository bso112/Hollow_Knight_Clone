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

}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();
}

//���콺 Ŭ������ Ÿ���� �׸���.
void CTileMgr::Picking_Tile(POINT& _pt)
{
	//���콺����Ʈ�� �޾� Ÿ���� �ε����� ��ȯ�Ѵ�.
	int x = _pt.x / TILECX;
	int y = _pt.y / TILECY;

	//���� ���õ� Ÿ���� �ε����̴�. (2���� �迭�� �ƴ� vector�� �Ϸķ� ���������Ƿ� �̷��� ����Ѵ�)
	int iIndex = x + y * TILEX;

	//��ȿ�� �˻�
	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_isColider();
	CObjMgr::Get_Instance()->Add_Object(OBJID::TILE, m_vecTile[iIndex]);

}

bool CTileMgr::IsStepOnTile(CObj * _pObj, float& _fY)
{
	float fX;
	float fY;
	for (auto& tile : m_vecTile)
	{
		//�ݶ��̴� Ÿ���̸�
		if (((CTile*)tile)->Get_isColider())
		{
			//�浹�߰�
			if (CCollisionMgr::Check_Rect(_pObj, tile, &fX, &fY))
				//�����浹�̰�
				if (fX > fY)
					//���� Ÿ���� �Ʒ��� ������ ��� �ִ°��̴�.
					if (tile->Get_INFO().fY > _pObj->Get_INFO().fY)
					{
						_fY = tile->Get_Rect().top - (_pObj->Get_INFO().iCY >> 1);
						return true;
					}
		}
		
	}
	return false;
}

//�׸� Ÿ���� �����Ѵ�.
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

		//Ÿ���� ��ġ, ũ�⸦ �����Ѵ�.
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
			dynamic_cast<CTile*>(pObj)->Set_isColider();
			CObjMgr::Get_Instance()->Add_Object(OBJID::TILE, pObj);

		}
		m_vecTile.emplace_back(pObj);


	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Load", L"Success", MB_OK);
}
