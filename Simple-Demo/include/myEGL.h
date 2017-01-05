#pragma once
#include <EGL/egl.h>


class myEGL
{
public:
	myEGL();
	bool InitEGL(EGLNativeWindowType hwnd);
	bool MakeCurrent();
	bool SwapBuffer();
	~myEGL();

private:
	EGLDisplay display;
	EGLConfig config;
	EGLContext context;
	EGLSurface surface;
	bool Initilized;
};