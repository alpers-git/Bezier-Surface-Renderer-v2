#include <iostream>
#include "Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char * function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] ( " << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Draw(const VertexArray & vArray, const IndexBuffer & iBuffer, const Shader & shader) const
{
	shader.Bind();
	vArray.Bind();
	iBuffer.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, iBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));

	//GLCall(glDrawElements(GL_LINE_LOOP, iBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray & vArray, const IndexBuffer & iBuffer, const Shader & shader, GLenum primitive) const
{
	shader.Bind();
	vArray.Bind();
	iBuffer.Bind();

	GLCall(glDrawElements(primitive, iBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
