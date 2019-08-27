Follow along of https://www.youtube.com/watch?v=W3gAzLwfIP0 :heart:

# Learns
## OpenGL
- Reference: http://docs.gl/
- OpenGL is a spec/graphics API definition, that graphic card manufacturers can implement
- Video author much prefers Direct3D 11 API to an OpenGL based API
- This is different to (for eg) SDL, which is an application development library that includes graphics
- OpenGL defines something like a state machine. Objects have IDs. You bind the object, then send instructions to draw that object
- GLFW is a simple OpenGL based API. Video series uses this
- GLEW is a library for finding graphic functions on your computer and exposing them to you. This helps compatibility. Without this, you only have access to legacy OpenGL. With this, you get access to modern OpenGL code your graphics manufacturers have written
- Pipeline:
	- Draw call
	- Vectex shader runs for each vertex, sets position and passes data to fragment shader. Because this is run many fewer times than the fragment shader (per vertex instead of per pixel), we want almost all of out logic to be here
	- Fragment shader runs once for each pixel, primarily or picking the pixel colour
	- Rasterisation - draw to screen
- Error handling
	- glGetError, will give us back an error code (may have several queued)
	- glDebugMessageCallback, as of OpenGL 4.3. We pass a callback OpenGL should use on error. This is better because it doesn't require polling, but requires up to date OpenGL
- Data can be pased to shaders by:
	- Via a Vertex buffer (attributes) - applied per vertex draw
	- Via Uniforms - applied per draw

## Shaders
- Shader code is code that runs on a GPU
- Vertex shader - run once per vertex
- Fragment shader (pixel shader) - run once per pixel
- Shaders can be complex. They are often generated on the fly
- GLSL is OpenGL shader language 

## Vertex Arrays (VAO)
- A way to bind vertex buffers (the vertex data, coordinates, attributes etc) with configuration
- Vertex buffers do not get stored with the layout. Without vertex arrays, we'd need to bind the vertex buffer on each draw
- Draw process without using vertex arrays:
  - bind shader
  - bind vertex buffer
  - set up vertex layout
  - bind index buffer
  - draw call
- Draw process using vertex arrays:
  - bind shader
  - bind vertex array
  - bind index buffer
  - draw call
- Vertex arrays are actually mandatory. In CORE progile, a default will be created.
- There are two ways we could use Vertex Arrays
  - Bind one vertex array for the lifetime of the application, and bind vertex buffers and layouts every time you want to draw. This is what would happen in COMPAT profile. A few years ago, this was benchmarking to be the more performant approach
  - Create a vertex array for each geometry we want to draw. This is recommended in OpenGL

## Visual Studio
- Visual Studio project properties are stored within .sln and .proj files. The .vs folder can be gitignored. If they appear not to be carried over, make sure the configuration is set to "All configurations".
- Undeclared symbol errors are common. If you google the method name, you should find a windows dev page about it. There's a table with the library it comes from. In properties > Linker > Input, add that library, and the problem goes away
