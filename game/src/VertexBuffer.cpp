#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	GLCall(glGenBuffers(1, &m_RendererID)); /* The object ID and buffer */
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); /* 6 * 2 because we have 6 vertices, each with an x and y float position */

}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
