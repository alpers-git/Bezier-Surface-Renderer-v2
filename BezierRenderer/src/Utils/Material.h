#pragma once
#include "glm\glm.hpp"
#include <iostream>
#include <string>
#include "../OpenGL/Shader.h"

//Works with GLSL struct defined below
/*
struct Material
{
vec4 ambient;
vec4 diffuse;
vec4 specular;
float shininess;
};
*/
using namespace std;

class Material
{
public:
	Material();
	~Material();
	Material(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess, string uniform_name);


	inline void SetAmbient(glm::vec4 ambient) { m_ambient = ambient; }
	inline void SetDiffuse(glm::vec4 diffuse) { m_diffuse = diffuse; }
	inline void SetSpecular(glm::vec4 specular) { m_specular = specular; }
	inline void SetShininess(float shininess) { m_shininess = shininess; }
	inline void SetUniformName(string name) { m_glsl_uniform_name = name; }

	inline glm::vec4 GetAmbient() { return m_ambient; }
	inline glm::vec4 GetDiffuse() { return m_diffuse; }
	inline glm::vec4 GetSpecular() { return  m_specular; }
	inline float GetShininess() { return  m_shininess; }
	inline string GetUniformName() { return m_glsl_uniform_name; }

private:
	glm::vec4 m_ambient;
	glm::vec4 m_diffuse;
	glm::vec4 m_specular;
	float m_shininess;
	string m_glsl_uniform_name;
};