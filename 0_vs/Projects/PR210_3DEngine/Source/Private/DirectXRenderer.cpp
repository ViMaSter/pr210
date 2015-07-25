#include "stdafx.h"
#include "DirectXRenderer.h"

DirectXRenderer::DirectXRenderer() {
	ClearBackBufferColor = SColor(0.0f, 0.75f, 1.0f);
	Vertices.push_back(SVertex());
}

DirectXRenderer::~DirectXRenderer() {

}

void DirectXRenderer::ErrorHandler(int ID) {
#ifdef FRAMEWORK_DEBUG
	std::cout << "Error id: " << ID << std::endl;
#endif
}

bool DirectXRenderer::Init(HWND* windowPointer) {
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
	if (CurrentState == State::NOT_READY) {
		CurrentState = State::VERTICES_ADDED;
		Vertices.clear();
	}

	Vertices.push_back(v);
}
