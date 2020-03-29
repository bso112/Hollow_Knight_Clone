#pragma once
class CMyTime
{
public:
	CMyTime();
	~CMyTime();

public:
	void Initalize();
	void Update();

public:
	float Get_DeltaTime() { return (float)m_dwDeltaTime / 1000; }

public:
	static CMyTime* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CMyTime;
		return m_pInstance;
	}
private:
	static CMyTime* m_pInstance;
	DWORD m_dwPrvTime;
	DWORD m_dwCurrTime;
	DWORD m_dwDeltaTime;

	
};

