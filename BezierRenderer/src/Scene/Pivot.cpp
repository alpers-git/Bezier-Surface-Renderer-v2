#include "Pivot.h"

Pivot::Pivot()
	: SceneObject()
{
	m_layout.Push<float>(4);
	m_layout.Push<float>(3);
	m_layout.Push<float>(2);

	m_x_axis_box = DraggablePoint();
	m_x_axis_box.MoveTo(glm::vec3(-5.0f, 0.0f, 0.0f));
	m_x_axis_box.Scale(glm::vec3(5.0f, 0.1f, 0.1f));
	m_x_axis_box.SetMaterial(Material(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, "u_Material"));

	m_y_axis_box = DraggablePoint();
	m_y_axis_box.MoveTo(glm::vec3(0.0f, -5.0f, 0.0f));
	m_y_axis_box.Scale(glm::vec3(0.1f, 5.0f, 0.1f));
	m_y_axis_box.SetMaterial(Material(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 10.0f, "u_Material"));

	m_z_axis_box = DraggablePoint();
	m_z_axis_box.MoveTo(glm::vec3(0.0f, 0.0f, -5.0f));
	m_z_axis_box.Scale(glm::vec3(0.1f, 0.1f, 5.0f));
	m_z_axis_box.SetMaterial(Material(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 10.0f, "u_Material"));

	SetTransformName("u_Model");
}

Pivot::~Pivot()
{
}

void Pivot::Draw(Renderer renderer, Shader * shader, VertexArray * vArray)
{
	//VertexBuffer vBuffer = VertexBuffer(&m_x_axis_box.GetVertices().at(0), sizeof(Vertex) * m_x_axis_box.GetVertices().size());
	shader->Bind();
	//vArray->AddBuffer(vBuffer, m_layout);

	shader->SetUniformMat4f(this->GetTransformName(), this->GetTransform() * m_x_axis_box.GetTransform());
	m_x_axis_box.Draw(renderer, shader, vArray);

	//vBuffer = VertexBuffer(&m_y_axis_box.GetVertices().at(0), sizeof(Vertex) * m_y_axis_box.GetVertices().size());
	shader->Bind();
	//vArray->AddBuffer(vBuffer, m_layout);

	shader->SetUniformMat4f(this->GetTransformName(), this->GetTransform() * m_y_axis_box.GetTransform());
	m_y_axis_box.Draw(renderer, shader, vArray);

	//vBuffer = VertexBuffer(&m_z_axis_box.GetVertices().at(0), sizeof(Vertex) * m_z_axis_box.GetVertices().size());
	shader->Bind();
	//vArray->AddBuffer(vBuffer, m_layout);

	shader->SetUniformMat4f(this->GetTransformName(), this->GetTransform()* m_z_axis_box.GetTransform());
	m_z_axis_box.Draw(renderer, shader, vArray);
}
