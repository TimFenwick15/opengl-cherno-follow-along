#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() {
	/* We must not pass 0 as the object ID. 1 is the first allowed.
	* The ID gets written to &m_RendererID.
	*/
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {

	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		/* A vertex contains position, texture, normal etc. Any data about a point; not just position.
		* With a buffer bound, glVertexAttribPointer adds an attribute to a vertex.
		* This is also bound to the current vertex array, which we are now defining ourselves.
		* 0 : offset, we only have one attribute so no offset. The next attribute would be offset by the size of this one.
		Pass this to the enable function also.
		* 2 : number of items per complete data set. In this case, an x and a y
		* GL_FLOAT : the data type
		* GL_FALSE : do we want OpenGL to try to normalise our data?
		* sizeof(float) * 2 : the width of our data set in bytes
		* 0 : distance to the following data type of our attribute
		*/
		GLCall(glEnableVertexAttribArray(i))
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalised, layout.GetStride(), (const void*) offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}
