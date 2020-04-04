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
	//��������Ʈ ��Ʈ����,
	//�ִϸ��̼��� ���۵Ǵ� ù �������� �ε���.(�ִϸ��̼��� ����ȣ) ���� 0�̴�.
	int			iFrameStart;
	//�ִϸ��̼��� ������ ������ �������� �ε���. ��, �� �ִϸ��̼��� �����ϴ� ������ �� -1
	int			iFrameEnd;
	//�ִϸ��̼��� ���ȣ
	int			iFrameScene;
	//�� �������� �� �и������嵿�� ����� ���ΰ�.
	DWORD		dwFrameSpeed;
	//�������� ������ �ʸ� ���� ���� Ÿ�̸�
	DWORD		dwFrameTime;
	//��������
	bool		bLoop;
	//�ִϸ��̼��� ������
	bool		bEnd; 
	//����Ƚ��
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
		// 0���δ� ���� �� ����.
		if (mag > 0)
		{
			fX = fX / mag;
			fY = fY / mag;
			return *this;
		}

		return *this;
	}

	//�ӽð�ü�� ���ϸ� �һ�, �ڱ��ڽ��� ���� ���������� ����
	tagVector2 Nomalize()
	{
		tagVector2 vec = {};
		float mag = magnitude();
		// 0���δ� ���� �� ����.
		if (mag > 0)
		{
			vec.fX = fX / mag;
			vec.fY = fY / mag;
			return vec;
		}
		//������ ���̰� 0 �̸� �� ���͸� ��� 
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
