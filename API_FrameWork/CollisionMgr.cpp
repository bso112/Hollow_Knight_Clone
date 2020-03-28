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
				
				//Dst�� �浹�����̰�, src�� �浹�� �ƴ� ���¿��� Dst�� ���ݸ������� ����
				//Dst�浹 �߿��� src�� ���� �ͼ� �浹�ص� �ݸ��� ���ʹϱ�.
				if (Dst->Get_isCollided() && !Src->Get_isCollided())
					Dst->OnCollisionEnter(Src, fX, fY);

				//�ݴ뵵 ��������.
				if (!Dst->Get_isCollided() && Src->Get_isCollided())
					Src->OnCollisionEnter(Dst, fX, fY);
				

				//�浹�̰� �浹������ false�� �浹���Ի���
				if (!Dst->Get_isCollided())
					Dst->OnCollisionEnter(Src, fX, fY);
				if (!Src->Get_isCollided())
					Src->OnCollisionEnter(Dst, fX, fY);


				Dst->Set_isCollided(true);
				Src->Set_isCollided(true);

				Dst->OnCollisionStay(Src, fX, fY);
				Src->OnCollisionStay(Dst, fX, fY);

			}
			else
			{
				//�ٲ�ߵ�. �浹ī��Ʈ ����ؼ�.. ���� �̹� �浹�� �ֵ鳢�� �浹�Ѵٸ�? -> �̰��� �Ű澵�ʿ� ����. ������ ���Ͷ� �÷��̾ �浹ó���ҰŴϱ�.
				if (Dst->Get_isCollided() && Src->Get_isCollided())
				{
					Dst->OnCollisionExit(Src, fX, fY);
					Dst->Set_isCollided(false);
					Src->OnCollisionExit(Dst, fX, fY);
					Src->Set_isCollided(false);
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
				if(!Dst->Get_isCollided())
					Dst->OnCollisionEnter(Src, fX, fY);
				if(!Src->Get_isCollided())
					Src->OnCollisionEnter(Dst, fX, fY);

				Dst->Set_isCollided(true);
				Src->Set_isCollided(true);

				Dst->OnCollisionStay(Src, fX, fY);
				Src->OnCollisionStay(Dst, fX, fY);

			}
			else
			{
				if (Dst->Get_isCollided() && Src->Get_isCollided())
				{
					Dst->OnCollisionExit(Src, fX, fY);
					Dst->Set_isCollided(false);
					Src->OnCollisionExit(Dst, fX, fY);
					Src->Set_isCollided(false);
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
				if (!Dst->Get_isCollided())
					Dst->OnCollisionEnter(Src, fX, fY);
				if (!Src->Get_isCollided())
					Src->OnCollisionEnter(Dst, fX, fY);

				Dst->Set_isCollided(true);
				Src->Set_isCollided(true);

				Dst->OnCollisionStay(Src, fX, fY);
				Src->OnCollisionStay(Dst, fX, fY);

			}
			else
			{
				if (Dst->Get_isCollided() && Src->Get_isCollided())
				{
					Dst->OnCollisionExit(Src, fX, fY);
					Dst->Set_isCollided(false);
					Src->OnCollisionExit(Dst, fX, fY);
					Src->Set_isCollided(false);
				}

			}
		}
	}
}



bool CCollisionMgr::Check_Sphere(CObj * _Dst, CObj * _Src)
{
	// abs()
	// ���밪. ������ ������ �ʴ´�.

	// sqrtf()
	// �������� �����ִ� �Լ��̴�.

	float fX = abs(_Dst->Get_INFO().fX - _Src->Get_INFO().fX);
	float fY = abs(_Dst->Get_INFO().fY - _Src->Get_INFO().fY);
	float fDis = sqrtf(fX * fX + fY * fY);

	float fRad = float((_Dst->Get_INFO().iCX + _Src->Get_INFO().iCX) >> 1);

	return fDis < fRad;
}

bool CCollisionMgr::Check_Sphere(const RECT& _Dst, const RECT& _Src)
{
	// abs()
	// ���밪. ������ ������ �ʴ´�.

	// sqrtf()
	// �������� �����ִ� �Լ��̴�.

	float radius_Dst = (float)((_Dst.right - _Dst.left) >> 1);
	float radius_Src = (float)((_Src.right - _Src.left) >> 1);


	float fX = abs((_Dst.left + radius_Dst) - (_Src.left + radius_Src));
	float fY = abs((_Dst.bottom - radius_Dst) - (_Src.bottom - radius_Src));
	//�� �� ������ �Ÿ�
	float fDis = sqrtf(fX * fX + fY * fY);

	//�������� �ִܰŸ�
	float fDiameter = radius_Dst + radius_Src;

	return fDis < fDiameter;
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y)
{
	// ���� ���� ����
	float fX = abs(_Dst->Get_INFO().fX - _Src->Get_INFO().fX);
	float fY = abs(_Dst->Get_INFO().fY - _Src->Get_INFO().fY);

	// �������� ��
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
