#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "../Utils/Material.h"

class Material;

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_Filepath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string & filepath);
	unsigned int CompileShader(unsigned int type, const std::string & source);
	unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader);

public:
	Shader(const std::string& filepath);
	Shader(const Shader& s);
	Shader &operator=(Shader rhs);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set Uniforms

	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
	void SetUniformMat3f(const std::string& name, const glm::mat3 matrix);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform1i(const std::string& name, int v0);
	void SetMaterial(const std::string& name, Material v0);
};