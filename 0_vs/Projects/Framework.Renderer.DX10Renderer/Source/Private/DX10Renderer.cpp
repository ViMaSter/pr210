#pragma once

#include "DX10Renderer.h"

namespace Framework {
	namespace Renderer {
		namespace DX10Renderer {
			bool ErrorHandler(HRESULT result) {
				if (result != S_OK) {
					DWORD msg = GetLastError();
#ifdef FRAMEWORK_DEBUG
					std::cout << "Error id: " << msg << std::endl;
#endif
					return true;
				}
				return false;
			}

			Renderer::Renderer() {
				CurrentRendererType = Framework::Renderer::Types::DirectX10;
			}

			Renderer::~Renderer() {
				void* tmp = &Vertices[0];
				std::vector<SVertex>().swap(Vertices);
				if (Device != nullptr) {
					Device->Release();
					Device = nullptr;
				}
				if (BackBufferView != nullptr) {
					BackBufferView->Release();
					BackBufferView = nullptr;
				}
				if (DepthStencilBufferView != nullptr) {
					DepthStencilBufferView->Release();
					DepthStencilBufferView = nullptr;
				}
				if (SwapChain != nullptr) {
					SwapChain->Release();
					SwapChain = nullptr;
				}
				if (InputLayout != nullptr) {
					InputLayout->Release();
					InputLayout = nullptr;
				}
				if (VertexShader != nullptr) {
					VertexShader->Release();
					VertexShader = nullptr;
				}
				if (PixelShader != nullptr) {
					PixelShader->Release();
					PixelShader = nullptr;
				}
				if (VertexBuffer != nullptr) {
					VertexBuffer->Release();
					VertexBuffer = nullptr;
				}
				if (VS != nullptr) {
					VS->Release();
					VS = nullptr;
				}
				if (PS != nullptr) {
					PS->Release();
					PS = nullptr;
				}
			}

			void Renderer::Init() {
				// Set object state
				CurrentState = State::INITIALIZED;

				// Temporary result value
				HRESULT tmpResult;
				DWORD msg = GetLastError();

				Device = NULL;
				BackBufferView = NULL;
				DepthStencilBufferView = NULL;
				SwapChain = NULL;

				IDXGIDevice* dxgiDevice;
				IDXGIFactory* factory = NULL;
				IDXGIAdapter* adapter = NULL;
				ID3D10Texture2D* backBuffer;
				ID3D10Texture2D* depthStencilBuffer;
				D3D10_VIEWPORT viewPort;
				D3D10_TEXTURE2D_DESC depthStencilDescription;
				DXGI_SWAP_CHAIN_DESC swapChainDescription;

				D3D_FEATURE_LEVEL FeatureLevels[1] = { D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_0 };

				tmpResult = D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE::D3D10_DRIVER_TYPE_HARDWARE, NULL,
					D3D10_CREATE_DEVICE_SINGLETHREADED | D3D10_CREATE_DEVICE_BGRA_SUPPORT, D3D10_SDK_VERSION, &Device);

				tmpResult = this->Device->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&dxgiDevice);

				tmpResult = dxgiDevice->GetAdapter(&adapter);

				tmpResult = adapter->GetParent(IID_PPV_ARGS(&factory));

				ZeroMemory(&swapChainDescription, sizeof(swapChainDescription));
				swapChainDescription.BufferDesc.Width = 1280;
				swapChainDescription.BufferDesc.Height = 720;
				swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
				swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
				swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
				swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
				swapChainDescription.SampleDesc.Count = 1;
				swapChainDescription.SampleDesc.Quality = 0;
				swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapChainDescription.BufferCount = 1;
				swapChainDescription.OutputWindow = *WindowPointer;
				swapChainDescription.Windowed = true;
				swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
				swapChainDescription.Flags = 0;

				tmpResult = factory->CreateSwapChain(Device, &swapChainDescription, &SwapChain);

				tmpResult = SwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBuffer);

				tmpResult = Device->CreateRenderTargetView(backBuffer, NULL, &BackBufferView);

				// Set up shader
				// load and compile the two shaders
				D3DX10CompileFromFile("Resources\\Shader\\Shaders.shader", 0, 0, "VShader", "vs_4_0", D3DCOMPILE_DEBUG, 0, 0, &VS, 0, 0);
				D3DX10CompileFromFile("Resources\\Shader\\Shaders.shader", 0, 0, "PShader", "ps_4_0", D3DCOMPILE_DEBUG, 0, 0, &PS, 0, 0);

				// encapsulate both shaders into shader objects
				tmpResult = Device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), &VertexShader);
				if (ErrorHandler(tmpResult)) {
					return;
				}

				tmpResult = Device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), &PixelShader);
				if (ErrorHandler(tmpResult)) {
					return;
				}

				Device->VSSetShader(VertexShader);
				Device->PSSetShader(PixelShader);

				D3D10_INPUT_ELEMENT_DESC InputElementDescription[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
				};

				Device->CreateInputLayout(InputElementDescription, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &InputLayout);
				Device->IASetInputLayout(InputLayout);

				AddVertex(Framework::Renderer::SVertex(0.25f, 0.25f, 1.f, 0.0f, 0.0f, 0.0f, 1.f));
				AddVertex(Framework::Renderer::SVertex(0.25f, 0.75f, 1.f, 0.0f, 0.0f, 0.0f, 1.f));
				AddVertex(Framework::Renderer::SVertex(0.75f, 0.25f, 1.f, 0.0f, 0.0f, 0.0f, 1.f));
				AddVertex(Framework::Renderer::SVertex(0.75f, 0.75f, 1.f, 0.0f, 0.0f, 0.0f, 1.f));

				D3D10_BUFFER_DESC bd;
				bd.Usage = D3D10_USAGE_DYNAMIC;
				bd.ByteWidth = sizeof(SVertex) * Vertices.size(); //total size of buffer in bytes
				bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
				bd.MiscFlags = 0;

				D3D10_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = &Vertices[0];
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;

				tmpResult = Device->CreateBuffer(&bd, &InitData, &VertexBuffer);

				ZeroMemory(&depthStencilDescription, sizeof(depthStencilDescription));
				depthStencilDescription.Width = 1280;
				depthStencilDescription.Height = 720;
				depthStencilDescription.MipLevels = 1;
				depthStencilDescription.ArraySize = 1;
				depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthStencilDescription.SampleDesc.Count = 1;
				depthStencilDescription.SampleDesc.Quality = 0;
				depthStencilDescription.Usage = D3D10_USAGE_DEFAULT;
				depthStencilDescription.BindFlags = D3D10_BIND_DEPTH_STENCIL;
				depthStencilDescription.CPUAccessFlags = 0;
				depthStencilDescription.MiscFlags = 0;

				tmpResult = Device->CreateTexture2D(&depthStencilDescription, 0, &depthStencilBuffer);

				tmpResult = Device->CreateDepthStencilView(depthStencilBuffer, 0, &DepthStencilBufferView);

				viewPort.Width = 1280;
				viewPort.Height = 720;
				viewPort.MinDepth = 0.0f;
				viewPort.MaxDepth = 1.0f;
				viewPort.TopLeftX = 0;
				viewPort.TopLeftY = 0;

				Device->RSSetViewports(1, &viewPort);

				// Post PreDraw - PreUpdate
#ifdef FRAMEWORK_DEBUG
				std::cout << "X10 Init" << std::endl;
#endif
			}

			void Renderer::Begin(int ClearColor) {
				HRESULT tmpResult;

				Device->ClearRenderTargetView(BackBufferView, (const float*)&(SColor(ClearColor)));

				Device->OMSetRenderTargets(1, &BackBufferView, nullptr);

				const UINT offset = 0;
				const UINT stride = sizeof(SVertex);

				Device->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

				Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				// copy the vertices into the buffer
				tmpResult = VertexBuffer->Map(D3D10_MAP::D3D10_MAP_WRITE_DISCARD, NULL, &MappedSubresource);    // map the buffer
				if (ErrorHandler(tmpResult)) {
					return;
				}

				// Post PreDraw - PreUpdate
#ifdef FRAMEWORK_DEBUG
				std::cout << "X10 Begin" << std::endl;
#endif
			}
			void Renderer::End() {
				HRESULT tmpResult;

				memcpy(MappedSubresource, &Vertices[0], sizeof(SVertex)*Vertices.size());                 // copy the data
				VertexBuffer->Unmap();

				Device->Draw(Vertices.size(), 0);

				tmpResult = SwapChain->Present(VSync, 0);
				if (ErrorHandler(tmpResult)) {
					return;
				}

				// Pre Update - Pre PostDraw
#ifdef FRAMEWORK_DEBUG
				std::cout << "X10 End" << std::endl;
#endif
			}

			void Renderer::AddVertex(SVertex v) {
				if (CurrentState == State::NOT_READY) {
					CurrentState = State::VERTICES_ADDED;
					Vertices.clear();
				}

				Vertices.push_back(v);
			}



			IRenderer* CreateRenderer(IRenderer** destination) {
				*destination = new Renderer();

				return *destination;
			}

			void DeleteRenderer(IRenderer** origin) {
				if (*origin != nullptr) {
					delete *origin;
				}
			}
		}
	}

}