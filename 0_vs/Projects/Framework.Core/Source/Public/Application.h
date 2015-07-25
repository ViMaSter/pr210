#pragma once

#include "Windows.h"

#include "IGame.h"

namespace Framework {
	/// <summary>
	/// Application that launches "games" deriving from IGame
	/// </summary>
	class Application {
	public:
		IGame* Game = 0;

		/// <summary>
		/// Coordinates renderer and game-hooks
		/// </summary>
		void Main();

		/// <summary>
		/// Switch the renderer associated with this instance
		/// </summary>
		void SwitchRenderer(Framework::Renderer::Types newType);

		Application();
		Application(IGame** game);
		~Application();
	};
}