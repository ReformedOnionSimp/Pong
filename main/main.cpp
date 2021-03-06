#include <Windows.h>
#include "Graphics.h"
#include "Pong.h"
#include "Pong2.h"
#include "Snake.h"
#include "GameController.h"

constexpr auto PONG = 1;
constexpr auto PONG2 = 2;

Graphics* graphics;

HMENU hMenu;

void AddMenus(HWND);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = "MainWindow";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);

	RECT rect = { 0, 0, 800, 600 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	HWND windowHandle = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,"MainWindow", "Arcadia", WS_OVERLAPPEDWINDOW, 100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL
	);

	if (!windowHandle)
		return -1;

	graphics = new Graphics();

	if (!graphics->Init(windowHandle)) 
	{
		delete graphics;
		return -1;
	}

	GameLevel::Init(graphics);

	ShowWindow(windowHandle, nShowCmd);

	GameController::Init();
	// GameController::LoadInitialLevel(new Level1());
	// GameController::LoadInitialLevel(new Pong());
	GameController::LoadInitialLevel(new Snake());

	float y = 0.0;
	float ySpeed = 0.0;

	MSG message;
	message.message = NULL;

	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&message);
		else
		{
			GameController::Update();
			graphics->BeginDraw();
			GameController::Render();
			graphics->EndDraw();
		}
	}

	/*
	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL) > 0) {
		DispatchMessage(&message);
	}
	
	delete graphics;
	*/

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_CREATE:
		AddMenus(hwnd);
		break; 

	case WM_COMMAND:
		switch (wParam)
		{
		case PONG:
			GameController::SwitchLevel(new Pong());
			break;
		case PONG2:
			GameController::SwitchLevel(new Pong2());
			break;
		default:
			break;
		}
		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case 0x57:
			GameController::Update(0x57);
			break;
		case 0x53:
			GameController::Update(0x53);
			break;
		case VK_UP:
			GameController::Update(VK_UP);
			break;
		case VK_DOWN:
			GameController::Update(VK_DOWN);
			break;
		default:
			break;
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	/* case WM_PAINT:
		graphics->BeginDraw();
		graphics->ClearScreen(0.f, 0.0f, 0.5f);
		//Spam circles
		for (int i = 0; i < 1000; i++)
		{
			graphics->DrawCircle(
				rand() % 800,
				rand() % 600,
				rand() % 100,
				(rand() % 100) / 100.0f,
				(rand() % 100) / 100.0f,
				(rand() % 100) / 100.0f,
				(rand() % 100) / 100.0f
			);

		}
		graphics->EndDraw();

	case WM_TIMER:
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	*/

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

// Menu

void AddMenus(HWND hWnd)
{
	hMenu = CreateMenu();
	HMENU hPongMenu = CreateMenu();

	HMENU Pong = CreateMenu();

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPongMenu, "Pong");

	AppendMenu(hPongMenu, MF_STRING, PONG, "Single Player");
	AppendMenu(hPongMenu, MF_STRING, PONG2, "Multi Player");

	SetMenu(hWnd, hMenu);
}