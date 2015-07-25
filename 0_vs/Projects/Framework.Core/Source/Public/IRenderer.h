#pragma once

#include "Common.h"

#include <vector>
#include "Windows.h"

#include "RendererTypes.h"

namespace Framework {
	namespace Renderer {
		enum Types {
			DirectX10,
			DirectX11
		};

		class IRenderer {
		public:
			virtual void Init() = 0; // Pre draw
			virtual void Begin(int ClearColor) = 0; // Pre draw
			virtual void End() = 0; // = Draw() + Present()
			virtual void AddVertex(SVertex v) {
				if (CurrentState == State::NOT_READY) {
					CurrentState = State::VERTICES_ADDED;
					Vertices.clear();
				}

				Vertices.push_back(v);
			}

			HWND* WindowPointer;

			Types CurrentRendererType;
			State CurrentState = State::NOT_READY;
			std::vector<SVertex> Vertices;
		};

		typedef IRenderer* (*Create)(IRenderer**);
		typedef void* (*Delete)(IRenderer**);

		void CreateRenderer(HWND** windowPointer, IRenderer** destination, Renderer::Types types);
		void DeleteRenderer(IRenderer** destination);
		bool ErrorHandler(HRESULT result);
	}
}