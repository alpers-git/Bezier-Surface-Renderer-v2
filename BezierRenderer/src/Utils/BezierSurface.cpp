#include "BezierSurface.h"
#include <math.h> 
#include <iostream>

const int SAMPLING_RATE = 15;

BezierSurface::BezierSurface()
	: m_num_control_row(0),
	m_num_control_col(0),
	m_control_points(NULL)
{}

BezierSurface::BezierSurface(int rows, int cols, glm::vec4 ** control_points)
	:m_num_control_row(rows),
	m_num_control_col(cols),
	m_control_points(control_points)
{
	EvaluateBezierSurface();
}

BezierSurface::~BezierSurface()
{
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
	m_vertices.clear();
	Vertex v;
	for (int i = 0; i < SAMPLING_RATE; i++)
		for (int j = 0; j < SAMPLING_RATE; j++) {
			v.SetPosition( CalculatePointOnBezierSurface( ((float)i) / ((float)SAMPLING_RATE - 1.0f), ((float)j) / ((float)SAMPLING_RATE - 1.0f),
				m_control_points, m_num_control_row, m_num_control_col ));
			v.SetTextureCoords(glm::vec2(( (float)i) / ((float)SAMPLING_RATE - 1.0f), ((float)j) / ((float)SAMPLING_RATE - 1.0f)));
			v.SetNormal( CalculateNormal(((float)i) / ((float)SAMPLING_RATE), ((float)j) / ((float)SAMPLING_RATE)));
			m_vertices.push_back(v);
		}
	CalculateIndices();
}

void BezierSurface::CalculateIndices()
{
	m_indices.clear();
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

glm::vec4 BezierSurface::CalculateDerivativeSum(vector<glm::vec4> v, float t, int direction)
{

	/*sum += ((-(degree -1 - n) * pow(1.0f - t, degree - n - 2) * pow(t, n) + n * pow(t, n - 1) *  pow(1.0f - t, degree -1 - n))
	* Factorial(degree-1) / (Factorial(degree -1 - n) * Factorial(n) ))
	* v.at(n);*/
	/*std::cout << "P" << n << " * " << -(degree - 1 - n) << " *( (1-u)^" << degree - n - 2 << " * u^" << n << " + " << n << " * u^" << n - 1 << " * (1-u)^" << degree - 1 - n << ")* C(" << degree - 1 << ", " << n << ") + ";*/
	int degree;
	if (direction == 0)
		degree = m_num_control_row - 1;
	else
		degree = m_num_control_col - 1;

	glm::vec4 sum1;
	glm::vec4 sum2;

	//(derivative(u * (1-u) )) * C(n, k) * Pn
	for (int n = 0; n < degree; n++)
	{
		sum1 += (Factorial(degree) / (Factorial(degree - n) * Factorial(n)))
			* pow(t, n) * pow(1.0f - t, degree - n)
			* (v.at( n + 1 ));
		//std::cout << "s1:" << n + 1 << std::endl;
	}

	for (int n = 0; n < degree; n++)
	{
		sum2 += (Factorial(degree) / (Factorial(degree - n) * Factorial(n)))
			* pow(t, n) * pow(1.0f - t, degree - n)
			* (v.at( n ));
		//std::cout << "s2:" << n << std::endl;
	}
	return (float)(degree +1) * (sum1 - sum2);
}

glm::vec3 BezierSurface::CalculateNormal(float u, float v)
{
	vector<glm::vec4> p;
	vector<glm::vec4> vCurve;
	for (int j = 0; j < m_num_control_col; ++j)
	{
		for (int i = 0; i < m_num_control_row; ++i)
		{
			p.push_back(m_control_points[i][j]);
		}
		vCurve.push_back( CalculatePointOnBezierCurve(v,&p.at(0), p.size()));
		p.clear();
	}

	glm::vec3 dU= CalculateDerivativeSum(vCurve, u, 1);

	//-----------------------------------------------------------------------------------\\

	vector<glm::vec4> uCurve;
	for (int i = 0; i < m_num_control_row; ++i) {
		uCurve.push_back(CalculatePointOnBezierCurve(v, m_control_points[i], m_num_control_col));
	}

	glm::vec3 dV = CalculateDerivativeSum(uCurve, v, 0);

	return glm::normalize(glm::cross(dU, dV));
}
