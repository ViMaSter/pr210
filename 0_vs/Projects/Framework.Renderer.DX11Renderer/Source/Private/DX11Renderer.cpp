#pragma once

#include "DX11Renderer.h"

namespace Framework {
	namespace Renderer {
		namespace DX11Renderer {
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
				CurrentRendererType = Framework::Renderer::Types::DirectX11;
			}

			Renderer::~Renderer() {
				void* tmp = &Vertices[0];
				std::vector<SVertex>().swap(Vertices);
				if (Device != nullptr) {
					Device->Release();
					Device = nullptr;
				}
				if (DeviceContext != nullptr) {
					DeviceContext->Release();
					DeviceContext = nullptr;
				}
				if (BackBuffer != nullptr) {
					BackBuffer->Release();
					BackBuffer = nullptr;
				}
				if (BackBuffer != nullptr) {
					BackBuffer->Release();
					BackBuffer = nullptr;
				}
				if (SwapChain != nullptr) {
					SwapChain->Release();
					SwapChain = nullptr;
				}
				if (InputLayout != nullptr) {
					InputLayout->Release();
					InputLayout = nullptr;
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

				// Swap chain info
				DXGI_RATIONAL rat;
				rat.Numerator = 60;
				rat.Denominator = 1;
				DXGI_SWAP_CHAIN_DESC SwapChainDescription;
				SwapChainDescription.BufferDesc.Width = 1280;
				SwapChainDescription.BufferDesc.Height = 720;
				SwapChainDescription.BufferDesc.RefreshRate = rat;
				SwapChainDescription.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
				SwapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
				SwapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_CENTERED;
				SwapChainDescription.SampleDesc.Count = 1;
				SwapChainDescription.SampleDesc.Quality = 0;
				SwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				SwapChainDescription.BufferCount = 1;
				SwapChainDescription.OutputWindow = *WindowPointer;
				SwapChainDescription.Windowed = true;
				SwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
				SwapChainDescription.Flags = 0;

				// Create device
				tmpResult = D3D11CreateDeviceAndSwapChain(
					nullptr,
					D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
					nullptr,
					D3D11_CREATE_DEVICE_DEBUG,
					NULL,
					0,
					D3D11_SDK_VERSION,
					&SwapChainDescription,
					&SwapChain,
					&Device,
					&CurrentFeatureLevel,
					&DeviceContext
				);

				// Get back buffer
				tmpResult = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
				if (ErrorHandler(tmpResult)) {
					return;
				}

				tmpResult = Device->CreateRenderTargetView(
					BackBuffer,
					NULL,
					&BackBufferRenderTarget
				);
				if (ErrorHandler(tmpResult)) {
					return;
				}

				// Release back buffer
				BackBuffer->Release();

				// Set viewport size
				Viewport.TopLeftX = 0;
				Viewport.TopLeftY = 0;
				Viewport.Width = 1280;
				Viewport.Height = 720;

				DeviceContext->RSSetViewports(1, &Viewport);
				if (ErrorHandler(tmpResult)) {
					return;
				}

				// Fill in sample-values for easier debugging
				AddVertex(Framework::Renderer::SVertex(0.25f, 0.25f, 1.f, 0.0f, 0.0f, 0.0f, 1.f));
				AddVertex(Framework::Renderer::SVertex(0.25f, 0.75f, 1.f, 0.0f, 0.0f, 0.0f, 1.f));
				AddVertex(Framework::Renderer::SVertex(0.75f, 0.25f, 1.f, 0.0f, 0.0f, 0.0f, 1.f));
				AddVertex(Framework::Renderer::SVertex(0.75f, 0.75f, 1.f, 0.0f, 0.0f, 0.0f, 1.f));

				// Create vertex buffer
				// Fill in a buffer description.
				D3D11_BUFFER_DESC VertexBufferDesc;
				VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				VertexBufferDesc.ByteWidth = sizeof(SVertex) * Vertices.size();
				VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				VertexBufferDesc.MiscFlags = 0;

				// Fill in the subresource data.
				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = &Vertices[0];
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;

				// Create the vertex buffer.
				tmpResult = Device->CreateBuffer(&VertexBufferDesc, &InitData, &VertexBuffer);
				if (ErrorHandler(tmpResult)) {
					return;
				}

				// Set up shader
				// load and compile the two shaders
				D3DX11CompileFromFile("Resources\\Shader\\Shaders.shader", 0, 0, "VShader", "vs_4_0", D3DCOMPILE_DEBUG, 0, 0, &VS, 0, 0);
				D3DX11CompileFromFile("Resources\\Shader\\Shaders.shader", 0, 0, "PShader", "ps_4_0", D3DCOMPILE_DEBUG, 0, 0, &PS, 0, 0);

				// encapsulate both shaders into shader objects
				tmpResult = Device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &VertexShader);
				if (ErrorHandler(tmpResult)) {
					return;
				}

				tmpResult = Device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &PixelShader);
				if (ErrorHandler(tmpResult)) {
					return;
				}

				// set the shader objects
				DeviceContext->VSSetShader(VertexShader, 0, 0);
				DeviceContext->PSSetShader(PixelShader, 0, 0);

				// create the input layout object
				D3D11_INPUT_ELEMENT_DESC InputElementDescription[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};

				Device->CreateInputLayout(InputElementDescription, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &InputLayout);
				DeviceContext->IASetInputLayout(InputLayout);
			}

			void Renderer::Begin(int ClearColor) {
				HRESULT tmpResult;
				DWORD msg = GetLastError();

				// Ready up for draw calls
				DeviceContext->ClearRenderTargetView(BackBufferRenderTarget, (const float*)&(SColor(ClearColor)));

				DeviceContext->OMSetRenderTargets(1, &BackBufferRenderTarget, nullptr);

				const UINT offset = 0;
				const UINT stride = sizeof(SVertex);

				DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

				// select which primtive type we are using
				DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				// copy the vertices into the buffer
				tmpResult = DeviceContext->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedSubresource);    // map the buffer
				if (ErrorHandler(tmpResult)) {
					return;
				}
#ifdef FRAMEWORK_DEBUG
				std::cout << "X11 Begin" << std::endl;
#endif
			}

			void Renderer::End() {
				HRESULT tmpResult;

				memcpy(MappedSubresource.pData, &Vertices[0], sizeof(SVertex)*Vertices.size());                 // copy the data
				DeviceContext->Unmap(VertexBuffer, NULL);

				// draw vertices
				DeviceContext->Draw(Vertices.size(), 0);

				// present screen
				tmpResult = SwapChain->Present(VSync, 0);
				if (ErrorHandler(tmpResult)) {
					return;
				}

#ifdef FRAMEWORK_DEBUG
				std::cout << "X11 End" << std::endl;
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
					Renderer* originalPointer = (Renderer*)*origin;
					delete originalPointer;
					*origin = 0;
				}
			}
		}
	}
	
}
