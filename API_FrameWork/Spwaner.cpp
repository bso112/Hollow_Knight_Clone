#include "stdafx.h"
#include "Spwaner.h"
#include "ObjMgr.h"

Spwaner* Spwaner::m_pInstance = nullptr;
Spwaner::Spwaner()
	:m_dwSpawnTimer(MAXDWORD)
{
}


Spwaner::~Spwaner()
{
}

void Spwaner::Update()
{
	if (m_dwSpawnTimer + m_fDelay * 1000 < GetTickCount())
	{
		CObjMgr::Get_Instance()->Add_Object(m_eID, m_pToSpwan);
	}
}

void Spwaner::Spwan(CObj * _obj, OBJID::ID _eID, float _delay)
{
	m_pToSpwan = _obj;
	m_eID = _eID;
	m_fDelay = _delay;
	m_dwSpawnTimer = GetTickCount();
}
