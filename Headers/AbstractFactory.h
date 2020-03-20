#pragma once

#ifndef __ABSTRACTFACTORY_H__
#define __ABSTRACTFACTORY_H__

class CObj;
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



};

#endif // !__ABSTRACTFACTORY_H__
