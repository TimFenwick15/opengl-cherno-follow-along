#include "Renderer.h"

void GLClearError() {
	/* Read error buffer until no flags returned */
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {

	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	shader.Bind();
	va.Bind();
	ib.Bind();

	/* If we were just drawing vetices, we'd use:
	* glDrawArrays(GL_TRIANGLES, 0, 3);
	* To draw an index buffer, use glDrawElements
	* 6 : number of vertices
	* nullptr : We've already bound indexBufferObject, so we don't need this arg
	*
	* glDrawElements is the main, most correct way to be drawing in OpenGL. We'll see this a lot.
	*/
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
