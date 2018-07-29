#pragma once
#include "Vertex.h"
#include <vector>

using namespace std;

class BezierSurface
{
public:
	BezierSurface();
	BezierSurface(int rows, int cols, glm::vec4** control_points);
	~BezierSurface();

	static glm::vec4 CalculatePointOnBezierCurve(float t, glm::vec4* calculation_points, int len);
	static glm::vec4 CalculatePointOnBezierSurface(float u, float v, glm::vec4** calculation_points, int r, int c);

	inline vector<Vertex> GetVertices() const { return m_vertices; };
	inline vector<unsigned int> GetIndices() const  { return m_indices; };

	void EvaluateBezierSurface();

private:
	int m_num_control_row;
	int m_num_control_col;
	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	glm::vec4** m_control_points;

	inline static int Factorial(int n) { return (n == 0 || n == 1) ? 1 : n * Factorial(n - 1); }

	void CalculateIndices();
	glm::vec3 CalculateNormal(float u, float v);
};