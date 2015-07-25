#include "Application.h"

namespace Framework {
	Application::Application(IGame** game) {
		// Activating the /02-compiler flag breaks this
		Game = (IGame*)*game;

		Framework::Renderer::CreateRenderer(&Game->WindowHandle, &(Game->CurrentRenderer), Game->RendererType);
	}

	Application::~Application() {
		delete Game;
	}

	void Application::SwitchRenderer(Framework::Renderer::Types newType) {
		Framework::Renderer::DeleteRenderer(&(Game->CurrentRenderer));

		Framework::Renderer::CreateRenderer(&Game->WindowHandle, &(Game->CurrentRenderer), newType);
	}

	void Application::Main() {
		Game->PreRender();

		Game->CurrentRenderer->Begin(Game->BackgroundColor);

		Game->Update();

		Game->CurrentRenderer->End();

		Game->PostRender();
	}
}