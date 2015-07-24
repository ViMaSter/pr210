#pragma once

#include <stdio.h>

#include "IRenderer.h"

namespace Framework {
	namespace Renderer {
		namespace DX11Renderer {
			class Renderer : public IRenderer {
			public:
				Renderer() {
					CurrentRendererType = Framework::Renderer::Types::DirectX11;
				}
				~Renderer() {
					
				}

				virtual void Begin(int ClearColor) {
					// Post PreDraw - PreUpdate
					printf("X11 Begin: ");
				}
				virtual void End() {
					// Pre Update - Pre PostDraw
					printf("X11 End: ");
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
