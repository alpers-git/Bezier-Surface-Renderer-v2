#include "BezierSurface.h"
#include <math.h> 
#include <iostream>

const int SAMPLING_RATE = 15;

BezierSurface::BezierSurface()
	: m_num_control_row(0),
	m_num_control_col(0),
	m_vertices(NULL),
	m_control_points(NULL),
	m_indices(NULL)
{}

BezierSurface::BezierSurface(int rows, int cols, glm::vec4 ** control_points)
	:m_num_control_row(rows),
	m_num_control_col(cols),
	m_control_points(control_points),
	m_indices(NULL)
{
	EvaluateBezierSurface();
}

BezierSurface::~BezierSurface()
{
	m_control_points = nullptr;
}

glm::vec4 BezierSurface::CalculatePointOnBezierCurve(float t, glm::vec4 * calculation_points, int len)
{
	glm::vec4 point;

	int p = len - 1;

	for (int i = 0; i < len; i++) {
		float k = (Factorial(p) / (Factorial(i) * Factorial(p - i))) * pow((1 - t), p - i) * pow((t), i);
		glm::vec4  newPoint = glm::vec4(calculation_points[i].x * k, calculation_points[i].y * k, calculation_points[i].z * k, 1);
		point = point + newPoint;
	}

	point.w = 1;
	return point;
}

glm::vec4 BezierSurface::CalculatePointOnBezierSurface(float u, float v, glm::vec4** calculation_points, int r, int c)
{
	glm::vec4* uCurve = new glm::vec4[r];

	for (int i = 0; i < r; i++) {
		glm::vec4* selected_cp = new glm::vec4[c];

		for (int j = 0; j < c; j++)
			selected_cp[j] = (calculation_points[i][j]);

		uCurve[i] = CalculatePointOnBezierCurve(u, selected_cp, c);
		delete[] selected_cp;
	}
	glm::vec4 point = CalculatePointOnBezierCurve(v, uCurve, r);
	delete[] uCurve;

	return point;
}

void BezierSurface::EvaluateBezierSurface()
{
	Vertex v;
	for (int i = 0; i < SAMPLING_RATE; i++)
		for (int j = 0; j < SAMPLING_RATE; j++) {
			v.SetPosition( CalculatePointOnBezierSurface( ((float)i) / ((float)SAMPLING_RATE - 1.0f), ((float)j) / ((float)SAMPLING_RATE - 1.0f),
				m_control_points, m_num_control_row, m_num_control_col ));
			v.SetTextureCoords(glm::vec2(( (float)i) / ((float)SAMPLING_RATE - 1.0f), ((float)j) / ((float)SAMPLING_RATE - 1.0f)));
			m_vertices.push_back(v);
			v.SetNormal( CalculateNormal(((float)i) / ((float)SAMPLING_RATE - 1.0f), ((float)j) / ((float)SAMPLING_RATE - 1.0f)));
		}
	CalculateIndices();
}

void BezierSurface::CalculateIndices()
{
	for (int j = 1; j <= SAMPLING_RATE * (SAMPLING_RATE -1); j++) {

		if (!((j - 1) % SAMPLING_RATE == (SAMPLING_RATE - 1 )))
		{
			m_indices.push_back(j - 1);
			m_indices.push_back(j);
			m_indices.push_back(SAMPLING_RATE + j);
			m_indices.push_back(SAMPLING_RATE - 1 + j);
		}
	}
}

glm::vec3 BezierSurface::CalculateNormal(float u, float v)
{
	return glm::vec3(0.0f, 0.0f, 1.0f);
}
