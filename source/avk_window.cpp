#include "avk_window.h"

#include <stdexcept>

namespace avk
{
	/// <summary>
	/// Window Constructor
	/// </summary>
	/// <param name="w">Window Width</param>
	/// <param name="h">Window Height</param>
	/// <param name="name">Window Name</param>
	AvkWindow::AvkWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }
	{
		initWindow();
	}

	/// <summary>
	/// AvkWindow destructor.
	/// </summary>
	AvkWindow::~AvkWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void AvkWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto avkWindow = reinterpret_cast<AvkWindow*>(glfwGetWindowUserPointer(window));
		avkWindow->framebufferResized = true;
		avkWindow->width = width;
		avkWindow->height = height;
	}

	/// <summary>
	/// Private - Initialize Window
	/// </summary>
	void AvkWindow::initWindow()
	{
		glfwInit();

		// Removes OpenGL from API
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		// Prevents Window from being resizable
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		// Create the window!
		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void AvkWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface!");
		}
	}
}