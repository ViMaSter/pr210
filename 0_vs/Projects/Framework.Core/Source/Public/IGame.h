#pragma once

#include "Windows.h"

#include "IRenderer.h"

namespace Framework {
	/// <summary>
	/// Interface for scene class used to inject custom code before, while and after draw-calls
	/// </summary>
	class IGame {
	public:
		~IGame() {
			Renderer::DeleteRenderer(&CurrentRenderer);
		}

		HWND* WindowHandle;

		/// <summary>
		/// Setting this to false, will gracefully close the application, the next time the game is able to
		/// </summary>
		bool IsRunning = false;

		/// <summary>
		/// Currently active renderer type
		/// </summary>
		Framework::Renderer::Types RendererType;
		/// <summary>
		/// Associated renderer
		/// </summary>
		Framework::Renderer::IRenderer* CurrentRenderer;

		/// <summary>
		/// Background color the backbuffer is cleared with (feeds into IRenderer::Begin())
		/// </summary>
		int BackgroundColor = 0;

		virtual void PreRender() = 0;
		virtual void Update() = 0;
		virtual void PostRender() = 0;
	};
}