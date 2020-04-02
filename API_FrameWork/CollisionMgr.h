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
	//일반적인 사각형 충돌함수
	static bool Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src);
	//onCollisionEnter에 충돌영역을 전달하는 충돌함수 
	static void Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src);
	//구충돌 함수
	static void Collision_Sphere(list<CObj*> _Dst, list<CObj*> _Src);

public:
	static bool Check_Sphere(const RECT& _Dst, const RECT& _Src);
	static bool Check_Sphere(CObj* _Dst, CObj* _Src);
	static bool Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y);
	static bool Check_Rect(INFO _Dst, INFO _Src, float* _x, float* _y);
	//두 접점의 교점을 알려준다.
	static bool Intersect_LIne(const LINE _line1, const LINE _line2, Vector2& _result);

};


#endif // !__COLLISIONMGR_H__
