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
#pragma region Direct X-relevant data
				// General data
				ID3D11Device* Device;
				ID3D11DeviceContext* DeviceContext;
				ID3D11Texture2D* BackBuffer;
				ID3D11RenderTargetView* BackBufferRenderTarget;
				IDXGISwapChain* SwapChain;
				ID3D11InputLayout* InputLayout;
				D3D11_VIEWPORT Viewport;
				D3D_FEATURE_LEVEL CurrentFeatureLevel;

				// Shader data
				ID3D11VertexShader* VertexShader;
				ID3D11PixelShader* PixelShader;
				ID3D10Blob *VS, *PS;

				// Vertex buffer data
				ID3D11Buffer* VertexBuffer;
				D3D11_MAPPED_SUBRESOURCE MappedSubresource;
#pragma endregion

				Renderer();
				virtual ~Renderer();

				virtual void Init();
				virtual void Begin(int ClearColor);
				virtual void End();
				virtual void AddVertex(SVertex v);
			};

			IRenderer* CreateRenderer(IRenderer** destination);
			void DeleteRenderer(IRenderer** origin);
		}
	}
}
