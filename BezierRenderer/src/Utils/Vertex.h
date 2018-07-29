#pragma once
#include "glm\glm.hpp"

class Vertex
{
public:
	Vertex();
	Vertex(const Vertex& other);
	Vertex(float c_x, float c_y, float c_z, float c_w, float n_x, float n_y, float n_z, float u, float v);
	Vertex(glm::vec4 coords, glm::vec3 normal_coords, glm::vec2 text_coords);
	~Vertex();

	inline void SetPosition(glm::vec4 p) { m_position = p; }
	inline void SetNormal(glm::vec3 n) { m_normal = n; }
	inline void SetTextureCoords(glm::vec2 t) { m_texture_coords = t; }

private:
	glm::vec4 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_texture_coords;
};