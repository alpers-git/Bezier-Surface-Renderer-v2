#pragma once
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include <iostream>

class PickerCursor
{
public:
	PickerCursor();
	~PickerCursor();

	void SetCursorPosCallBack(GLFWwindow* window);
	inline void SetProjectionMatrix(glm::mat4 mat) { m_projection_mat = mat; }

private:
	glm::mat4 m_projection_mat;
	static void CursorPositionCallback(GLFWwindow * window, double x_pos, double y_pos);

};
