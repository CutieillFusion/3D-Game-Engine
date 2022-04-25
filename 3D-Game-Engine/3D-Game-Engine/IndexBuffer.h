#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
//https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-buffers-index-how-to
//This holds the a list of Indices of a Mesh
//This is directly linked to the Vertex Buffer
//	Each Index is a pointer to a Vertex in Vertex Buffer
class IndexBuffer
{
public:
	IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system);
	UINT getSizeIndexList();
	~IndexBuffer();
private:
	UINT m_size_list;
private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};