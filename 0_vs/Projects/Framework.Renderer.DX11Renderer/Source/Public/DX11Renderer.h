#pragma once

#include "IRenderer.h"

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DX11.lib")

#include "D3D11.h"
#include "D3DX11async.h"

#include "D3DCompiler.h"

namespace Framework {
	namespace Renderer {
		namespace DX11Renderer {
			class Renderer : public IRenderer {
			public:
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

				D3D11_MAPPED_SUBRESOURCE MappedSubresource;
				ID3D11Buffer* VertexBuffer;

				// Custom data
				int VSync = 0;

				Renderer();
				~Renderer();

				virtual void Init(); // Pre draw
				virtual void Begin(int ClearColor); // Pre draw
				virtual void End(); // = Draw() + Present()
				virtual void AddVertex(SVertex v);
			};

			IRenderer* __declspec(dllexport) CreateRenderer(IRenderer** destination);
			void __declspec(dllexport) DeleteRenderer(IRenderer** origin);
		}
	}
}
