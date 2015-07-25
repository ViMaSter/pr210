#pragma once

#include "IRenderer.h"

namespace Framework {
	namespace Renderer {
		namespace DX10Renderer {
			class Renderer : public IRenderer {
			public:
				Renderer();
				~Renderer();

				virtual void Init(); // Pre draw
				virtual void Begin(int ClearColor); // Pre draw
				virtual void End(); // = Draw() + Present()
				virtual void AddVertex(SVertex v);
			};

			IRenderer* CreateRenderer(IRenderer** destination);
			void DeleteRenderer(IRenderer** origin);
		}
	}
}
