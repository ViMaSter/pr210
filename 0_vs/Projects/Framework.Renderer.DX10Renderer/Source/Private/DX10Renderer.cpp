#pragma once

#include "IRenderer.h"

namespace Framework {
	namespace Renderer {
		namespace DX10Renderer {
			class Renderer : public IRenderer {
			public:
				Renderer() {
					CurrentRendererType = Framework::Renderer::Types::DirectX10;
				}
				~Renderer() {
					
				}

				virtual void Begin(int ClearColor) {
					// Pre draw
				}
				virtual void End() {
					// = Draw() + Present()
				}
			};

			IRenderer* CreateRenderer(IRenderer** destination) {
				*destination = new Renderer();

				return *destination;
			}

			void DeleteRenderer(IRenderer** origin) {
				if (*origin != nullptr) {
					Renderer* originalPointer = (Renderer*)*origin;
					delete originalPointer;
					*origin = 0;
				}
			}
		}
	}

}