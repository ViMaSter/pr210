// PR210_3DEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

// Resource file
#include "../../Resources/Resources/MainResources.h"

// DX bridge
#include "DirectXRenderer.h"

// Forward declarations
ATOM				MyRegisterClass();
BOOL				InitInstance(int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// Global variables
HINSTANCE hInst;
HACCEL hAccelTable;
HWND hWnd;

DirectXRenderer Renderer;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	hInst = hInstance;

	// Initialize global strings
	MyRegisterClass();

	// Perform application initialization:
	if (!InitInstance (nCmdShow))
	{
		return FALSE;
	}

	// DX Things
	Renderer.AddVertex(SVertex(0.25f, 0.25f, 1.f, 1.0f, 0.0f, 0.0f, 1.f));
	Renderer.AddVertex(SVertex(0.25f, 0.75f, 1.f, 0.0f, 0.0f, 1.0f, 1.f));
	Renderer.AddVertex(SVertex(0.75f, 0.25f, 1.f, 0.0f, 1.0f, 0.0f, 1.f));
	Renderer.AddVertex(SVertex(0.75f, 0.75f, 1.f, 1.0f, 1.0f, 1.0f, 1.f));
	Renderer.Init(&hWnd);
	Renderer.Render();

	hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE("PR210_3DENGINE"));

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			Renderer.Render();
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

/// <summary>
/// Registers the window class
/// </summary>
/// <returns>Unique identifier of registred class</returns>
ATOM MyRegisterClass()
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hIconSm		= LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= L"PR210_3DENGINE";
	wcex.lpszClassName	= L"PR210_3DENGINE";

	return RegisterClassEx(&wcex);
}

/// <summary>
/// Creates the main window
/// </summary>
/// <param name="nCmdShow">Settings for <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/ms633548%28v=vs.85%29.aspx">ShowWindow()</a>.</param>
/// <returns>Returns whether or not <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/ms632679%28v=vs.85%29.aspx">CreateWindow()</a> succeeded</returns>
BOOL InitInstance(int nCmdShow)
{
	hWnd = CreateWindow(L"PR210_3DENGINE", L"Window", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// @TODO: Run render loop

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}