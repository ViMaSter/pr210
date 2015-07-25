#pragma once

#include "Windows.h"

#include "IGame.h"

namespace Framework {
	class Application {
	public:
		IGame* Game = 0;

		void Main();

		Application();
		Application(IGame** game);
		~Application();
	};
}