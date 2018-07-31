#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
		Material b_material(glm::vec4(0.0f, 0.05f, 0.05, 1.0f),
				glm::vec4(0.4f, 0.5f, 0.5f, 1.0f),
				glm::vec4(0.04f, 0.7f, 0.7f, 1.0f), 0.78f, "u_Material");

		BezierSurface* b_surface = new BezierSurface(5, 5, control_points, b_material);

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray vArray;
		VertexBuffer vBuffer(&b_surface->GetVertices().at(0), sizeof(Vertex) * b_surface->GetVertices().size());

		VertexBufferLayout layout;
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

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		shader.SetUniform4f("u_LightPosition", 0.0f, 0.0f, 18.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/rubber.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

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

		int num_indices = 36;
		glm::vec4 color(0.3f, 0.1f, 0.9f, 1.0f);
		bool increment = true;

		bool show_demo_window = true;
		bool show_another_window = false;

		glm::vec3 cam_pos(0.0f, 0.0f, -4.0f);
		glm::vec3 axisa = glm::vec3(0.0f, 1.0f, 0.6f);
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		float ra = 0.0f;
		float a = 0.0f;

		GLCall(glClearColor(0.86f, 0.86f, 0.86f, 1.0f));

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();

			shader.Bind();
			{
				model = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
				view = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
				view = glm::rotate(view, glm::radians(ra), axisa);
				mvp = proj * view * model;
				glm::mat4 normal_matrix = glm::inverse(glm::transpose(mvp));

				shader.SetUniform4f("u_LightPosition", cam_pos.x, cam_pos.y, cam_pos.z, 1.0f);
				shader.SetUniformMat4f("u_MVP", mvp);
				shader.SetUniformMat4f("u_NormalMat", normal_matrix);

				control_points[2][2].z = a;
				b_surface->EvaluateBezierSurface();//Recalculating surface

				VertexBuffer vBuffer = VertexBuffer(&b_surface->GetVertices().at(0), sizeof(Vertex) * b_surface->GetVertices().size());
				vArray.AddBuffer(vBuffer, layout);

				for (int i = 0; i < b_surface->GetIndices().size(); i += 4)
				{
					shader.SetMaterial(b_material.GetUniformName(), b_material);
					IndexBuffer iBuffer(&b_surface->GetIndices().at(i), 4);
					renderer.Draw(vArray, iBuffer, shader, GL_TRIANGLE_FAN);

					shader.SetMaterial(b_material.GetUniformName(), Material());
					renderer.Draw(vArray, iBuffer, shader, GL_LINE_LOOP);
				}

				//b_surface->Draw(renderer, shader, vArray, false);

			}

			{
				ImGui::Text("Transforms");                           // Display some text (you can use a format string too)
				ImGui::SliderFloat3("Camera Pos", &cam_pos.x, -10.0f, 10.0f);
				ImGui::SliderFloat3("Camera Rot", &axisa.x, -40.0f, 40.0f);
				ImGui::SliderFloat("Camera Rot angle", &ra, 0.0f, 360.0f);
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