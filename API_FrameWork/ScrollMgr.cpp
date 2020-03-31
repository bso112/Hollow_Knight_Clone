#include "stdafx.h"
#include "ScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f), m_dwShakeTimer(0), m_fShakeTime(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Update()
{
	if (m_dwShakeTimer + m_fShakeTime * 1000 > GetTickCount())
	{
		//0.1�ʿ� �ѹ��� ������
		if (m_dwShakeTermTimer + 25 < GetTickCount())
		{
			//ī�޶� ���
			Set_Scroll_X(m_fShakeIntencity);
			m_fShakeIntencity = -m_fShakeIntencity;
			m_dwShakeTermTimer = GetTickCount();
		}
	}
	else
	{
		m_dwShakeTimer = 0;
		m_fShakeTime = 0.f;
		m_dwShakeTermTimer = 0;
	}

}


void CScrollMgr::Scroll_Lock()
{
	//ĳ���Ͱ� ���� (0,0) ��, ���� ���� �������� ��ġ�ϰ� �ִ�.
	//���� ĳ���Ͱ� �� ���¿��� �������� �̵��ϰų� ���� �̵������� ��ũ���ϸ� ���� �ٱ����� ���̰Եȴ�.
	//���� ���� �ɾ��ش�.

	//ĳ���Ͱ� �������� �̵��Ҷ� �� 
	if (0 < m_fScrollX)
		m_fScrollX = 0.f;
	//ĳ���Ͱ� ���� �̵��Ҷ� ��
	if (0 < m_fScrollY)
		m_fScrollY = 0.f;

	//ĳ���Ͱ� ���������� �̵��Ҷ� ��
	//WINCX - TILECX * TILEX �� ���� ��ũ�ѵ� �� �ִ� �ִ밪�̴�.
	//��, �ִ� ��ũ�Ѱ��� ������ �ذ��̴�.
	if (-8440 > m_fScrollX)
		m_fScrollX = -8440;
	//ĳ���Ͱ� �Ʒ��� �̵��Ҷ� ��
	if (-1970 > m_fScrollY)
		m_fScrollY = -1970;
}

void CScrollMgr::Shake_Camera(float _fIntencity, float _fTime)
{
	m_fShakeIntencity = _fIntencity;
	m_fShakeTime = _fTime;
	m_dwShakeTimer = GetTickCount();
	m_dwShakeTermTimer = GetTickCount();
}
