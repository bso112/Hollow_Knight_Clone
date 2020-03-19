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
	// static 함수로 구현한 이유는 객체를 생성하지 않아도 스코프연산자를 통해 호출이 가능하다!
	static void Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src);
	static void Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src);
	static void Collision_Sphere(list<CObj*> _Dst, list<CObj*> _Src);

	static bool Check_Sphere(CObj* _Dst, CObj* _Src);
	static bool Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y);
};


#endif // !__COLLISIONMGR_H__
