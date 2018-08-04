#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

#include "OpenGl/Renderer.h"
#include "OpenGl/VertexBuffer.h"
#include "OpenGl/IndexBuffer.h"

#include "OpenGl/VertexArray.h"
#include "OpenGl/VertexBufferLayout.h"
#include "OpenGl/Shader.h"
#include "OpenGl/Renderer.h"
#include "OpenGl/Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Utils/Vertex.h"
#include "Utils/BezierSurface.h"
#include "Utils/Material.h"
#include "Utils/PickerCursor.h"
#include "Scene\SceneObject.h"
#include "Scene/DraggablePoint.h"

float cursor_x, cursor_y;
bool unhandled_callback, clicked;

static void CursorPositionCallback(GLFWwindow * window, double x_pos, double y_pos);
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1080, 720, "app", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	/*PickerCursor cursor;
	cursor.SetCursorPosCallBack(window);*/
	//glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;


	//Generating Vertex Data
	glm::vec4** control_points = new glm::vec4*[5];

	//FILL THE CP ARRAY WITH THESE
	glm::vec4 cps[5][5] = { {glm::vec4(-1, -1, 1, 1), glm::vec4(-0.5, -1, 1, 1), glm::vec4(0, -1, 1, 1), glm::vec4(0.5, -1, 1, 1), glm::vec4(1, -1, 1, 1)},
	{glm::vec4(-1, -0.5, 1, 1), glm::vec4(-0.5, -0.5, 1, 1), glm::vec4(0, -0.5, 1, 1), glm::vec4(0.5, -0.5, 1, 1), glm::vec4(1, -0.5, 1, 1) },
	{glm::vec4(-1, 0, 1, 1), glm::vec4(-0.5, 0, 1, 1), glm::vec4(0, 0, 0.0f, 1), glm::vec4(0.5, 0, 1, 1), glm::vec4(1, 0, 1, 1)},
	{glm::vec4(-1, 0.5, 1, 1), glm::vec4(-0.5, 0.5, 1, 1), glm::vec4(0, 0.5, 1.0f, 1), glm::vec4(0.5, 0.5, 1, 1), glm::vec4(1, 0.5, 1, 1)},
	{glm::vec4(-1, 1, 1, 1), glm::vec4(-0.5, 1, 1, 1), glm::vec4(0, 1, 1, 1), glm::vec4(0.5, 1, 1, 1), glm::vec4(1, 1, 1, 1) } };
	
	for (int i = 0; i < 5; i++)
	{
		control_points[i] = new glm::vec4[5];
		for (int j = 0; j < 5; j++)
		{
			control_points[i][j] = cps[i][j];
		}
	}


	//------------------------------------//
	{
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		Texture texture("res/textures/metal.jpg", "u_Texture");

		Material b_material(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
				glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1000.0f, "u_Material");
		BezierSurface* b_surface = new BezierSurface(5, 5, control_points, MaterialTexture(b_material, texture));

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray vArray;
		VertexBufferLayout layout;
		VertexBuffer vBuffer(&b_surface->GetVertices().at(0), sizeof(Vertex) * b_surface->GetVertices().size());

		layout.Push<float>(4);
		layout.Push<float>(3);
		layout.Push<float>(2);
		vArray.AddBuffer(vBuffer, layout);


		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));

		IndexBuffer iBuffer(&b_surface->GetIndices().at(0), 4 * sizeof(unsigned int));

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.5f, 0.1f, 100.f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.9f));
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mvp = proj * view * model;

		//-----------------------------------------------------------------//
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		//-----------------------------------------------------------------//
		shader.Bind();

		shader.SetUniform4f("u_LightPosition", 0.0f, 0.0f, 18.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp);

		vArray.Unbind();
		shader.Unbind();

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LEQUAL));
		GLCall(glEnable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonOffset(1.0, 2.0));

		Renderer renderer;
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		bool show_demo_window = true;
		bool show_another_window = false;

		glm::vec3 ligth_pos(0.0f, 0.0f, 4.0f);
		glm::vec3 c_center = glm::vec3(0.0f, 0.0f, 0.0f);
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		float zoom = 5.0f;

		float old_a = 0.0f;
		float a = 0.0f;				
		MVP mvp2;
		mvp2.proj = proj;
		mvp2.view = view;
		mvp2.model = model;
		mvp2.uni_name = "u_MVP";

		GLCall(glClearColor(0.86f, 0.86f, 0.86f, 1.0f));
		GLCall(glLineWidth(1.5f));

		int ind_x=2, ind_y=2;

		DraggablePoint point = DraggablePoint();
		//b_surface->SetTransformName("u_Model");

		float t = 0.0f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
			if (mouseState == GLFW_PRESS)
				clicked = true;
			else
				clicked = false;
			/* Render here */
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();

			shader.Bind();
			{
				//b_surface->MoveTo(glm::vec3(0.0f, 2*sin(t), 0.0f));
				model = b_surface->GetTransform();
				view = glm::lookAt(glm::vec3(0.0f, 0.0f, zoom),
					c_center,
					glm::vec3(0.0f, 1.0f, 0.0f));
				mvp = proj * view * model;
				glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));

				shader.SetUniform4f("u_LightPosition", ligth_pos.x, ligth_pos.y, ligth_pos.z, 1.0f);
				shader.SetUniformMat4f("u_MVP", mvp);
				//shader.SetUniformMat4f("u_View", view);
				//shader.SetUniformMat4f("u_Proj", proj);
				shader.SetUniformMat4f("u_NormalMat", normal_matrix);
				t += 0.01;

				//-----------------------------------------------------------------//
				if (clicked)
				{

					float x = (2.0f * cursor_x) / width - 1.0f;
					float y = 1.0f - (2.0f * cursor_y) / height;

					glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);

					//std::cout << "x: " << x << ", y: " << y <<std::endl;
					glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;
					ray_eye.z = -1.0f;
					ray_eye.w = 0.0f;
					glm::vec3 ray_wor = (glm::inverse(view) * ray_eye);
					ray_wor = glm::normalize(ray_wor);

					float min_distance = FLT_MAX;

					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 5; j++)
						{
							float t = (model * view * control_points[i][j]).z / ray_wor.z;
							glm::vec3 hit_point = ray_wor * t;

							if (glm::distance(hit_point, glm::vec3(model * view * control_points[i][j])) < min_distance)
							{
								min_distance = glm::distance(hit_point, glm::vec3(model * view * control_points[i][j]));
								ind_x = i;
								ind_y = j;
								std::cout << "selected cp: " << ind_x << ind_y << std::endl;
							}
						}
					}

					//std::cout << hit_point.x << ", " << hit_point.y << ", " << hit_point.z << ", " << std::endl;
					//unhandled_callback = false;
				}
				//-----------------------------------------------------------------//

				control_points[ind_x][ind_y].z = a;
				if (old_a != a)
				{
					b_surface->EvaluateBezierSurface();					//Recalculating surface
					old_a = a;
				}
				b_surface->Draw(renderer, &shader, &vArray);			//Rendering
				b_surface->DrawWireFrame(renderer, &shader, &vArray);	//Rendering WireFrame
				mvp2.view = view;
				mvp2.model = model;
				b_surface->DrawControlPoints(renderer, &shader, &vArray, point, mvp2);

				point.MoveTo(ligth_pos);
				point.Scale(glm::vec3(0.02, 0.02, 0.02));
				model = point.GetTransform();
				mvp = proj * view * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				//shader.SetUniformMat4f("u_Model", model);

				Material m2(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
					glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
					glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
					10.0f, "u_Material");
				point.SetMaterial(m2);

				point.Draw(renderer, &shader, &vArray);
				point.Scale(glm::vec3(50, 50, 50));

			}

			{
				ImGui::Text("Transforms");                           // Display some text (you can use a format string too)
				ImGui::SliderFloat3("Ligth Pos", &ligth_pos.x, -10.0f, 15.0f);
				ImGui::SliderFloat3("Camera Center", &c_center.x, -10.0f, 10.0f);
				ImGui::SliderFloat("Camera Zoom", &zoom, -10.0f, 10.0f);
				ImGui::SliderFloat("a", &a, -49.0f, 49.0f);
				//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
				//ImGui::Checkbox("Another Window", &show_another_window);

				/*if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
					counter++;*/
				//ImGui::SameLine();
				//ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		delete b_surface;
	}

	for (int i = 0; i < 5; i++)
		delete[] control_points[i];
	delete[] control_points;
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

static void CursorPositionCallback(GLFWwindow * window, double x_pos, double y_pos)
{
	cursor_x = x_pos;
	cursor_y = y_pos;
	//unhandled_callback = true;
	/*int width, height;
	glfwGetWindowSize(window, &width, &height);
	float x = (2.0f * x_pos) / width - 1.0f;
	float y = 1.0f - (2.0f * y_pos) / height;

	glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);

	//std::cout << "x: " << x << ", y: " << y <<std::endl;
	glm::vec4 ray_eye = glm::inverse(m_projection_mat) * ray_clip;*/
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << "aaaa " << std::endl;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		clicked = true;
	}

	/*if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		clicked = false;
	}*/
}