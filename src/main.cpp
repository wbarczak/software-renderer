#include <cstdlib>
#include <iostream>
#include <numbers>
#include "MiniFB.h"
#include "Core.hpp"
#include "Model.hpp"
#include "RenderSurface.hpp"

int32_t main()
{
	constexpr int32_t width = 720;
	constexpr int32_t height = width;

	RenderSurface buffer(width, height);
	Model model("diablo3_pose.obj");

	mfb_window* window = mfb_open_ex("Software Renrerer", width, height, 0);

	SecClock clock;
	float angle = 0;
	do
	{
		buffer.clear(Colors::Black);
		buffer.clearZBuffer();

		angle -= clock.restart() * std::numbers::pi / 6.0f;
		buffer.renderModel(model, angle);

		int32_t state = mfb_update_ex(window, buffer.rawZBuffer().data(), width, height);

		if (state < 0)
		{
			window = nullptr;
			break;
		}
	} while (mfb_wait_sync(window));

	/*auto filename = "framebuffer.ppm";
	auto bufferFilename = "framebuffer_z_buffer.ppm";
	buffer.savePpmUpsideDown(filename);
	buffer.rawZBuffer().savePpmUpsideDown(bufferFilename);
	std::cout << "Image saved to '" << filename << "', buffer saved to: '" << bufferFilename << "'.\n";*/
}