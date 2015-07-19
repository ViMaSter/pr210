#pragma once

#include <vector>
#include <iostream>

#pragma comment(lib, "D3D10.lib")
#pragma comment(lib, "D3DX10.lib")

#include "D3D10.h"
#include "D3DX10Async.h"

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

class DirectX10Renderer {
public:
	// Custom data
	enum State {
		NOT_READY,
		INITIALIZED,
		VERTICES_ADDED
	} CurrentState = DirectX10Renderer::State::NOT_READY;
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

