#include"myWindow.h"



LRESULT CALLBACK defaultCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	myWindow* window = (myWindow*)GetWindowLongPtr(hWnd, 0);
	switch (msg) {
	case WM_SIZE:
		window->width = LOWORD(lParam);
		window->height = HIWORD(lParam);
		window->changeSize = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}


myWindow::myWindow(const char* WindowName, int Width, int Height)
{
	width = Width;
	height = Height;
	if (strlen(WindowName) >= 64)
	{
		MessageBox(NULL, TEXT("WindowName is too long, set it to 'Simple Demo' "), NULL, MB_OK);
		strcpy_s(winName, "Simple Demo");
	}
	else
	    strcpy_s(winName, WindowName);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = defaultCallback;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = sizeof(void*);
	wndClass.hInstance = NULL;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WindowName;


	if (!RegisterClass(&wndClass)) 
	{
		MessageBox(NULL, TEXT("Failed to create!"), WindowName, MB_ICONERROR);
		return;
	}
	
	hwnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, NULL, NULL);
	SetWindowLongPtr(hwnd, 0, (long)this);

}


void myWindow::show()
{
	ShowWindow(hwnd, SW_SHOW);
}

myWindow::~myWindow()
{
	DestroyWindow(hwnd);
	UnregisterClass(winName, NULL);
}

HWND myWindow::getMyWindow()
{
	return this->hwnd;
}