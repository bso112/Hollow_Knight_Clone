#pragma once
#ifndef __SCREWBULLET_H__
#define __SCREWBULLET_H__


#include "Obj.h"
class CScrewBullet : public CObj
{
public:
	CScrewBullet();
	virtual ~CScrewBullet();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	POINT		m_tCenter;
	float		m_fRotAngle;
	float		m_fRotSpeed;
	float		m_fRotDis;

	bool		m_bStart;
};


#endif // !__SCREWBULLET_H__
