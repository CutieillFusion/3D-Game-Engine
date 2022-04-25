#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
//https://docs.microsoft.com/en-us/windows/win32/direct3d9/what-is-a-swap-chain-
//This is abunch of virtual framebuffers utilized by the graphics card for frame rate and someother stuff
class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);

	bool present(bool vsync);

	~SwapChain();
private:
	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_rtv = nullptr;
	ID3D11DepthStencilView* m_dsv = nullptr;
	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};