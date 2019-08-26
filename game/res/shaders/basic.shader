#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main() {
    gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main() {
	//color = vec4(1.0, 0.0, 0.0, 1.0); // red
	//color = vec4(0.2, 0.3, 0.8, 1.0); // blue
	color = u_Color; // Using the uniform instead
};

/* This is a video suggested way of reading a shader into the application.
 * layout(location = 0) refers to attributeIndex
 * We need to use vec4, even though we're drawing a vec2. OpenGL will cast it
 * In fragmentShader, color is an rgba
 */