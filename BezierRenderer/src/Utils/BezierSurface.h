#pragma once
#include "Vertex.h"
#include <vector>
#include "../OpenGL/Shader.h"
#include "../OpenGL/Renderer.h"
#include "Material.h"

using namespace std;

class BezierSurface
{
public:
	BezierSurface();
	BezierSurface(int rows, int cols, glm::vec4** control_points);
	BezierSurface(int rows, int cols, glm::vec4** control_points, Material mat);
	~BezierSurface();

	static glm::vec4 CalculatePointOnBezierCurve(float t, glm::vec4* calculation_points, int len);
	static glm::vec4 CalculatePointOnBezierSurface(float u, float v, glm::vec4** calculation_points, int r, int c);

	inline vector<Vertex> GetVertices() const { return m_vertices; };
	inline vector<unsigned int> GetIndices() const  { return m_indices; };
	inline Material GetMaterial() { return m_material; }

	inline void SetMaterial(Material material) { m_material = material; }

	void EvaluateBezierSurface();

private:
	int m_num_control_row;
	int m_num_control_col;
	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	glm::vec4** m_control_points;
	Material m_material;

	inline static int Factorial(int n) { return (n == 0 || n == 1) ? 1 : n * Factorial(n - 1); }

	void CalculateIndices();

	glm::vec4 CalculateDerivativeSum(vector<glm::vec4> v, float t, int direction);
	glm::vec3 CalculateNormal(float u, float v);
};