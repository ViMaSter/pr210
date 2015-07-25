#pragma once

#include <stdio.h>

#include "IRenderer.h"

namespace Framework {
	namespace Renderer {
		namespace DX10Renderer {
			bool ErrorHandler(HRESULT result) {
				if (result != S_OK) {
					DWORD msg = GetLastError();
#ifdef FRAMEWORK_DEBUG
					std::cout << "Error id: " << msg << std::endl;
#endif
					return true;
				}
				return false;
			}

			class Renderer : public IRenderer {
			public:
				Renderer() {
					CurrentRendererType = Framework::Renderer::Types::DirectX10;
				}
				~Renderer() {
					
				}

				virtual void Init() {
					// Post PreDraw - PreUpdate
#ifdef FRAMEWORK_DEBUG
					std::cout << "X10 Init" << std::endl;
#endif
				}

				virtual void Begin(int ClearColor) {
					// Post PreDraw - PreUpdate
#ifdef FRAMEWORK_DEBUG
					std::cout << "X10 Begin" << std::endl;
#endif
				}
				virtual void End() {
					// Pre Update - Pre PostDraw
#ifdef FRAMEWORK_DEBUG
					std::cout << "X10 End" << std::endl;
#endif
				}

				void AddVertex(SVertex v) {
					if (CurrentState == State::NOT_READY) {
						CurrentState = State::VERTICES_ADDED;
						Vertices.clear();
					}

					Vertices.push_back(v);
				}
			};

			IRenderer* CreateRenderer(IRenderer** destination) {
				//*destination = new Renderer();

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