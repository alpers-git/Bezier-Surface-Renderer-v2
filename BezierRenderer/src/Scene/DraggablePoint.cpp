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
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
		10.0f, "u_Material");
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
		shader->SetUniformMat4f(this->GetTransformName(), this->GetTransform());

		shader->SetMaterial(m_mat_tex.mat.GetUniformName(), m_mat_tex.mat);
		IndexBuffer iBuffer(&GetIndices().at(0), GetIndices().size());
		renderer.Draw(*vArray, iBuffer, *shader, GL_LINES);
		GLCall(glEnable(GL_DEPTH_TEST));
}

bool DraggablePoint::Hit(Ray ray, float aabb, float* distance)
{

	// Intersection method from Real-Time Rendering and Essential Mathematics for Games

	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 OBBposition_worldspace(m_transform.transform[3].x, m_transform.transform[3].y, m_transform.transform[3].z);

	glm::vec3 delta = OBBposition_worldspace - ray.origin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(m_transform.transform[0].x, m_transform.transform[0].y, m_transform.transform[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray.direction, xaxis);

		if (fabs(f) > 0.001f) { // Standard case

			float t1 = (e + -aabb) / f; // Intersection with the "left" plane
			float t2 = (e + aabb) / f; // Intersection with the "right" plane
											 // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

											 // We want t1 to represent the nearest intersection, 
											 // so if it's not the case, invert t1 and t2
			if (t1 > t2) {
				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin)
				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + -aabb > 0.0f || -e + aabb < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		glm::vec3 yaxis(m_transform.transform[1].x, m_transform.transform[1].y, m_transform.transform[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(ray.direction, yaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + -aabb) / f;
			float t2 = (e + aabb) / f;

			if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + -aabb > 0.0f || -e + aabb < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		glm::vec3 zaxis(m_transform.transform[2].x, m_transform.transform[2].y, m_transform.transform[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(ray.direction, zaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + -aabb) / f;
			float t2 = (e + aabb) / f;

			if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + -aabb > 0.0f || -e + aabb < 0.0f)
				return false;
		}
	}

	//*distance = tMin;
	return true;
}
/*{
	//source:http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
	float tMin = 0.0f;
	float tMax = FLT_MAX;

	glm::vec3 OBBposition_worldspace(m_transform.transform[3].x, m_transform.transform[3].y, m_transform.transform[3].z);
	glm::vec3 delta = OBBposition_worldspace - ray.origin;

	glm::vec3 xaxis(m_transform.transform[0].x, m_transform.transform[0].y, m_transform.transform[0].z);
	float e = glm::dot(xaxis, delta);
	float f = glm::dot(ray.direction, xaxis);

	// Beware, don't do the division if f is near 0 ! See full source code for details.
	if (f > 0.5 || f < -0.5)
	{
		float t1 = (e + -1.0f) / f; // Intersection with the "left" plane
		float t2 = (e + 1.0f) / f; // Intersection with the "right" plane

		if (t1 > t2) { // if wrong order
			float w = t1; t1 = t2; t2 = w; // swap t1 and t2
		}

		// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
		if (t2 < tMax) tMax = t2;
		// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
		if (t1 > tMin) tMin = t1;

		if (tMax < tMin)
			return false;
	}
	else
		return false;

	//---------------------------------------------//
	tMin = 0.0f;
	tMax = FLT_MAX;
	glm::vec3 yaxis(m_transform.transform[1].x, m_transform.transform[1].y, m_transform.transform[1].z);
	e = glm::dot(yaxis, delta);
	f = glm::dot(ray.direction, yaxis);

	// Beware, don't do the division if f is near 0 ! See full source code for details.
	t1 = (e + 1.0f) / f; // Intersection with the "left" plane
	t2 = (e + -1.0f) / f; // Intersection with the "right" plane

	if (t1>t2) { // if wrong order
		float w = t1; t1 = t2; t2 = w; // swap t1 and t2
	}

	// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
	if (t2 < tMax) tMax = t2;
	// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
	if (t1 > tMin) tMin = t1;

	if (tMax < tMin)
		return false;*/
	//---------------------------------------------//
	/*tMin = 0.0f;
	tMax = FLT_MAX;
	glm::vec3 zaxis(m_transform.transform[2].x, m_transform.transform[2].y, m_transform.transform[2].z);
	e = glm::dot(zaxis, delta);
	f = glm::dot(ray.direction, zaxis);

	// Beware, don't do the division if f is near 0 ! See full source code for details.
	t1 = (e + 1.0f) / f; // Intersection with the "left" plane
	t2 = (e + -1.0f) / f; // Intersection with the "right" plane

	if (t1>t2) { // if wrong order
		float w = t1; t1 = t2; t2 = w; // swap t1 and t2
	}

	// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
	if (t2 < tMax) tMax = t2;
	// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
	if (t1 > tMin) tMin = t1;

	if (tMax < tMin)
		return false;

	cout << "hit" << endl;

	return true;
}*/

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
