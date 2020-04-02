#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__


class CObj;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	//�Ϲ����� �簢�� �浹�Լ�
	static bool Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src);
	//onCollisionEnter�� �浹������ �����ϴ� �浹�Լ� 
	static void Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src);
	//���浹 �Լ�
	static void Collision_Sphere(list<CObj*> _Dst, list<CObj*> _Src);

public:
	static bool Check_Sphere(const RECT& _Dst, const RECT& _Src);
	static bool Check_Sphere(CObj* _Dst, CObj* _Src);
	static bool Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y);
	static bool Check_Rect(INFO _Dst, INFO _Src, float* _x, float* _y);
	//�� ������ ������ �˷��ش�.
	static bool Intersect_LIne(const LINE _line1, const LINE _line2, Vector2& _result);

};


#endif // !__COLLISIONMGR_H__
