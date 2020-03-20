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

#endif // !__STRUCT_H__
