#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/* This is a Visual Studio specific break, there are more general ways to do this */
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
	/* Read error buffer until no flags returned */
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {

	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);

	enum class ShaderType {
		NONE = -1, VEXTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VEXTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();

	/* id : the shader we created earlier
	 * 1: how many source codes
	 * &src : the source code
	 * nullptr : length, think this is the array of the lengths of each source code
	 */
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	/* Error handling */
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		/* alloca is so we can allocate a variable size array on the stack.
		 * Video comments recommended against this, as it's inconsistently implemented.
		 */
		char* message = (char*)alloca(length * sizeof(char));

		/* shader id, max message length, length address for some reason,
		 * and a buffer to write message to
		 */
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

/* For simplicity, shader source code will be a string in our code */
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

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
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer)); /* The object ID and buffer */
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW)); /* 6 * 2 because we have 6 vertices, each with an x and y float position */

	/* A vertex contains position, texture, normal etc. Any data about a point; not just position.
	 * With a buffer bound, glVertexAttribPointer adds an attribute to a vertex.
	 * 0 : offset, we only have one attribute so no offset. The next attribute would be offset by the size of this one.
	       Pass this to the enable function also.
	 * 2 : number of items per complete data set. In this case, an x and a y
	 * GL_FLOAT : the data type
	 * GL_FALSE : do we want OpenGL to try to normalise our data?
	 * sizeof(float) * 2 : the width of our data set in bytes
	 * 0 : distance to the following data type of our attribute
	 */
	unsigned int attributeIndex = 0;
	GLCall(glEnableVertexAttribArray(attributeIndex));
	GLCall(glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	unsigned int indexBufferObject;
	GLCall(glGenBuffers(1, &indexBufferObject));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	/* 4f because we're passing 4 floats (to a vec4) */
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f)) ;

	float r = 0.0f;
	float increment = 0.05;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		/* This is "legacy" OpenGL. It's discouraged but fine for testing. */
		/*glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();*/

		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

		/* If we were just drawing vetices, we'd use: 
		 * glDrawArrays(GL_TRIANGLES, 0, 3);
		 * To draw an index buffer, use glDrawElements
		 * 6 : number of vertices
		 * nullptr : We've already bound indexBufferObject, so we don't need this arg
		 * 
		 * glDrawElements is the main, most correct way to be drawing in OpenGL. We'll see this a lot.
		 */
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)); /* An error introduced */

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

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}