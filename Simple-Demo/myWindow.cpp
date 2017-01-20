#include"myWindow.h"
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>
#define MOUSE_MOVE_HORIZON_ANGLE 90.0f
#define MOUSE_MOVE_VERTICAL_ANGLE 60.0f
#define KEY_MOVE_PACE 0.1f
#define MOUSEWHEEL_MOVE_PACE 0.1f

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
		if (window->getClickedPosition(x, y))
		{
			int position_x = LOWORD(lParam);
			int position_y = HIWORD(lParam);
			float vertical = y - position_y;
			float horizen = x - position_x;
			float angle_horizen = horizen / ((float)window->width / 2) * MOUSE_MOVE_HORIZON_ANGLE;
			float angle_vertical = vertical / ((float)window->height / 2) * MOUSE_MOVE_VERTICAL_ANGLE;

			window->getCamera()->rotate(glm::vec3(glm::radians(angle_vertical), glm::radians(angle_horizen), 0.0));
			window->setClickedPosition(position_x, position_y);
		}
		break;
	}
	case WM_CHAR:
		switch (wParam)
		{
		case 'w':
		{
			glm::vec3 direction = window->getCamera()->getDirection();
			glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
			glm::vec3 right = glm::cross(direction, up);
			glm::vec3 forward = glm::normalize(glm::cross(up, right));
			window->getCamera()->translate(KEY_MOVE_PACE * forward);
			break;
		}
		case 's':
		{
			glm::vec3 direction = window->getCamera()->getDirection();
			glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
			glm::vec3 right = glm::cross(direction, up);
			glm::vec3 forward = glm::normalize(glm::cross(up, right));
			window->getCamera()->translate(-KEY_MOVE_PACE * forward);
			break;
		}
		case 'a':
		{
			glm::vec3 direction = window->getCamera()->getDirection();
			glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
			glm::vec3 right = glm::cross(direction, up);
			window->getCamera()->translate(-KEY_MOVE_PACE * right);
			break;
		}
		case 'd':
		{
			glm::vec3 direction = window->getCamera()->getDirection();
			glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
			glm::vec3 right = glm::cross(direction, up);
			window->getCamera()->translate(KEY_MOVE_PACE * right);
			break;
		}
		default:
			std::cout << wParam << std::endl;
			break;
		}
		break;
	case WM_MOUSEWHEEL:
	{
		int zDelta = (short)HIWORD(wParam);

		glm::vec3 direction = window->getCamera()->getDirection();
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
		glm::vec3 right = glm::cross(direction, up);
		glm::vec3 forward = glm::normalize(glm::cross(up, right));
		window->getCamera()->translate(MOUSEWHEEL_MOVE_PACE * (zDelta / 120) * forward);
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

	camera.setPosition(glm::vec3(1.0, 0.5, 3.0));
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

bool myWindow::setClickedPosition(int x, int y)
{
	if (!button_clicked)
	{
		botton_click_x = -1;
		botton_click_y = -1;
	}
	else
	{
		botton_click_y = y;
		botton_click_x = x;
	}
	return button_clicked;
}

HWND myWindow::getMyWindow()
{
	return this->hwnd;
}

Camera* myWindow::getCamera()
{
	return &this->camera;
}