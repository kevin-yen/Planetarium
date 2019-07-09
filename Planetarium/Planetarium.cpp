// Planetarium.cpp

// Main window routine and procedures

#include <windows.h>

#include "Planetarium.h"
#include "WinOpenGl.h"
#include "WinImpFunc.h"
#include "OpenGlMain.h"

// Our device handle.
HDC			hDC				= NULL;
HWND		hWnd;

// Initial width and height of the window.
const int	wndHeight		= 512;
const int	wndWidth		= 512;

// Arrary for coorsponding key on the keyboard.
bool keys[256];

// WinProc :	Message handle, or window procedure.
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch(uMessage)
	{
	
	// Procedure for menu bar.
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{

		case ID_FILE_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;

		case ID_HELP_ABOUT:
			DialogBox(GetModuleHandle(NULL),
				MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutDialogProc);
			break;
		}
		break;

	// Procedure for user input on the keyboard.
	// If the user press a key the coorsponding
	//  bool value (true, false) array is true-
	case WM_KEYDOWN:
		keys[wParam] = TRUE;
		break;

	// If the user press a key the coorsponding
	//    bool value (true, false) array is false-
	case WM_KEYUP:
		keys[wParam] = FALSE;
		break;

	// Procedure for window resizing.
	case WM_SIZE:
		giResize(HIWORD(lParam), LOWORD(lParam));
		break;

	// Procedure for closing and destroying window.
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	// Don't care about messages we don't handle.
	default:
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	return 0;
}

// WinMain :	Main function for a window 32 program.
int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, 
				   int nCmdShow)
{
	MSG uMessage;
	BOOL fQuit = FALSE;
	glContext glCurrent;
	bool* pfKey;
	pfKey = keys;

	fQuit = !(CreateWnd(&hWnd, hInstance, "Planetarium 101", 
						wndWidth, wndHeight,
						16, false));

	hDC = glCurrent.init(hWnd, 16);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	giInit( );

	while(fQuit != TRUE)
	{
		// There's a message.
		if(PeekMessage(&uMessage, NULL, 0, 0, PM_REMOVE))
		{
			// The message is quit.
			if(uMessage.message == WM_QUIT)
				fQuit = TRUE;
			else
			{
				TranslateMessage(&uMessage);
				DispatchMessage(&uMessage);
			}
		}

		// There's no message.
		else
		{
			// Render all graphics here and display the renderings.
			giKeyProc(pfKey);
			GraphicsMain();
			SwapBuffers(hDC);
		}
	}
	return uMessage.wParam;
}