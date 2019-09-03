#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"

int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Could also have COMAPT profile */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Sync buffer swap with monitor refresh rate */
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Died" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	/* When exiting our application, the gl context will be deleted, then IndexBuffer destructor (with GLCall) is called.
	 * When there's no gl context, GLClearError generates an error. This causes an infinite loop.
	 * Putting a scope around the class use causes them to be deleted before the gl context is deleted.
	 * Normally, we'd allocate these classes on the heap, so the problem would go away.
	 */
	{
		/* This is all Vertex data. Positions in this case, but will contain other data normally.
		 * The GPU draws triangles, and these are the points of a square.
		 * We use an index buffer to tell the GPU what order to draw these in, making our shape up in triangles.
		 * Advantage of this is we don't need to duplicate vertices. */
		float positions[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
		
		// eg code: unbind everything
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		
		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			renderer.Clear();

			/* This is "legacy" OpenGL. It's discouraged but fine for testing. */
			/*glBegin(GL_TRIANGLES);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.0f, 0.5f);
			glVertex2f(0.5f, -0.5f);
			glEnd();*/

			/* The draw process is:
			 * - Bind shader
			 * - Bind Vertex Array
			 * - Bind index buffer
			 * - Draw call
			 * 
			 * Ideally, these lines would be abstracted out, but this requires materials.
			 * Materials = shader + uniforms
			 */
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);

			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}
