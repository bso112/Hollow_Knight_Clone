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
	//루프유무
	bool		bLoop;
	//애니메이션이 끝났냐
	bool		bEnd; 
	//루프횟수
	int			iLoopCnt = 0;

	tagFrame() :bLoop(true) {};


}FRAME;

typedef struct tagStat
{
	float m_fMaxHp;
	float m_fHp;

	tagStat() {};
	tagStat(float _fMaxHp)
	{
		m_fMaxHp = _fMaxHp;
		m_fHp = _fMaxHp;
	}

}STAT;


typedef struct tagVector2
{
	float fX;
	float fY;

	tagVector2() { fX = 0; fY = 0; }

	tagVector2(float _fX, float _fY)
	{
		fX = _fX;
		fY = _fY;
	}

	tagVector2 operator+(tagVector2 _vec)
	{
		return tagVector2(fX + _vec.fX, fY + _vec.fY);
	}

	tagVector2 operator-(tagVector2 _vec)
	{
		return tagVector2(fX - _vec.fX, fY - _vec.fY);
	}
	tagVector2& operator+=(tagVector2 _vec)
	{
		fX += _vec.fX;
		fY += _vec.fY;
		return *this;
	}

	tagVector2& operator-=(tagVector2 _vec)
	{
		fX -= _vec.fX;
		fY -= _vec.fY;
		return *this;
	}

	tagVector2& operator+=(float _scalar)
	{
		fX += _scalar;
		fY += _scalar;
		return *this;
	}

	tagVector2& operator-=(float _scalar)
	{
		fX -= _scalar;
		fY -= _scalar;
		return *this;
	}

	tagVector2 operator*(float _scalar)
	{
		return tagVector2(fX * _scalar, fY * _scalar);
	}
	tagVector2 operator+(float _scalar)
	{

		return tagVector2(fX  +_scalar, fY + _scalar);
	}

	tagVector2 operator-(float _scalar)
	{
		return tagVector2(fX  -_scalar, fY - _scalar);
	}

	tagVector2& nomalize()
	{
		float mag = magnitude();
		// 0으로는 나눌 수 없다.
		if (mag > 0)
		{
			fX = fX / mag;
			fY = fY / mag;
			return *this;
		}

		return *this;
	}

	//임시객체를 리턴만 할뿐, 자기자신의 값을 변경하지는 않음
	tagVector2 Nomalize()
	{
		tagVector2 vec = {};
		float mag = magnitude();
		// 0으로는 나눌 수 없다.
		if (mag > 0)
		{
			vec.fX = fX / mag;
			vec.fY = fY / mag;
			return vec;
		}
		//벡터의 길이가 0 이면 빈 벡터를 출력 
		return vec;
	}

	float magnitude()
	{
		return sqrtf(fX*fX + fY * fY);
	}

}Vector2;


typedef struct tagLine
{
	Vector2 src;
	Vector2 dst;

	tagLine() {};
	tagLine(Vector2 _src, Vector2 _dst) { src = _src; dst = _dst; }
	
}LINE;
#endif // !__STRUCT_H__
