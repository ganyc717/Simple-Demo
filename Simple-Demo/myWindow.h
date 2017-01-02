#pragma once
#include<windows.h>

typedef LRESULT (*WinCallBack) (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


class myWindow
{
public:
	myWindow(const char* WindowName, int Width, int height);
	~myWindow();
	void show();
	HWND getMyWindow();
	int width;
	int height;
	bool changeSize;
private:
	WNDCLASS wndClass;
	HWND hwnd;
	char winName[64];
};
