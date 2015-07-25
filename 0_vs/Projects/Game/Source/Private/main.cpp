// Running in debug-config sets FRAMEWORK_DEBUG (see Common.h in Framework.Core for more info)

#pragma region Framework
// Framework
#pragma comment(lib, "Framework.Core.lib")
#include "Application.h"

// Specific Game-/Scene-class
#include "Game.h"

using namespace Framework;

Application* application;
IGame* game;
#pragma endregion

#pragma region Debug helper
#ifdef FRAMEWORK_DEBUG
// Setup console
#include <io.h>
#include <fcntl.h>
#include <ios>
#include <windows.h>
void RedirectIOToConsole() {
	int ConsoleHandle;
	int StdHandle;
	FILE* ConsolePointer;

	// allocate a console for this app
	AllocConsole();

	// redirect unbuffered STDOUT to the console
	StdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleHandle = _open_osfhandle(StdHandle, _O_TEXT);

	ConsolePointer = _fdopen(ConsoleHandle, "w");
	*stdout = *ConsolePointer;
	setvbuf(stdout, NULL, _IONBF, 0);


	// redirect unbuffered STDERR to the console
	StdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	ConsoleHandle = _open_osfhandle(StdHandle, _O_TEXT);

	ConsolePointer = _fdopen(ConsoleHandle, "w");
	*stderr = *ConsolePointer;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();
}
#endif
#pragma endregion

#pragma region Windows Core
// Forward declarations
ATOM				RegisterWindowClass();
BOOL				InitializeWindowInstance(int);
LRESULT CALLBACK	WindowMessageProcedure(HWND, UINT, WPARAM, LPARAM);

// Global variables
/// <summary>
/// Handle to this processes instance
/// </summary>
HINSTANCE InstanceHandle;
/// <summary>
/// Handle to the main window
/// </summary>
HWND WindowHandle;

/// <summary>
/// Registers the window class
/// </summary>
/// <returns>Unique identifier of registred class</returns>
ATOM RegisterWindowClass() {
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowMessageProcedure;
	wcex.hInstance = InstanceHandle;
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = "PR210_3DENGINE";
	wcex.lpszClassName = "PR210_3DENGINE";

	return RegisterClassEx(&wcex);
}

/// <summary>
/// Creates the main window
/// </summary>
/// <param name="nCmdShow">Settings for <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/ms633548%28v=vs.85%29.aspx">ShowWindow()</a>.</param>
/// <returns>Returns whether or not <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/ms632679%28v=vs.85%29.aspx">CreateWindow()</a> succeeded</returns>
BOOL InitializeWindowInstance(int displaySettings) {
	WindowHandle = CreateWindow("PR210_3DENGINE", "Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, InstanceHandle, NULL);

	if (!WindowHandle) {
		return FALSE;
	}

	ShowWindow(WindowHandle, displaySettings);
	UpdateWindow(WindowHandle);

	return TRUE;
}

/// <summary>
/// Processes the messages for the window
/// </summary>
/// <param name="hWnd">The associated window handle</param>
/// <param name="message">Sent message</param>
/// <param name="wParam">Additional parameter sent to message</param>
/// <param name="lParam">Additional parameter sent to message</param>
/// <returns></returns>
LRESULT CALLBACK WindowMessageProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
		case WM_PAINT:
			hdc = BeginPaint(windowHandle, &ps);

			application->Main();

			// @TODO: Run render loop

			EndPaint(windowHandle, &ps);
			InvalidateRect(windowHandle, NULL, TRUE);
			break;
		case WM_KEYDOWN:
			switch (wParam) {
			case 0x43:
				PostMessage(WindowHandle, WM_QUIT, 0, 0);
				break;
			}
			break;
		case WM_DESTROY:
			game->IsRunning = false;
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(windowHandle, message, wParam, lParam);
	}
	return 0;
}
#pragma endregion

/// <summary>
/// Entry point for the application
/// 
/// More info can be found at https://msdn.microsoft.com/en-us/library/windows/desktop/ms633559(v=vs.85).aspx
/// </summary>
/// <param name="instanceHandle">This processes instance handle</param>
/// <param name="previousInstanceHandle">Always NULL - used to find already existing HINSTANCE's of this process</param>
/// <param name="commandLineArguments">Additional command line arguments used when the programm was started</param>
/// <param name="displaySettings">Display settings (start mini-/maximized, etc.)</param>
/// <returns></returns>
int APIENTRY WinMain(HINSTANCE instanceHandle, HINSTANCE previousInstanceHandle, LPTSTR commandLineArguments, int displaySettings) {
	InstanceHandle = InstanceHandle;

	// Initialize global strings
	RegisterWindowClass();

	// Perform application initialization:
	if (!InitializeWindowInstance(displaySettings)) {
		return FALSE;
	}

#ifdef FRAMEWORK_DEBUG
	RedirectIOToConsole();
#endif

	// INIT HERE!
#pragma region Framework
	game = new Game(&WindowHandle, 0x00c0ffFF, Framework::Renderer::Types::DirectX11);

	application = new Application(&game);
#pragma endregion

	// Main message loop
	MSG message;

	while (game->IsRunning) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		switch (message.message) {
		case WM_KEYDOWN:
			switch (message.wParam) {
			case VK_ESCAPE:
				game->IsRunning = false;
				break;
			case 'F':
				//SwitchRenderer(GraphicsModuleType::DirectX10);
				break;
			case 'G':
				//SwitchRenderer(GraphicsModuleType::DirectX11);
				break;
			}
			break;
		case WM_QUIT:
			if (message.message == WM_QUIT)
				game->IsRunning = false;
			break;
		}

			


		application->Main();
	}

	// CLEAN UP HERE!
	delete application;

	// Send end message
	return (int)message.wParam;
}