#pragma once

#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct tagInfo
{
	float	fX;
	float	fY;
	int		iCX;
	int		iCY;
}INFO;

typedef struct tagLinePos
{
	tagLinePos() { ZeroMemory(this, sizeof(tagLinePos)); }
	tagLinePos(float _x, float _y) :fX(_x), fY(_y) {}

	float	fX;
	float	fY;
}LINEPOS;

typedef struct tagLineInfo
{
	tagLineInfo() { ZeroMemory(this, sizeof(tagLineInfo)); }
	tagLineInfo(LINEPOS& _tLeft, LINEPOS& _tRight)
		: tLeftPos(_tLeft), tRightPos(_tRight) {}

	LINEPOS		tLeftPos;
	LINEPOS		tRightPos;
}LINEINFO;


typedef struct tagFrame
{
	//스프라이트 시트에서,
	//애니메이션이 시작되는 첫 프레임의 인덱스.(애니메이션의 열번호) 보통 0이다.
	int			iFrameStart;
	//애니메이션이 끝나는 마지막 프레임의 인덱스. 즉, 한 애니메이션을 구성하는 프레임 수 -1
	int			iFrameEnd;
	//애니메이션의 행번호
	int			iFrameScene;
	//한 프레임을 몇 밀리세컨드동안 출력할 것인가.
	DWORD		dwFrameSpeed;
	//프레임을 실행한 초를 세기 위한 타이머
	DWORD		dwFrameTime;


}FRAME;
#endif // !__STRUCT_H__
