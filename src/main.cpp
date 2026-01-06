#include <cstdlib>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Core.hpp"
#include "Model.hpp"
#include "RenderSurface.hpp"

int32_t main()
{
	GLFWwindow* window;

	if (!glfwInit()) return -1;

	constexpr int32_t width = 720;
	constexpr int32_t height = 720;

	window = glfwCreateWindow(width, height, "Software Rastorizer", nullptr, nullptr);
	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

	RenderSurface framebuffer(width, height);

	Model model("diablo3_pose.obj");

	framebuffer.renderModel(model);

	auto filename = "framebuffer.ppm";
	auto bufferFilename = "framebuffer_z_buffer.ppm";
	framebuffer.savePpmUpsideDown(filename);
	framebuffer.rawZBuffer().savePpmUpsideDown(bufferFilename);
	std::cout << "Image saved to '" << filename << "', buffer saved to: '" << bufferFilename << "'.\n";
}