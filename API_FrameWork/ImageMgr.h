#pragma once
#ifndef __IMAGEMGR_H__
#define __IMAGEMGR_H__

class CImageMgr
{
public:
	CImageMgr();
	~CImageMgr();

public:
	//타일을 그린다
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();
	void Add_Image(CObj* _pImage) { m_vecImage.push_back(_pImage); }
	int Get_VecImageSize() { return m_ivecImageSize; }

public:
	//원본 이미지 벡터의 이미지를 깊은 복사한 이미지를 반환한다.
	CObj* Get_Image(int _iIndex);
	const TCHAR* Get_ImageName(int _iIndex);

public:
	//이미지를 드래그한다.
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
	//전체 이미지 벡터(원본 + 인스턴스)
	vector<CObj*>		m_vecImage;
	//원본 이미지 사이즈
	int					m_ivecImageSize;

};


#endif // !__IMAGEMGR_H__


