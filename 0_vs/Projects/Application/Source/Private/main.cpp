#include <stdio.h>

#include "Renderer.h"

#pragma comment(lib, "Framework.Core.lib")

Framework::Renderer* currentRenderer;

void main() {
	printf("a");
	Framework::Renderer::CreateRenderer(&currentRenderer, Framework::Renderer::DirectX10);
	printf("b");
	Framework::Renderer::CreateRenderer(&currentRenderer, Framework::Renderer::DirectX11);
	printf("c");
}