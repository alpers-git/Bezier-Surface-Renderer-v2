#include "PickerCursor.h"

PickerCursor::PickerCursor()
{
}
PickerCursor::~PickerCursor()
{
}

void PickerCursor::SetCursorPosCallBack(GLFWwindow * window)
{
	glfwSetCursorPosCallback(window, CursorPositionCallback);
}

void PickerCursor::CursorPositionCallback(GLFWwindow * window, double x_pos, double y_pos)
{
	/*int width, height;
	glfwGetWindowSize(window, &width, &height);
	float x = (2.0f * x_pos) / width - 1.0f;
	float y = 1.0f - (2.0f * y_pos) / height;

	glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);

	//std::cout << "x: " << x << ", y: " << y <<std::endl;
	glm::vec4 ray_eye = glm::inverse(m_projection_mat) * ray_clip;*/
}