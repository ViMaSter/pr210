#include "Common.h"

#include "IGame.h"

#include <stdio.h>

using namespace Framework;

/// <summary>
/// Example class to demonstrate custom-code injection
/// </summary>
class Game : public IGame {
public:
	virtual void PreRender();
	virtual void Update();
	virtual void PostRender();

	Game(HWND* windowHandle, int backgroundColor, Framework::Renderer::Types rendererType);
};