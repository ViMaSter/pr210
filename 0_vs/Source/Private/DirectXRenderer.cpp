#include "stdafx.h"
#include "DirectXRenderer.h"

DirectXRenderer::DirectXRenderer() {
	ClearBackBufferColor = SColor(0.0f, 0.75f, 1.0f);
	Vertices.push_back(SVertex());
}

DirectXRenderer::~DirectXRenderer() {

}

void DirectXRenderer::ErrorHandler(int ID) {
	printf("Error id: %d", ID);
}

bool DirectXRenderer::Init(HWND* windowPointer) {
	// Set object state
	CurrentState = DirectXRenderer::State::INITIALIZED;

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
	SwapChainDescription.OutputWindow = *windowPointer;
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

	if (tmpResult != S_OK) {
		msg = GetLastError();
		DirectXRenderer::ErrorHandler(msg);
		return false;
	}

	// Get back buffer
	tmpResult = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (tmpResult != S_OK) {
		msg = GetLastError();
		DirectXRenderer::ErrorHandler(msg);
		return false;
	}

	tmpResult = Device->CreateRenderTargetView(
		pBackBuffer,
		NULL,
		&BackBuffer
	);

	if (tmpResult != S_OK) {
		msg = GetLastError();
		DirectXRenderer::ErrorHandler(msg);
		return false;
	}

	// Release back buffer
	pBackBuffer->Release();

	// Set viewport size
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = 1280;
	Viewport.Height = 720;

	DeviceContext->RSSetViewports(1, &Viewport);

	if (tmpResult != S_OK) {
		msg = GetLastError();
		DirectXRenderer::ErrorHandler(msg);
		return false;
	}

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

	if (tmpResult != S_OK) {
		msg = GetLastError();
		DirectXRenderer::ErrorHandler(msg);
		return false;
	}

	// Set up shader
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile(L"Resources\\Shader\\Shaders.shader", 0, 0, "VShader", "vs_4_0", D3DCOMPILE_DEBUG, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(L"Resources\\Shader\\Shaders.shader", 0, 0, "PShader", "ps_4_0", D3DCOMPILE_DEBUG, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	tmpResult = Device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &VertexShader);
	if (tmpResult != S_OK) {
		msg = GetLastError();
		DirectXRenderer::ErrorHandler(msg);
		return false;
	}

	tmpResult = Device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &PixelShader);
	if (tmpResult != S_OK) {
		msg = GetLastError();
		DirectXRenderer::ErrorHandler(msg);
		return false;
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
	return true;
}

bool DirectXRenderer::Render() {
	HRESULT tmpResult;
	DWORD msg = GetLastError();

	// draw the vertex buffer to the back buffer
	DeviceContext->ClearRenderTargetView(BackBuffer, (const float*)&ClearBackBufferColor);

	DeviceContext->OMSetRenderTargets(1, &BackBuffer, nullptr);

	const UINT offset = 0;
	const UINT stride = sizeof(SVertex);

	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// select which primtive type we are using

	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;
	tmpResult = DeviceContext->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedSubresource);    // map the buffer

	if (tmpResult != S_OK) {
		msg = GetLastError();
		DirectXRenderer::ErrorHandler(msg);
		return false;
	}

	memcpy(MappedSubresource.pData, &Vertices[0], sizeof(SVertex)*Vertices.size());                 // copy the data
	DeviceContext->Unmap(VertexBuffer, NULL);

	// draw vertices
	DeviceContext->Draw(4, 0);

	tmpResult = SwapChain->Present(VSync, 0);

	if (tmpResult != S_OK) {
		msg = GetLastError();
		DirectXRenderer::ErrorHandler(msg);
		return false;
	}
	return true;
}

void DirectXRenderer::AddVertex(SVertex v) {
	if (CurrentState == DirectXRenderer::State::NOT_READY) {
		CurrentState = DirectXRenderer::State::VERTICES_ADDED;
		Vertices.clear();
	}

	Vertices.push_back(v);
}
