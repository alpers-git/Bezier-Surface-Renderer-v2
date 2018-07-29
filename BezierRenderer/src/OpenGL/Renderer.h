#pragma once

#include <GL/glew.h>
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char * function, const char* file, int line);

class Renderer
{
private:
public:
	void Draw( const VertexArray& vArray, const IndexBuffer& iBuffer, const Shader& shader ) const;
	void Draw(const VertexArray& vArray, const IndexBuffer& iBuffer, const Shader& shader, GLenum primitive) const;
	void Clear();
};