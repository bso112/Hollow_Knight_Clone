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
	//원본 이미지 벡터의 이미지를 깊은 복사한 이미지를 반환한다.
	CObj* Get_Image(int _iIndex);
	const TCHAR* Get_ImageName(int _iIndex);
	//인스턴스를 뒤에서 하나 지운다.
	void Pop_back() {if(m_vecImage.size() > 0) m_vecImage.pop_back(); }
public:
	//이미지를 드래그한다.
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
	//에디터 배치용 이미지
	vector<CObj*>		m_vecImageForEditor;
	//실제 처리할 이미지
	vector<CObj*>		m_vecImage;

};


#endif // !__IMAGEMGR_H__


