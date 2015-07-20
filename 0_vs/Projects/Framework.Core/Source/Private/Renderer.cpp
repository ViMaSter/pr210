#include "Renderer.h"

namespace Framework {
	void Renderer::CreateRenderer(Renderer** destination, RendererTypes types) {
		switch (types) {
			case RendererTypes::DirectX10:
				// Init DX10
				*destination = (Renderer*)0x10101010;
				break;
			case RendererTypes::DirectX11:
				// Init DX11
				*destination = (Renderer*)0x11111111;
				break;
		}
	}
}