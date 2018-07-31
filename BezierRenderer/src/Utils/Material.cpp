#include "Material.h"

Material::Material()
	:m_ambient(glm::vec4()),
	m_diffuse(glm::vec4()),
	m_specular(glm::vec4())
{}

Material::~Material()
{}

Material::Material(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess, string uniform_name)
	:m_ambient(ambient),
	m_diffuse(diffuse),
	m_specular(specular),
	m_shininess(shininess),
	m_glsl_uniform_name(uniform_name)
{
}
