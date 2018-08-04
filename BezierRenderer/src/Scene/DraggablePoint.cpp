#include "DraggablePoint.h"

DraggablePoint::DraggablePoint()
	:SceneObject()
{
	m_layout.Push<float>(4);
	m_layout.Push<float>(3);
	m_layout.Push<float>(2);

	PushCubeModel();

	Material material(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f, "u_Material");
		m_mat_tex = MaterialTexture(material, Texture("res/textures/flat.jpg", "u_Texture"));
}

DraggablePoint::DraggablePoint(MaterialTexture mat_tex)
	: SceneObject()
{
	m_layout.Push<float>(4);
	m_layout.Push<float>(3);
	m_layout.Push<float>(2);

	PushCubeModel();

	Material material(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f, "u_Material");
	m_mat_tex = mat_tex;
}

DraggablePoint::~DraggablePoint()
{
}

void DraggablePoint::Draw(Renderer renderer, Shader * shader, VertexArray * vArray)
{
		GLCall(glDisable(GL_DEPTH_TEST));
		VertexBuffer vBuffer = VertexBuffer(&GetVertices().at(0), sizeof(Vertex) * GetVertices().size());
		shader->Bind();
		vArray->AddBuffer(vBuffer, m_layout);

		m_mat_tex.tex.Bind();
		shader->SetUniform1i(m_mat_tex.tex.GetUniformName(), 0);
		//shader->SetUniformMat4f(this->GetTransformName(), this->GetTransform());

		shader->SetMaterial(m_mat_tex.mat.GetUniformName(), m_mat_tex.mat);
		IndexBuffer iBuffer(&GetIndices().at(0), GetIndices().size());
		renderer.Draw(*vArray, iBuffer, *shader, GL_LINES);
		GLCall(glEnable(GL_DEPTH_TEST));
}

void DraggablePoint::PushCubeModel()
{
	m_vertices.push_back(Vertex(glm::vec4(-1.0, -1.0, 1.0, 1.0f),
		glm::vec3(-1.0, -1.0, 1.0), glm::vec2(0.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec4(1.0, -1.0, 1.0, 1.0f),
		glm::vec3(1.0, -1.0, 1.0), glm::vec2(0.0f, 1.0f)));
	m_vertices.push_back(Vertex(glm::vec4(1.0, 1.0, 1.0, 1.0f),
		glm::vec3(1.0, 1.0, 1.0), glm::vec2(1.0f, 1.0f)));
	m_vertices.push_back(Vertex(glm::vec4(-1.0, 1.0, 1.0, 1.0f),
		glm::vec3(-1.0, 1.0, 1.0), glm::vec2(1.0f, 0.0f)));

	m_vertices.push_back(Vertex(glm::vec4(-1.0, -1.0, -1.0, 1.0f),
		glm::vec3(-1.0, -1.0, -1.0), glm::vec2(1.0f, 1.0f)));
	m_vertices.push_back(Vertex(glm::vec4(1.0, -1.0, -1.0, 1.0f),
		glm::vec3(1.0, -1.0, -1.0), glm::vec2(1.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec4(1.0, 1.0, -1.0, 1.0f),
		glm::vec3(1.0, 1.0, -1.0), glm::vec2(0.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec4(-1.0, 1.0, -1.0, 1.0f),
		glm::vec3(-1.0, 1.0, -1.0), glm::vec2(0.0f, 1.0f)));

	//pushing indices
	m_indices.push_back(0);
	m_indices.push_back(1);

	m_indices.push_back(1);
	m_indices.push_back(2);

	m_indices.push_back(2);
	m_indices.push_back(3);

	m_indices.push_back(3);
	m_indices.push_back(0);

	m_indices.push_back(0);
	m_indices.push_back(4);

	m_indices.push_back(4);
	m_indices.push_back(7);

	m_indices.push_back(7);
	m_indices.push_back(3);

	m_indices.push_back(4);
	m_indices.push_back(5);

	m_indices.push_back(5);
	m_indices.push_back(6);

	m_indices.push_back(6);
	m_indices.push_back(7);

	m_indices.push_back(5);
	m_indices.push_back(1);

	m_indices.push_back(6);
	m_indices.push_back(2);
}
