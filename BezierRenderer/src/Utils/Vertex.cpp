#include "Vertex.h"
#pragma once

Vertex::Vertex()
	: m_position(glm::vec4()), m_normal(glm::vec3()), m_texture_coords(glm::vec2())
{}

Vertex::Vertex(const Vertex & other)
	:m_position(other.m_position),
	m_normal(other.m_normal),
	m_texture_coords(other.m_texture_coords)
{}

Vertex::Vertex(float c_x, float c_y, float c_z, float c_w, float n_x, float n_y, float n_z, float u, float v)
	: m_position(glm::vec4(c_x, c_y, c_z, c_w)),
	m_normal(glm::vec3(n_x, n_y, n_z)),
	m_texture_coords(glm::vec2(u, v))
{}

Vertex::Vertex(glm::vec4 coords, glm::vec3 normal_coords, glm::vec2 text_coords)
	: m_position(coords), m_normal(normal_coords), m_texture_coords(text_coords)
{}

Vertex::~Vertex()
{}
