#include"myWindow.h"
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>

LRESULT CALLBACK defaultCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	myWindow* window = (myWindow*)GetWindowLongPtr(hWnd, 0);

	switch (msg) {
	case WM_SIZE:
		window->width = LOWORD(lParam);
		window->height = HIWORD(lParam);
		window->changeSize = true;
		break;
	case WM_LBUTTONDOWN:
		window->bottonClickDown(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		window->bottonClickUp();
		break;
	case WM_MOUSEMOVE:
	{
		int x, y;
		static int x_pre = -1;
		static int y_pre = -1;
		if (window->getClickedPosition(x, y))
		{
			int position_x = LOWORD(lParam);
			int position_y = HIWORD(lParam);
			if (x_pre == -1 && y_pre == -1)
			{
				x_pre = position_x;
				y_pre = position_y;
				break;
			}
			float vertical = y_pre - position_y;
			float horizen = x_pre - position_x;
			float angle_horizen = horizen / ((float)window->width / 2)*90.0; // 90 degree per half screen
			float angle_vertical = vertical / ((float)window->height / 2)*60.0; // 60 degree per half screen
			glm::mat4 roatation = glm::mat4(1.0);
			roatation = glm::rotate(roatation, glm::radians(angle_horizen), glm::vec3(0.0, 1.0, 0.0));
			roatation = glm::rotate(roatation, glm::radians(angle_vertical), glm::vec3(1.0, 0.0, 0.0));

			glm::vec4 newPosition = roatation * glm::vec4(window->getCamera().position, 1.0);
			window->setCameraPosition(newPosition.x, newPosition.y, newPosition.z);
			x_pre = position_x;
			y_pre = position_y;
		}
		else
		{
			x_pre = -1;
			y_pre = -1;
		}
		break;
	}
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
	camera.position = glm::vec3(0.0,0.0,5.0);
	camera.lookAt = glm::vec3(0.0, 0.0, 0.0);
	camera.up = glm::vec3(0.0, 1.0, 0.0);
	botton_click_x = 0;
	botton_click_y = 0;
	button_clicked = false;
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

void myWindow::bottonClickDown(int x, int y)
{
	botton_click_x = x;
	botton_click_y = y;
	button_clicked = true;
}

void myWindow::bottonClickUp()
{
	button_clicked = false;
}

bool myWindow::getClickedPosition(int& x, int& y)
{
	if (!button_clicked)
	{
		x = -1;
		y = -1;
	}
	else
	{
		x = botton_click_x;
		y = botton_click_y;
	}
	return button_clicked;
}

void myWindow::setCameraPosition(float x, float y, float z)
{
	camera.position = glm::vec3(x, y, z);
}

HWND myWindow::getMyWindow()
{
	return this->hwnd;
}

Camera myWindow::getCamera()
{
	return this->camera;
}