#include "stdafx.h"
#include "TileMgr.h"
#include "Tile.h"

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
	for (auto& pTile : m_vecTile)
		pTile->Render(_DC);
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();
}

//마우스 클릭으로 타일을 그린다.
void CTileMgr::Picking_Tile(POINT& _pt, int _iDrawID)
{
	//마우스포인트를 받아 타일의 인덱스로 변환한다.
	int x = _pt.x / TILECX;
	int y = _pt.y / TILECY;

	//현재 선택된 타일의 인덱스이다. (2차원 배열이 아닌 vector에 일렬로 저장했으므로 이렇게 계산한다)
	int iIndex = x + y * TILEX;

	//유효성 검사
	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	//해당 타일을 어떤 모양으로 그릴 것인지 정해준다. 
	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawID(_iDrawID);
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
		iDrawID = dynamic_cast<CTile*>(pTile)->Get_DrawID();

		//타일의 위치, 크기를 저장한다.
		WriteFile(hFile, &pTile->Get_INFO(), sizeof(INFO), &dwByte, NULL);
		//어떤 종류의 타일인지 저장한다.
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
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

	Release();

	DWORD dwByte = 0;
	INFO tTemp = {};
	int iDrawID = 0;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);

		if (0 == dwByte)
			break;

		//타일을 생성하고 위치를 셋팅해준다.
		CObj* pObj = CAbstractFactory<CTile>::Create(tTemp.fX, tTemp.fY);
		//어떤 타일인지 셋팅해준다.
		dynamic_cast<CTile*>(pObj)->Set_DrawID(iDrawID);

		m_vecTile.emplace_back(pObj);
	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Load", L"Success", MB_OK);
}
