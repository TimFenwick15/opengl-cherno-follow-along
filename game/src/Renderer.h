#pragma once

#include <GL/glew.h>
#include <iostream>

/* This is a Visual Studio specific break, there are more general ways to do this */
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
