
#pragma comment(lib, "Framework.Core.lib")

#include <stdio.h>

#include "Application.h"

using namespace Framework;

class Game : public IGame {
public:
	virtual void PreRender();
	virtual void Update();
	virtual void PostRender();

	Game(HWND* windowHandle, int backgroundColor, Framework::Renderer::Types rendererType);
};

	void Game::PreRender() {
		printf("Pre!");
	}
	void Game::Update() {
		printf("Update!");
	}
	void Game::PostRender() {
		printf("Post!");
	}

	Game::Game(HWND* windowHandle, int backgroundColor, Framework::Renderer::Types rendererType) {
		WindowHandle = windowHandle;
		BackgroundColor = backgroundColor;
		RendererType = rendererType;

		IsRunning = true;
	}

HWND WindowHandle;
Application* application;
Game* game;

void main() {
	game = new Game(&WindowHandle, 0x00c0ffFF, Framework::Renderer::Types::DirectX11);

	application = new Application((IGame*)game);

	while (game->IsRunning) {
		application->Main();
	}

	delete application;
}