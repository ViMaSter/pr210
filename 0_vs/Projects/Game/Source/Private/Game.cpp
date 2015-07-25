#include "Game.h"

void Game::PreRender() {
	// Example of how to modify the vertex buffer
	CurrentRenderer->Vertices.clear();
	
	DWORD a = GetTickCount();
	CurrentRenderer->AddVertex(Framework::Renderer::SVertex(0.25f, 0.25f, 1.f, 0.5f, (float)((a / 1000) % 2), 0.0f, 1.f));
	CurrentRenderer->AddVertex(Framework::Renderer::SVertex(0.25f, 0.75f, 1.f, 0.5f, (float)((a / 1000) % 2), 0.0f, 1.f));
	CurrentRenderer->AddVertex(Framework::Renderer::SVertex(0.75f, 0.25f, 1.f, 0.5f, (float)((a / 1000) % 2), 0.0f, 1.f));
	CurrentRenderer->AddVertex(Framework::Renderer::SVertex(0.75f, 0.75f, 1.f, 0.5f, (float)((a / 1000) % 2), 0.0f, 1.f));

#ifdef FRAMEWORK_DEBUG
	std::cout << ("Pre!") << std::endl;
#endif
}
void Game::Update() {


#ifdef FRAMEWORK_DEBUG
	std::cout << ("Update!") << std::endl;
#endif
}
void Game::PostRender() {


#ifdef FRAMEWORK_DEBUG
	std::cout << ("Post!") << std::endl;
#endif
}

Game::Game(HWND* windowHandle, int backgroundColor, Framework::Renderer::Types rendererType) {
	WindowHandle = windowHandle;
	BackgroundColor = backgroundColor;
	RendererType = rendererType;

	IsRunning = true;
}