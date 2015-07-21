#include <stdio.h>

#include "IRenderer.h"

#pragma comment(lib, "Framework.Core.lib")

Framework::Renderer::IRenderer* currentRenderer;

void main() {
	printf("a");
	Framework::Renderer::CreateRenderer(&currentRenderer, Framework::Renderer::Types::DirectX10);
	printf("b");
	Framework::Renderer::DeleteRenderer(&currentRenderer);
	printf("c");
	Framework::Renderer::CreateRenderer(&currentRenderer, Framework::Renderer::Types::DirectX11);
	printf("d");
	Framework::Renderer::DeleteRenderer(&currentRenderer);
	printf("e");
}