#pragma once

#include "RendererTypes.h"

namespace Framework {
	class Renderer {
	protected:
		Renderer();
		~Renderer();
	public:
		enum RendererTypes {
			DirectX10,
			DirectX11
		} CurrentRendererType;

		virtual void Begin(int ClearColor) = 0; // Pre draw
		virtual void End() = 0; // = Draw() + Present()

		static void Renderer::CreateRenderer(Renderer** destination, RendererTypes types);
	};
}