#pragma once

#include "RendererTypes.h"

namespace Framework {
	namespace Renderer {
		enum Types {
			DirectX10,
			DirectX11
		};

		class IRenderer {
		public:
			virtual void Begin(int ClearColor) = 0; // Pre draw
			virtual void End() = 0; // = Draw() + Present()

			Types CurrentRendererType;
		};

		typedef IRenderer* (*Create)(IRenderer**);
		typedef void* (*Delete)(IRenderer**);

		void CreateRenderer(IRenderer** destination, Renderer::Types types);
		void DeleteRenderer(IRenderer** destination);
	}
}