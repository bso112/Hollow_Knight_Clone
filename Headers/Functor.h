#pragma once

#ifndef __FUNCTOR_H__
#define __FUNCTOR_H__

class CStringCMP
{
public:
	CStringCMP(const TCHAR* _pString) : m_String(_pString) {}

public:
	template<typename T>
	bool operator()(T& _obj)
	{
		return !lstrcmp(m_String, _obj.first);
	}

private:
	const TCHAR* m_String;
};

#endif // !__FUNCTOR_H__
