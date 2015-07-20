#pragma once

#include "Renderer.h"

namespace Framework {
	class DX11Renderer : public Renderer {
		DX11Renderer();
		~DX11Renderer();

		virtual void Begin(int ClearColor); // Pre draw
		virtual void End(); // = Draw() + Present()
	};
}
