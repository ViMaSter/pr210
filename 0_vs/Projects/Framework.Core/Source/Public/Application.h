#pragma once

#include "Windows.h"

#include "IGame.h"

namespace Framework {
	class Application {
	public:
		IGame* Game = 0;

		void Main();

		Application(IGame** game);
		~Application();
	};
}