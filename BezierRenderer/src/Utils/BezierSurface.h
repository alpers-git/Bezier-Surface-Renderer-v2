#pragma once
#include <vector>
#include "../OpenGL/Shader.h"
#include "../OpenGL/Renderer.h"
#include "../OpenGL/VertexBufferLayout.h"
#include "../Scene/SceneObject.h"
#include "../Scene/DraggablePoint.h"
#include "../Scene/Pivot.h"

using namespace std;

class BezierSurface : public SceneObject
{
public:
	BezierSurface();
	BezierSurface(int rows, int cols, glm::vec4** control_points);
	BezierSurface(int rows, int cols, glm::vec4 ** control_points, MaterialTexture model);
	~BezierSurface();

	static glm::vec4 CalculatePointOnBezierCurve(float t, glm::vec4* calculation_points, int len);
	static glm::vec4 CalculatePointOnBezierSurface(float u, float v, glm::vec4** calculation_points, int r, int c);

	void DrawWireFrame(Renderer renderer, Shader* shader, VertexArray* vArray);
	void DrawControlPoints(Renderer renderer, Shader* shader, VertexArray* vArray);
	void Draw(Renderer renderer, Shader* shader, VertexArray* vArray);
	void HitAndSelect(Ray ray);

	inline void GetControlPoints(glm::vec4** c) { c = m_control_points; }
	//inline void SetCPMaterial(MaterialTexture mat_tex) { m_control_point_mat_tex = mat_tex; };
	inline void SetControlPointScale(float s) { m_control_point_scale = s; }
	void DragSelectedControlPoints(glm::vec3 drag);

	void EvaluateBezierSurface();

private:
	int m_num_control_row;
	int m_num_control_col;
	glm::vec4** m_control_points;
	DraggablePoint** m_draggable_points;
	Pivot m_piv = Pivot();
	VertexBufferLayout m_layout;
	float m_control_point_scale = 0.01f;

	inline static int Factorial(int n) { return (n == 0 || n == 1) ? 1 : n * Factorial(n - 1); }

	void CalculateIndices();
	void PushDraggablePoints();

	glm::vec4 CalculateDerivativeSum(vector<glm::vec4> v, float t, int direction);
	glm::vec3 CalculateNormal(float u, float v);
};