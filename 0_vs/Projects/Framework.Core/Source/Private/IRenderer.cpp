#include <Windows.h>

#include "IRenderer.h"

namespace Framework {
	namespace Renderer {
		void CreateRenderer(HWND** windowPointer, IRenderer** destination, Renderer::Types types) {
			HMODULE graphicsModule = 0;
			Framework::Renderer::Create createFuncPtr;

			switch (types) {
			case Renderer::Types::DirectX10:
				// Init DX10
				graphicsModule = LoadLibrary("Framework.Renderer.DX10Renderer.dll");
				createFuncPtr = (Framework::Renderer::Create)GetProcAddress(graphicsModule, "CreateRenderer");
				createFuncPtr(destination);
				break;
			case Renderer::Types::DirectX11:
				// Init DX11
				graphicsModule = LoadLibrary("Framework.Renderer.DX11Renderer.dll");
				createFuncPtr = (Framework::Renderer::Create)GetProcAddress(graphicsModule, "CreateRenderer");
				createFuncPtr(destination);
				break;
			}

			(*destination)->WindowPointer = *windowPointer;

			(*destination)->Init();
		}

		void DeleteRenderer(IRenderer** destination) {
			HMODULE graphicsModule = 0;
			Framework::Renderer::Delete deleteFuncPtr;

			switch ((*destination)->CurrentRendererType) {
			case Renderer::Types::DirectX10:
				// Init DX10
				graphicsModule = LoadLibrary("Framework.Renderer.DX10Renderer.dll");
				deleteFuncPtr = (Framework::Renderer::Delete)GetProcAddress(graphicsModule, "DeleteRenderer");
				deleteFuncPtr(destination);
				break;
			case Renderer::Types::DirectX11:
				// Init DX11
				graphicsModule = LoadLibrary("Framework.Renderer.DX11Renderer.dll");
				deleteFuncPtr = (Framework::Renderer::Delete)GetProcAddress(graphicsModule, "DeleteRenderer");
				deleteFuncPtr(destination);
				break;
			}
		}
	}
}