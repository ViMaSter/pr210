#pragma once

#include "IRenderer.h"

namespace Framework {
	namespace Renderer {
		namespace DX10Renderer {
			class Renderer : public IRenderer {
			public:
				Renderer();
				~Renderer();

				virtual void Begin(int ClearColor); // Pre draw
				virtual void End(); // = Draw() + Present()
			};

			IRenderer* CreateRenderer(IRenderer** destination);
			void DeleteRenderer(IRenderer** origin);
		}
	}
}
