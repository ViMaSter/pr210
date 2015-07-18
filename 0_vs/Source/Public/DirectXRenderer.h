#pragma once

#include <vector>
#include <iostream>

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DX11.lib")

#include "D3D11.h"
#include "D3DX11async.h"

#include "D3DCompiler.h"

#pragma region Position
typedef float TPosition[3];
struct SPosition {
	float X;
	float Y;
	float Z;

	SPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
		X = x;
		Y = y;
		Z = z;
	}
};
#pragma endregion

#pragma region Color
typedef float TColor[4];
/// Color ranges from 0.0f to 1.0f
struct SColor {
	float R;
	float G;
	float B;
	float A;

	SColor(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) {
		R = r;
		G = g;
		B = b;
		A = a;
	}
};
#pragma endregion

#pragma region Vertex
struct SVertex {
	SPosition Position;
	SColor Color;

	SVertex(float X = 0.0f, float Y = 0.0f, float Z = 0.0f, float R = 0.0f, float G = 0.0f, float B = 0.0f, float A = 1.0f) {
		Position = SPosition(X, Y, Z);
		Color = SColor(R, G, B, A);
	}
};
#pragma endregion

class DirectXRenderer {
public:
	// Custom data
	enum State {
		NOT_READY,
		INITIALIZED,
		VERTICES_ADDED
	} CurrentState = DirectXRenderer::State::NOT_READY;
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

