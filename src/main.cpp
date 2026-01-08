#include <cstdlib>
#include <iostream>
#include <numbers>
#include "MiniFB_cpp.h"
#include "Core.hpp"
#include "Model.hpp"
#include "RenderSurface.hpp"

int32_t main()
{
	constexpr int32_t width = 720;
	constexpr int32_t height = width;

	RenderSurface buffer(width, height);
	Model model("diablo3_pose.obj");

	mfb_window* window = mfb_open_ex("Software Renderer", width, height, 0);

	float dir = 1.0f;
	bool showZBuffer = true;
	mfb_set_keyboard_callback([&dir, &showZBuffer](mfb_window* window, mfb_key key, mfb_key_mod mod, bool isPressed){
		switch (key)
		{
		case KB_KEY_ESCAPE:
			mfb_close(window);
			return;
		case KB_KEY_A:
		case KB_KEY_LEFT:
			dir = -1.0f;
			return;
		case KB_KEY_D:
		case KB_KEY_RIGHT:
			dir = 1.0f;
			return;
		case KB_KEY_SPACE:
			dir = 0.0f;
			return;
		case KB_KEY_Z:
			showZBuffer = true;
			return;
		case KB_KEY_P:
			showZBuffer = false;
			return;
		}
	}, window);

	SecClock elapsedTime;
	int64_t frames = 0;
	SecClock clock;
	float angle = 0;
	do
	{
		buffer.clear(Colors::Black);

		constexpr float secondsPerSpin = 6.0f;
		float frametime = clock.restart();
		angle += dir * frametime * std::numbers::pi * 2.0f / secondsPerSpin;
		buffer.renderModel(model, angle);

		int32_t state;
		if (showZBuffer)
		{
			auto zBuffer = buffer.visualizeZBuffer();
			state = mfb_update_ex(window, zBuffer.data(), width, height);
		}
		else
		{
			state = mfb_update_ex(window, buffer.data(), width, height);
		}

		++frames;

		if (state < 0)
		{
			window = nullptr;
			break;
		}
	} while (mfb_wait_sync(window));

	std::cout << "Average fps: " << frames / elapsedTime.elapsed() << '\n';

	/*auto filename = "framebuffer.ppm";
	auto bufferFilename = "framebuffer_z_buffer.ppm";
	buffer.savePpmUpsideDown(filename);
	buffer.rawZBuffer().savePpmUpsideDown(bufferFilename);
	std::cout << "Image saved to '" << filename << "', buffer saved to: '" << bufferFilename << "'.\n";*/
}