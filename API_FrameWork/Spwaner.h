#pragma once

class CObj;
class Spwaner
{
public:
	Spwaner();
	~Spwaner();

public:
	void Update();

public:
	static Spwaner* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new Spwaner;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

public:
	void Spwan(CObj* _obj, OBJID::ID _eID, float _delay);

private:
	static Spwaner*	m_pInstance;
	CObj* m_pToSpwan;
	OBJID::ID m_eID;
	DWORD m_dwSpawnTimer;
	float m_fDelay;
};

