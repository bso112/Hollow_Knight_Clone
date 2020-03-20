#include "stdafx.h"
#include "ScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
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
	if (-2900 > m_fScrollX)
		m_fScrollX = -2900;
	//ĳ���Ͱ� �Ʒ��� �̵��Ҷ� ��
	if (-1100 > m_fScrollY)
		m_fScrollY = -1100;
}