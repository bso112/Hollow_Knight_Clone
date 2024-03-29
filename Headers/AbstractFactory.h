#pragma once

#ifndef __ABSTRACTFACTORY_H__
#define __ABSTRACTFACTORY_H__

class CObj;
class CMyImage;
template <typename T>
class CAbstractFactory
{
public:
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}
	static CObj* Create(float _x, float _y)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Initialize();
		return pObj;
	}
	static CObj* Create(float _x, float _y, float _fAngle)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_x, _y);
		pObj->Set_Angle(_fAngle);
		return pObj;
	}

	static CObj* Create(float _x, float _y, TCHAR* _pFrameKey, int _iCX, int _iCY)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Set_FrameKey(_pFrameKey);
		pObj->Set_Size(_iCX, _iCY);	
		pObj->Initialize();
		return pObj;
	}


	static CObj* Create(float _x, float _y, TCHAR* _pFrameKey, FRAME _frame, int _iCX, int _iCY)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Set_FrameKey(_pFrameKey);
		pObj->Set_Size(_iCX, _iCY);
		pObj->Set_Frame(_frame);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(float _x, float _y, TCHAR* _pFrameKey, FRAME _frame ,int _iCX, int _iCY, float _fDuration)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Set_FrameKey(_pFrameKey);
		pObj->Set_Size(_iCX, _iCY);
		pObj->Set_Frame(_frame);
		pObj->Initialize();
		dynamic_cast<CMyImage*>(pObj)->Set_Duration(_fDuration);
		return pObj;
	}


	static CObj* Create(INFO _tInfo, INFO _tImgInfo, TCHAR* _pFrameKey, FRAME _frame)
	{
		CObj* pObj = new T;
		pObj->Set_Info(_tInfo);
		pObj->Set_ImgInfo(_tImgInfo);
		pObj->Set_FrameKey(_pFrameKey);
		pObj->Set_Frame(_frame);
		pObj->Initialize();
		return pObj;
	}



};

#endif // !__ABSTRACTFACTORY_H__
