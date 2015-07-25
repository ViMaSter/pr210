#pragma once

#include "Common.h"

#include <vector>
#include "Windows.h"

#include "RendererTypes.h"

namespace Framework {
	namespace Renderer {
		/// <summary>
		/// List of all currently supported Renderer-Types
		/// </summary>
		enum Types {
			DirectX10,
			DirectX11
		};

		/// <summary>
		/// General interface for a renderer
		/// 
		/// All renderer have support for a vertex buffer using the SVertex-layout
		/// Additionally, more customized draw-calls can be sent to the renderer
		/// by overloading IGame::Update()
		/// </summary>
		class IRenderer {
		public:
			virtual ~IRenderer() {}

			/// <summary>
			/// This function is ran after the construction, but before the first draw occurs.
			/// The first draw call will be initiated automatically and shouldn't be called from here
			/// </summary>
			virtual void Init() = 0;


			/// <summary>
			/// Used to prepare the renderer for new input
			/// </summary>
			/// <param name="ClearColor">Color to clear the backbuffer with</param>
			virtual void Begin(int ClearColor) = 0;

			/// <summary>
			/// Draws and presents the received draw-calls
			/// </summary>
			virtual void End() = 0;

			/// <summary>
			/// Helper function to add vertex-data.
			/// </summary>
			/// <param name="v">The vertex to add</param>
			virtual void AddVertex(SVertex v) {
				if (CurrentState == State::NOT_READY) {
					CurrentState = State::VERTICES_ADDED;
					Vertices.clear();
				}

				Vertices.push_back(v);
			}

			/// <summary>
			/// The window pointer associated with this instance of a renderer
			/// </summary>
			HWND* WindowPointer;

			/// <summary>
			/// Determines, how many vertical syncs have to occur, before the backbuffer is presented (see https://msdn.microsoft.com/en-us/library/windows/desktop/bb174576(v=vs.85).aspx - Parameters: SyncInterval)
			/// </summary>
			int VSync = 0;

			/// <summary>
			/// The current renderer type of this instance
			/// </summary>
			Types CurrentRendererType;
			/// <summary>
			/// The current renderer state of this instance
			/// </summary>
			State CurrentState = State::NOT_READY;
			/// <summary>
			/// Direct access to the vertex-data
			/// </summary>
			std::vector<SVertex> Vertices;
		};

		typedef IRenderer* (*Create)(IRenderer**);
		typedef void* (*Delete)(IRenderer**);

		/// <summary>
		/// Creates a renderer
		/// </summary>
		/// <param name="windowPointer">The window pointer to associate with the new renderer</param>
		/// <param name="destination">Pointer to the location of the new renderer</param>
		/// <param name="types">Which renderer to use</param>
		void CreateRenderer(HWND** windowPointer, IRenderer** destination, Renderer::Types types);
		/// <summary>
		/// Deletes a renderer
		/// </summary>
		/// <param name="destination">Renderer to delete</param>
		void DeleteRenderer(IRenderer** destination);

		/// <summary>
		/// Customizable helper-snipped for easier error handling
		/// </summary>
		/// <param name="result">Result to handle</param>
		/// <returns></returns>
		bool ErrorHandler(HRESULT result);
	}
}