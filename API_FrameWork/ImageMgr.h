#pragma once
#ifndef __IMAGEMGR_H__
#define __IMAGEMGR_H__

class CImageMgr
{
public:
	CImageMgr();
	~CImageMgr();

public:
	//Ÿ���� �׸���
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();
	void Add_Image(CObj* _pImage) { m_vecImage.push_back(_pImage); }

public:
	//���콺�� Ŭ��������, �ش� �̹����� �׸���.
	void Picking_Image(POINT& _pt);
	//�̹����� �巡���Ѵ�.
	void Drag_Image(POINT& _pt);

public:
	void Save_Image();
	void Load_Image();


public:
	static CImageMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CImageMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CImageMgr*	m_pInstance;
	//Ÿ���� ����
	vector<CObj*>		m_vecImage;
};


#endif // !__IMAGEMGR_H__


