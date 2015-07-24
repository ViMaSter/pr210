#pragma once

#include "Windows.h"

#include "IRenderer.h"

namespace Framework {
	class IGame {
	public:
		HWND* WindowHandle;
		bool IsRunning = false;

		Framework::Renderer::Types RendererType;
		Framework::Renderer::IRenderer* CurrentRenderer;

		int BackgroundColor = 0;

		virtual void PreRender() = 0;
		virtual void Update() = 0;
		virtual void PostRender() = 0;
	};
}