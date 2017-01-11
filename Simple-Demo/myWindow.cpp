#include"myWindow.h"
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>
#define PI 3.14159265358979323846


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
			float angle_horizen = horizen / ((float)window->width / 2)*90.0; // 90 degree per half screen
			float angle_vertical = vertical / ((float)window->height / 2)*60.0; // 60 degree per half screen
			glm::mat4 roatation = glm::mat4(1.0);
			roatation = glm::rotate(roatation, glm::radians(angle_horizen), glm::vec3(0.0, 1.0, 0.0));
			roatation = glm::rotate(roatation, glm::radians(angle_vertical), glm::vec3(1.0, 0.0, 0.0));
			glm::vec4 newPosition = roatation * glm::vec4(window->getCamera()->position - window->getCamera()->lookAt, 1.0) + glm::vec4(window->getCamera()->lookAt, 0.0);


			glm::vec3 direction = window->getCamera()->position - window->getCamera()->lookAt;
			float length = glm::sqrt(glm::dot(direction, direction));
			glm::vec3 up = window->getCamera()->up;
			assert(length > 0);
			float alpha = glm::acos(direction.y / length);
			float beta = glm::radians(angle_vertical);
			if (up.y < 0)
				alpha = -alpha;

			if ((alpha >= 0 && alpha - beta < 0) ||
				(alpha >= -PI && alpha - beta < -PI) ||
				(alpha <= 0 && alpha - beta > 0) ||
				(alpha <= PI && alpha - beta > PI))
			{
				window->getCamera()->up.y *= -1;
			}

			window->getCamera()->position = glm::vec3(newPosition);
			window->setClickedPosition(position_x, position_y);
		}
		break;
	}
	case WM_CHAR:
		switch (wParam)
		{
		case 'w':
		{
			
			glm::vec3 position = window->getCamera()->position;
			glm::vec3 lookat = window->getCamera()->lookAt;
			glm::vec3 direction = position - lookat;
			glm::vec3 up = window->getCamera()->up;
			float length = glm::sqrt(glm::dot(direction, direction));
			float alpha = glm::acos(direction.y / length);
			float beta = glm::radians(3.5f); // 3.5 degree per time
			if (up.y < 0)
				alpha = -alpha;
			if ((alpha >= 0 && alpha - beta < 0) ||
				(alpha >= -PI && alpha - beta < -PI))
			{
				window->getCamera()->up.y *= -1;
			}
			glm::mat4 roatation = glm::mat4(1.0);
			roatation = glm::rotate(roatation, beta, glm::vec3(1.0, 0.0, 0.0));
			glm::vec4 newPosition = roatation * glm::vec4(window->getCamera()->position - window->getCamera()->lookAt, 1.0) + glm::vec4(window->getCamera()->lookAt, 0.0);
			window->getCamera()->position = glm::vec3(newPosition);
			break;
		}
		case 's':
		{
			glm::vec3 position = window->getCamera()->position;
			glm::vec3 lookat = window->getCamera()->lookAt;
			glm::vec3 direction = position - lookat;
			glm::vec3 up = window->getCamera()->up;
			float length = glm::sqrt(glm::dot(direction, direction));
			float alpha = glm::acos(direction.y / length);
			if (up.y < 0)
				alpha = -alpha;
			float beta = glm::radians(-3.5f); // 3.5 degree per time
			if ((alpha <= 0 && alpha - beta > 0) ||
				(alpha <= PI && alpha - beta > PI))
			{
				window->getCamera()->up.y *= -1;
			}
			glm::mat4 roatation = glm::mat4(1.0);
			roatation = glm::rotate(roatation, beta, glm::vec3(1.0, 0.0, 0.0));
			glm::vec4 newPosition = roatation * glm::vec4(window->getCamera()->position - window->getCamera()->lookAt, 1.0) + glm::vec4(window->getCamera()->lookAt, 0.0);
			window->getCamera()->position = glm::vec3(newPosition);
			break;
		}
		case 'a':
		{
			float horizen = -3.0; // 3 degree per time
			glm::mat4 roatation = glm::mat4(1.0);
			roatation = glm::rotate(roatation, glm::radians(horizen), glm::vec3(0.0, 1.0, 0.0));

			glm::vec4 newPosition = roatation * glm::vec4(window->getCamera()->position, 1.0);
			window->getCamera()->position = glm::vec3(newPosition);
			break;
		}
		case 'd':
		{
			float horizen = 3.0; // 3 degree per time
			glm::mat4 roatation = glm::mat4(1.0);
			roatation = glm::rotate(roatation, glm::radians(horizen), glm::vec3(0.0, 1.0, 0.0));

			glm::vec4 newPosition = roatation * glm::vec4(window->getCamera()->position, 1.0);
			window->getCamera()->position = glm::vec3(newPosition);
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
		zDelta *= -1;
		std::cout << zDelta << std::endl;
		glm::vec3 position = window->getCamera()->position;
		glm::vec3 lookat = window->getCamera()->lookAt;
		glm::vec3 direction = position - lookat;
		if (glm::dot(direction, direction) <= 0.16 && zDelta < 0)  // length <= 0.4 
			break;
		window->getCamera()->position += (float)(((float)zDelta) / 120.0 * 0.2) * glm::normalize(direction); // 0.2 per zDelta
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
	camera.position = glm::vec3(0.0,0.0,-3.0);
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