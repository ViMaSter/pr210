#pragma once

#include "IRenderer.h"

#pragma comment(lib, "D3D10.lib")
#pragma comment(lib, "D3DX10.lib")

#include "D3D10.h"
#include "D3DX10Async.h"

#include "D3DCompiler.h"

namespace Framework {
	namespace Renderer {
		namespace DX10Renderer {
			class Renderer : public IRenderer {
			public:
#pragma region Direct X-relevant data
				// General data
				ID3D10Device* Device;
				ID3D10RenderTargetView* BackBufferView;
				ID3D10DepthStencilView* DepthStencilBufferView;
				IDXGISwapChain* SwapChain;
				ID3D10InputLayout* InputLayout;

				// Shader data
				ID3D10VertexShader* VertexShader;
				ID3D10PixelShader* PixelShader;
				ID3D10Blob *VS, *PS;

				// Vertex buffer data
				ID3D10Buffer* VertexBuffer;
				void* MappedSubresource;
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
