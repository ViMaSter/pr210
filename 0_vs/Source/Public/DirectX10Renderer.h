#pragma once

#include <vector>
#include <iostream>

#pragma comment(lib, "D3D10.lib")
#pragma comment(lib, "D3DX10.lib")

#include "D3D10.h"
#include "D3DX10Async.h"

#include "D3DCompiler.h"

#include "DXTypes.h"

class DirectX10Renderer {
public:
	// Custom data
	State CurrentState = State::NOT_READY;
	std::vector<SVertex> Vertices;

	// Dx data
	/// @brief A pointer to the device ID3D10Device object.
	///
	ID3D10Device* device;

	/// @brief A pointer to the backbuffer ID3D10RenderTargetView object.
	///
	ID3D10RenderTargetView* backBufferView;

	/// @brief A pointer to the depth-stencil-buffer ID3D10DepthStencilView object.
	///
	ID3D10DepthStencilView* depthStencilBufferView;

	/// @brief A pointer to the swap-chain IDXGISwapChain object.
	///
	IDXGISwapChain* swapChain;
	ID3D10InputLayout* InputLayout;

	// Shader data
	ID3D10VertexShader* VertexShader;
	ID3D10PixelShader* PixelShader;

	ID3D10Buffer* VertexBuffer;

	// Custom data
	int VSync = 0;
	SColor ClearBackBufferColor;


	// ctor
	DirectX10Renderer();
	// dtor
	~DirectX10Renderer();


	// Custom methods
	bool Init(HWND* windowPointer);
	bool Render();
	void AddVertex(SVertex v);


	// Static methods
	static void ErrorHandler(int ID);
};

