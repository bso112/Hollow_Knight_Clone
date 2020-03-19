#include "stdafx.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "Bullet.h"
#include "Monster.h"
#include "Mouse.h"


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src)
{
	RECT rc = {};

	for(auto& Dst : _Dst)
	{
		for(auto& Src : _Src)
		{
			if(IntersectRect(&rc, &(Dst->Get_Rect()), &(Src->Get_Rect())))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src)
{
	for(auto& Dst : _Dst)
	{
		for(auto& Src : _Src)
		{
			float fX = 0.f, fY = 0.f;

			if(Check_Rect(Dst, Src, &fX, &fY))
			{
				if(fX > fY)	// 상하
				{
					if(Dst->Get_INFO().fY < Src->Get_INFO().fY)
						Src->Set_PosY(fY);
					else
						Src->Set_PosY(-fY);
				}
				else // 좌우
				{
					if(Dst->Get_INFO().fX < Src->Get_INFO().fX)
						Src->Set_PosX(fX);
					else
						Src->Set_PosX(-fX);
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere(list<CObj*> _Dst, list<CObj*> _Src)
{
	for(auto& Dst : _Dst)
	{
		for(auto& Src : _Src)
		{
			if(Check_Sphere(Dst, Src))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CObj * _Dst, CObj * _Src)
{
	// abs()
	// 절대값. 음수가 나오지 않는다.

	// sqrtf()
	// 제곱근을 구해주는 함수이다.

	float fX = abs(_Dst->Get_INFO().fX - _Src->Get_INFO().fX);
	float fY = abs(_Dst->Get_INFO().fY - _Src->Get_INFO().fY);
	float fDis = sqrtf(fX * fX + fY * fY);

	float fRad = float((_Dst->Get_INFO().iCX + _Src->Get_INFO().iCX) >> 1);

	return fDis < fRad;
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y)
{
	// 중점 간의 길이
	float fX = abs(_Dst->Get_INFO().fX - _Src->Get_INFO().fX);
	float fY = abs(_Dst->Get_INFO().fY - _Src->Get_INFO().fY);

	// 반지름의 합
	int iCX = (_Dst->Get_INFO().iCX + _Src->Get_INFO().iCX) >> 1;
	int iCY = (_Dst->Get_INFO().iCY + _Src->Get_INFO().iCY) >> 1;

	if(iCX > fX && iCY > fY)
	{
		*_x = iCX - fX;
		*_y = iCY - fY;
		return true;
	}

	return false;

}
