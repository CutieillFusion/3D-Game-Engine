#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
//https://www.ubisoft.com/en-ca/help/gameplay/article/what-is-vertex-and-pixel-shading/000072849
//This is Allows us to add special effects to objects in 3D through the pixel data
class PixelShader
{
public:
	PixelShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);
	~PixelShader();
private:
	ID3D11PixelShader* m_ps;
	RenderSystem* m_system = nullptr;
private:
	friend class RenderSystem;
	friend class DeviceContext;
};

