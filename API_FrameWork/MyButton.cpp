#include "stdafx.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"


CMyButton::CMyButton()
	:m_pText(nullptr), m_pFrameKey(nullptr)
{
}


CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	m_tInfo.iCX = 150;
	m_tInfo.iCY = 45;

	m_iDrawID = 0;
}

int CMyButton::Update()
{
	Update_Rect();




	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{

	//��ư�� ����Ʈ�� �޾ƿ´�.
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	//�ش緺Ʈ�ȿ� ����Ʈ�� �ִ°�?
	if (PtInRect(&m_tRect, pt))
	{
		//���� ���� ���콺Ŭ���Ǹ�
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			//���� ������Ű�� Button_Start�̸�
			if (!lstrcmp(m_pFrameKey, L"Button_Start"))
			{
				//�������� ������ ��ȯ
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_STAGE);
				return;
			}
			//���� ������Ű�� Button_Edit�̸�
			else if (!lstrcmp(m_pFrameKey, L"Button_Edit"))
			{

			}
			//���� ������Ű�� Button_Exit�̸�
			else if (!lstrcmp(m_pFrameKey, L"Button_Exit"))
				//���� ������ ����
				DestroyWindow(g_hWnd);
		}
		m_iDrawID = 1;
	}
	else
		m_iDrawID = 0;
}

void CMyButton::Render(HDC _DC)
{
	Update_Rect();


	//������Ű�� ���� ��ư�� � �̹����� ������ �ִ��� �����ϴ� ������ ��.
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	if (hMemDC)
	{
		//�̹����� ��������Ʈ �̹����̱� ������ m_iDrawID�� �����Ͽ� �����.
		//���, Late_Update���� ��ư�� ���콺�� �÷����� m_iDrawID = 1;�� ���������Ƿ� ��ư�� �̹����� �ٲ�.
		GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY
			, RGB(0, 0, 0));

	}

	if(m_pTarget)
		//�ؽ�Ʈ ���
		DrawText(_DC, m_pText, lstrlen(m_pText), &m_tRect, DT_CENTER | DT_VCENTER);




}

void CMyButton::Release()
{
}
