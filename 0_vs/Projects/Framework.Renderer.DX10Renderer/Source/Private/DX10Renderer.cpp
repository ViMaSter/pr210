#pragma once

#include <stdio.h>

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
					// Post PreDraw - PreUpdate
					printf("X10 Begin: ");
				}
				virtual void End() {
					// Pre Update - Pre PostDraw
					printf("X10 End: ");
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