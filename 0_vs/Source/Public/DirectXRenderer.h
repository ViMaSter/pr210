#pragma once

#include <vector>
#include <iostream>

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DX11.lib")

#include "D3D11.h"
#include "D3DX11async.h"

#include "D3DCompiler.h"

#include "DXTypes.h"

class DirectXRenderer {
public:
	// Custom data
	State CurrentState = State::NOT_READY;
	std::vector<SVertex> Vertices;

	// Dx data
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;
	ID3D11Texture2D* pBackBuffer;
	ID3D11RenderTargetView* BackBuffer;
	IDXGISwapChain* SwapChain;
	ID3D11InputLayout* InputLayout;
	D3D11_VIEWPORT Viewport;
	D3D_FEATURE_LEVEL CurrentFeatureLevel;

	// Shader data
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;

	ID3D11Buffer* VertexBuffer;

	// Custom data
	int VSync = 0;
	SColor ClearBackBufferColor;


	// ctor
	DirectXRenderer();
	// dtor
	~DirectXRenderer();


	// Custom methods
	bool Init(HWND* windowPointer);
	bool Render();
	void AddVertex(SVertex v);


	// Static methods
	static void ErrorHandler(int ID);
};

