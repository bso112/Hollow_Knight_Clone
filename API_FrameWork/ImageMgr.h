#pragma once
#ifndef __IMAGEMGR_H__
#define __IMAGEMGR_H__

class CImageMgr
{
public:
	CImageMgr();
	~CImageMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();
	void Add_Image(CObj* _pImage) { m_vecImage.push_back(_pImage); }
	void Add_EditImage(CObj* _pImage) { m_vecImageForEditor.push_back(_pImage); }
	int Get_VecImageSize() { return m_vecImageForEditor.size(); }


public:
	//���� �̹��� ������ �̹����� ���� ������ �̹����� ��ȯ�Ѵ�.
	CObj* Get_Image(int _iIndex);
	const TCHAR* Get_ImageName(int _iIndex);
	//�ν��Ͻ��� �ڿ��� �ϳ� �����.
	void Pop_back() {if(m_vecImage.size() > 0) m_vecImage.pop_back(); }
public:
	//�̹����� �巡���Ѵ�.
	void Drag_Image(POINT& _pt);
	void Remove_Image(POINT _pt);

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
	//������ ��ġ�� �̹���
	vector<CObj*>		m_vecImageForEditor;
	//���� ó���� �̹���
	vector<CObj*>		m_vecImage;

};


#endif // !__IMAGEMGR_H__


