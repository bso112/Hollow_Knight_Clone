#include "stdafx.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "Monster.h"
#include "Mouse.h"


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

bool CCollisionMgr::Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src)
{

	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		float fX = 0.f, fY = 0.f;

		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				//만약 서로 충돌한 적이 없으면 서로의 충돌리스트에 서로를 추가한다.
				if (!Dst->Contain_Collided(Src))
				{
					Dst->Add_Collided(Src);
					Dst->Set_isCollided(true);
					Dst->OnCollisionEnter(Src, fX, fY);
				}
				if (!Src->Contain_Collided(Dst))
				{
					Src->Add_Collided(Dst);
					Src->Set_isCollided(true);
					Src->OnCollisionEnter(Dst, fX, fY);
				}


				Dst->OnCollisionStay(Src, fX, fY);
				Src->OnCollisionStay(Dst, fX, fY);

			}
			else
			{
				//만약 상대방이 전에 충돌한 리스트에 있으면 삭제한다.
				Dst->Erase_Collided(Src);
				Src->Erase_Collided(Dst);

				//충돌리스트가 비었고, 전에 충돌한적이 있으면 collisionExit을 실행한다.
				if (Dst->Get_CollidedSize() == 0 && Dst->Get_isCollided())
				{
					Dst->Set_isCollided(false);
					Dst->OnCollisionExit(Src, fX, fY);
				}
				if (Src->Get_CollidedSize() == 0 && Src->Get_isCollided())
				{
					Src->Set_isCollided(false);
					Src->OnCollisionExit(Dst, fX, fY);
				}
			}
		}
	}
	return false;
}

void CCollisionMgr::Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src)
{


	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			float fX = 0.f, fY = 0.f;

			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				//만약 서로 충돌한 적이 없으면 서로의 충돌리스트에 서로를 추가한다.
				if (!Dst->Contain_Collided(Src))
				{
					Dst->Add_Collided(Src);
					Dst->Set_isCollided(true);
					Dst->OnCollisionEnter(Src, fX, fY);
				}
				if (!Src->Contain_Collided(Dst))
				{
					Src->Add_Collided(Dst);
					Src->Set_isCollided(true);
					Src->OnCollisionEnter(Dst, fX, fY);
				}


				Dst->OnCollisionStay(Src, fX, fY);
				Src->OnCollisionStay(Dst, fX, fY);

			}
			else
			{
				//만약 상대방이 전에 충돌한 리스트에 있으면 삭제한다.
				Dst->Erase_Collided(Src);
				Src->Erase_Collided(Dst);

				//충돌리스트가 비었고, 전에 충돌한적이 있으면 collisionExit을 실행한다.
				if (Dst->Get_CollidedSize() == 0 && Dst->Get_isCollided())
				{
					Dst->Set_isCollided(false);
					Dst->OnCollisionExit(Src, fX, fY);
				}
				if (Src->Get_CollidedSize() == 0 && Src->Get_isCollided())
				{
					Src->Set_isCollided(false);
					Src->OnCollisionExit(Dst, fX, fY);
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere(list<CObj*> _Dst, list<CObj*> _Src)
{
	for (auto& Dst : _Dst)
	{

		for (auto& Src : _Src)
		{
			float fX = 0.f, fY = 0.f;

			if (Check_Sphere(Dst, Src))
			{
				//만약 서로 충돌한 적이 없으면 서로의 충돌리스트에 서로를 추가한다.
				if (!Dst->Contain_Collided(Src))
				{
					Dst->Add_Collided(Src);
					Dst->Set_isCollided(true);
					Dst->OnCollisionEnter(Src, fX, fY);
				}
				if (!Src->Contain_Collided(Dst))
				{
					Src->Add_Collided(Dst);
					Src->Set_isCollided(true);
					Src->OnCollisionEnter(Dst, fX, fY);
				}


				Dst->OnCollisionStay(Src, fX, fY);
				Src->OnCollisionStay(Dst, fX, fY);

			}
			else
			{
				//만약 상대방이 전에 충돌한 리스트에 있으면 삭제한다.
				Dst->Erase_Collided(Src);
				Src->Erase_Collided(Dst);

				//충돌리스트가 비었고, 전에 충돌한적이 있으면 collisionExit을 실행한다.
				if (Dst->Get_CollidedSize() == 0 && Dst->Get_isCollided())
				{
					Dst->Set_isCollided(false);
					Dst->OnCollisionExit(Src, fX, fY);
				}
				if (Src->Get_CollidedSize() == 0 && Src->Get_isCollided())
				{
					Src->Set_isCollided(false);
					Src->OnCollisionExit(Dst, fX, fY);
				}
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

bool CCollisionMgr::Check_Sphere(const RECT& _Dst, const RECT& _Src)
{
	// abs()
	// 절대값. 음수가 나오지 않는다.

	// sqrtf()
	// 제곱근을 구해주는 함수이다.

	float radius_Dst = (float)((_Dst.right - _Dst.left) >> 1);
	float radius_Src = (float)((_Src.right - _Src.left) >> 1);


	float fX = abs((_Dst.left + radius_Dst) - (_Src.left + radius_Src));
	float fY = abs((_Dst.bottom - radius_Dst) - (_Src.bottom - radius_Src));
	//두 원 사이의 거리
	float fDis = sqrtf(fX * fX + fY * fY);

	//지름이자 최단거리
	float fDiameter = radius_Dst + radius_Src;

	return fDis < fDiameter;
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y)
{
	// 중점 간의 길이
	float fX = abs(_Dst->Get_INFO().fX - _Src->Get_INFO().fX);
	float fY = abs(_Dst->Get_INFO().fY - _Src->Get_INFO().fY);

	// 반지름의 합
	int iCX = (_Dst->Get_INFO().iCX + _Src->Get_INFO().iCX) >> 1;
	int iCY = (_Dst->Get_INFO().iCY + _Src->Get_INFO().iCY) >> 1;

	if (iCX > fX && iCY > fY)
	{
		*_x = iCX - fX;
		*_y = iCY - fY;
		return true;
	}

	return false;

}
