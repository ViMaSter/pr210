#include "Application.h"

namespace Framework {
	Application::Application(IGame** game) {
		// Activating the /02-compiler flag breaks this
		Game = (IGame*)*game;

		Framework::Renderer::CreateRenderer(&Game->WindowHandle, &(Game->CurrentRenderer), Game->RendererType);
	}

	Application::~Application() {

	}

	void Application::Main() {
		Game->PreRender();

		Game->CurrentRenderer->Begin(Game->BackgroundColor);

		Game->Update();

		Game->CurrentRenderer->End();

		Game->PostRender();
	}
}