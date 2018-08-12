#pragma once
#include "SceneObject.h"
#include "DraggablePoint.h"


class Pivot : public SceneObject
{
public:
	Pivot();
	~Pivot();

	void Draw(Renderer renderer, Shader* shader, VertexArray* vArray);
private:
	DraggablePoint m_x_axis_box;
	DraggablePoint m_y_axis_box;
	DraggablePoint m_z_axis_box;
	VertexBufferLayout m_layout;
};