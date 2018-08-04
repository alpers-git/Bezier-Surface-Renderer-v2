#pragma once
#include "SceneObject.h"

class DraggablePoint : public SceneObject
{
public:
	DraggablePoint();
	DraggablePoint(MaterialTexture mat_tex);
	~DraggablePoint();

	void Draw(Renderer renderer, Shader* shader, VertexArray* vArray);
	inline void SetSelected(bool s) { m_selected = s; }
	inline bool IsSelected() { return m_selected; }

private:
	VertexBufferLayout m_layout;
	bool m_selected;
	void PushCubeModel();

};
