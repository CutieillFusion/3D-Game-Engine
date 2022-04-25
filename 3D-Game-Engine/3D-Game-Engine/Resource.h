#pragma once
#include <string>
//Base Class of all Resources
class Resource
{
public:
	Resource(const wchar_t* full_path);
	virtual ~Resource();
protected:
	std::wstring m_full_path;
};

