#pragma once
#include <unordered_map>
#include <string>
#include "Prerequisites.h"
#include "Resource.h"

//Base class of all the Resource's Managers
class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	ResourcePtr createResourceFromFile(const wchar_t* file_path);
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) = 0;
private:
	std::unordered_map<std::wstring, ResourcePtr> m_map_resources;//Holds a list of the reosurces with path as the index
};