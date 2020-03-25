#include "stdafx.h"
#include "MyTime.h"

CMyTime* CMyTime::m_pInstance = nullptr;

CMyTime::CMyTime()
{
}


CMyTime::~CMyTime()
{
}

void CMyTime::Initalize()
{
	m_dwPrvTime = 0.f;
	m_dwCurrTime = GetTickCount();
}

void CMyTime::Update()
{
	m_dwPrvTime = m_dwCurrTime;
	m_dwCurrTime = GetTickCount();
	m_dwDeltaTime = m_dwCurrTime = m_dwPrvTime;
}
