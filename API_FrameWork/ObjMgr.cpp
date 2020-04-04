#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Monster.h"
#include "CollisionMgr.h"
#include "ScrollMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Add_Object(OBJID::ID _eID, CObj * _pObj)
{
	//중복 불허용
	for (auto& obj : m_listObj[_eID])
	{
		if (obj == _pObj)
			return;
	}
	m_listObj[_eID].emplace_back(_pObj);
}

int CObjMgr::Update()
{

	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::PLAYER], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::WEAPON], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::WEAPON], m_listObj[OBJID::PLAYER]);
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TRAP], m_listObj[OBJID::PLAYER]);
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TRAP], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TRAP], m_listObj[OBJID::WEAPON]);


	for (int i = 0; i < OBJID::END; ++i)
	{
		auto& iter = m_listObj[i].begin();
		for (; iter != m_listObj[i].end(); )
		{
			int iEvent = (*iter)->Update();
			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_listObj[i].erase(iter);

				if (m_listObj[i].empty())
					break;
			}
			else
			{
				if (OBJ_FREEZE == iEvent)
					return 0;

				++iter;
			}
		}
	}









	return 0;
}

void CObjMgr::Late_Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->Late_Update();
			if (m_listObj[i].empty())
				break;

		}
	}
}

void CObjMgr::Render(HDC _DC)
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('E'))
				Draw_Rect(_DC, pObj->Get_Rect(), CScrollMgr::Get_Instance()->Get_Scroll_X(), CScrollMgr::Get_Instance()->Get_Scroll_Y());
			pObj->Render(_DC);
		}
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>);
		m_listObj[i].clear();
	}
}

CObj* CObjMgr::Get_Target(CObj* _Dst, OBJID::ID _eID)
{
	if (m_listObj[_eID].empty())
		return nullptr;

	CObj* pTarget = nullptr;
	float fDis = 0.f;

	for (auto& Src : m_listObj[_eID])
	{
		if (Src->Get_Dead())
			continue;

		float fX = abs(_Dst->Get_INFO().fX - Src->Get_INFO().fX);
		float fY = abs(_Dst->Get_INFO().fY - Src->Get_INFO().fY);
		float fDis2 = sqrtf(fX * fX + fY * fY);

		if (!pTarget || fDis > fDis2)
		{
			pTarget = Src;
			fDis = fDis2;
		}
	}
	return pTarget;
}

void CObjMgr::Delete_ID(OBJID::ID _eID)
{
	for (auto& pObj : m_listObj[_eID])
		Safe_Delete(pObj);

	m_listObj[_eID].clear();
}
