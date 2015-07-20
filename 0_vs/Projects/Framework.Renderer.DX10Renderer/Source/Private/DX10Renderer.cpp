#pragma once

#include "Renderer.h"

namespace Framework {
	class DX10Renderer : public Renderer {
		DX10Renderer() {

		}
		~DX10Renderer() {
			Renderer::~Renderer();
		}

		virtual void Begin(int ClearColor) {
			// Pre draw
		}
		virtual void End() {
			// = Draw() + Present()
		}
	};
}
