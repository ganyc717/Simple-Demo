#include"myEGL.h"

myEGL::myEGL()
{
	Initilized = false;
};

myEGL::~myEGL()
{
	if (!Initilized)
		return;
	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(display, context);
	eglDestroySurface(display, surface);
	eglTerminate(display);
}

bool myEGL::InitEGL(EGLNativeWindowType hwnd)
{
	EGLBoolean ret;
	int numconfigs = 0;
	const EGLint eglattr[] = {
		EGL_RED_SIZE,       8,
		EGL_GREEN_SIZE,     8,
		EGL_BLUE_SIZE,      8,
		EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
		EGL_NONE
	};

	const EGLint contextattr[] = {
		EGL_CONTEXT_CLIENT_VERSION,     3,
		EGL_NONE
	};

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	if (display == EGL_NO_DISPLAY)
	{
		MessageBox(NULL, "No Display Found", NULL, MB_ICONERROR);
		return false;
	}

	int major = 3;
	int minor = 0;
	ret = eglInitialize(display, &major, &minor);

	if (!ret)
	{
		MessageBox(NULL, "eglInitialize failed", NULL, MB_ICONERROR);
		return false;
	}

	ret = eglChooseConfig(display, eglattr, NULL, 1, &numconfigs);
	if (numconfigs == 0)
	{
		MessageBox(NULL, "No matched configs found", NULL, MB_ICONERROR);
		return false;
	}

	ret = eglChooseConfig(display, eglattr, &config, 1, &numconfigs);

	surface = eglCreateWindowSurface(display, config, hwnd, NULL);
	if (surface == EGL_NO_SURFACE)
	{
		MessageBox(NULL, "CreateWindowSurface failed", NULL, MB_ICONERROR);
		return false;
	}

	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextattr);
	if (context == EGL_NO_CONTEXT)
	{
		MessageBox(NULL, "CreateContext failed", NULL, MB_ICONERROR);
		return false;
	}
	Initilized = true;
	return MakeCurrent();
}
bool myEGL::MakeCurrent()
{
	EGLBoolean ret;
	if (!Initilized)
	{
		MessageBox(NULL, "Please Initialize EGL first", NULL, MB_ICONERROR);
		return false;
	}

	ret = eglMakeCurrent(display, surface, surface, context);
	if (ret == EGL_FALSE)
	{
		MessageBox(NULL, "MakeCurrent Failed", NULL, MB_ICONERROR);
		return false;
	}
	return true;
}

bool myEGL::SwapBuffer()
{
	if (!Initilized)
	{
		MessageBox(NULL, "Please Initialize EGL first", NULL, MB_ICONERROR);
		return false;
	}
	return eglSwapBuffers(display, surface);
}