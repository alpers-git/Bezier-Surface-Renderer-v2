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
	
}