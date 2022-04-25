#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
//https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-buffers-constant-how-to
//Used to hold data the persists and can be accessed by any GPU shader for that update cycle
//Data Stays Constant until the next Update Cycle

//This holds world, projection, and view matrices
//Also holds a Time Value
class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system);
	void update(DeviceContextPtr context, void* buffer);
	~ConstantBuffer();
private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};