#include"myWindow.h"


#include"fileops.hpp"



using namespace std;

int main()
{

	myWindow win("demo",700,500);
	win.show();

	bool exit = false;
	MSG msg;
	while (!exit) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT)
				exit = true;
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	system("pause");
	return 0;
} 