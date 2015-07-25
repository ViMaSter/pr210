#include "stdafx.h"
#include "DirectX10Renderer.h"

DirectX10Renderer::DirectX10Renderer() {
	ClearBackBufferColor = SColor(0.0f, 0.75f, 1.0f);
	Vertices.push_back(SVertex());
}

DirectX10Renderer::~DirectX10Renderer() {

}

void DirectX10Renderer::ErrorHandler(int ID) {
#ifdef FRAMEWORK_DEBUG
	std::cout << "Error id: " << ID << std::endl;
#endif
}

bool DirectX10Renderer::Init(HWND* windowPointer) {
	// Set object state
	CurrentState = State::INITIALIZED;

	// Temporary result value
	HRESULT tmpResult;
	DWORD msg = GetLastError();

	this->device = NULL;
	this->backBufferView = NULL;
	this->depthStencilBufferView = NULL;
	this->swapChain = NULL;

	IDXGIDevice* dxgiDevice;
	IDXGIFactory* factory = NULL;
	IDXGIAdapter* adapter = NULL;
	ID3D10Texture2D* backBuffer;
	ID3D10Texture2D* depthStencilBuffer;
	D3D10_VIEWPORT viewPort;
	D3D10_TEXTURE2D_DESC depthStencilDescription;
	DXGI_SWAP_CHAIN_DESC swapChainDescription;

	D3D_FEATURE_LEVEL featureLevels[1] = { D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_0 };

	tmpResult = D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE::D3D10_DRIVER_TYPE_HARDWARE, NULL,
		D3D10_CREATE_DEVICE_SINGLETHREADED | D3D10_CREATE_DEVICE_BGRA_SUPPORT, D3D10_SDK_VERSION, &this->device);

	tmpResult = this->device->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&dxgiDevice);

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
	swapChainDescription.OutputWindow = *windowPointer;
	swapChainDescription.Windowed = true;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescription.Flags = 0;

	tmpResult = factory->CreateSwapChain(this->device, &swapChainDescription, &this->swapChain);

	tmpResult = this->swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBuffer);

	tmpResult = this->device->CreateRenderTargetView(backBuffer, NULL, &this->backBufferView);





	// Set up shader
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DX10CompileFromFile(L"Resources\\Shader\\Shaders.shader", 0, 0, "VShader", "vs_4_0", D3DCOMPILE_DEBUG, 0, 0, &VS, 0, 0);
	D3DX10CompileFromFile(L"Resources\\Shader\\Shaders.shader", 0, 0, "PShader", "ps_4_0", D3DCOMPILE_DEBUG, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	tmpResult = this->device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), &VertexShader);
	if (tmpResult != S_OK) {
		msg = GetLastError();
		return false;
	}

	tmpResult = this->device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), &PixelShader);
	if (tmpResult != S_OK) {	
		msg = GetLastError();
		return false;
	}

	this->device->VSSetShader(VertexShader);
	this->device->PSSetShader(PixelShader);

	D3D10_INPUT_ELEMENT_DESC InputElementDescription[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	this->device->CreateInputLayout(InputElementDescription, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &InputLayout);
	this->device->IASetInputLayout(InputLayout);



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

	tmpResult = this->device->CreateBuffer(&bd, &InitData, &VertexBuffer);





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

	tmpResult = this->device->CreateTexture2D(&depthStencilDescription, 0, &depthStencilBuffer);

	tmpResult = this->device->CreateDepthStencilView(depthStencilBuffer, 0, &this->depthStencilBufferView);

	viewPort.Width = 1280;
	viewPort.Height = 720;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	this->device->RSSetViewports(1, &viewPort);

	return true;
}

bool DirectX10Renderer::Render() {
	this->device->ClearRenderTargetView(this->backBufferView, (const float*)&ClearBackBufferColor);

	this->device->OMSetRenderTargets(1, &this->backBufferView, nullptr);

	const UINT offset = 0;
	const UINT stride = sizeof(SVertex);

	this->device->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	this->device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	this->device->Draw(0, 0);

	this->swapChain->Present(0, 0);
	return true;
}

void DirectX10Renderer::AddVertex(SVertex v) {
	if (CurrentState == State::NOT_READY) {
		CurrentState = State::VERTICES_ADDED;
		Vertices.clear();
	}

	Vertices.push_back(v);
}
