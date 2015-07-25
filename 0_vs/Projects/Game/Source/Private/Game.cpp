#include "Game.h"

void Game::PreRender() {


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