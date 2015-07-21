#pragma once

namespace Framework {
	namespace Renderer {
#pragma region Position
		typedef float TPosition[3];
		struct SPosition {
			float X;
			float Y;
			float Z;

			SPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
				X = x;
				Y = y;
				Z = z;
			}
		};
#pragma endregion

#pragma region Color
		typedef float TColor[4];
		/// Color ranges from 0.0f to 1.0f
		struct SColor {
			float R;
			float G;
			float B;
			float A;

			SColor(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) {
				R = r;
				G = g;
				B = b;
				A = a;
			}

			operator int() {
				int result = 0;
				result ^= (int)(R * 255) << 6;
				result ^= (int)(G * 255) << 4;
				result ^= (int)(B * 255) << 2;
				result ^= (int)(A * 255) << 0;
				return result;
			}
		};
#pragma endregion

#pragma region Vertex
		struct SVertex {
			SPosition Position;
			SColor Color;

			SVertex(float X = 0.0f, float Y = 0.0f, float Z = 0.0f, float R = 0.0f, float G = 0.0f, float B = 0.0f, float A = 1.0f) {
				Position = SPosition(X, Y, Z);
				Color = SColor(R, G, B, A);
			}
		};
#pragma endregion

		enum State {
			NOT_READY,
			INITIALIZED,
			VERTICES_ADDED
		};
	}
}