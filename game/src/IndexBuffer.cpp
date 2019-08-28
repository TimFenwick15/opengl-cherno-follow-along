#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) 
	: m_Count (count) {
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_RendererID)); /* The object ID and buffer */
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); /* 6 * 2 because we have 6 vertices, each with an x and y float position */

}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
