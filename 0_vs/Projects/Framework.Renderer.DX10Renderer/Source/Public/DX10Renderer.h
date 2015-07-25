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

				void* MappedSubresource;
				ID3D10Buffer* VertexBuffer;

				// Custom data
				int VSync = 0;

				Renderer();
				~Renderer();

				virtual void Init(); // Pre draw
				virtual void Begin(int ClearColor); // Pre draw
				virtual void End(); // = Draw() + Present()
				virtual void AddVertex(SVertex v);
			};

			IRenderer* CreateRenderer(IRenderer** destination);
			void DeleteRenderer(IRenderer** origin);
		}
	}
}
