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
#ifdef FRAMEWORK_DEBUG
					std::cout << "X11 Begin: " << std::endl;
#endif
				}
				virtual void End() {
					// Pre Update - Pre PostDraw
#ifdef FRAMEWORK_DEBUG
					std::cout << "X11 End: " << std::endl;
#endif
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