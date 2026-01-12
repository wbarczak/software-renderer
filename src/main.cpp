#include <cstdlib>
#include <iostream>
#include <numbers>

#include "MiniFB_cpp.h"

#include "Core.hpp"
#include "Model.hpp"
#include "RenderSurface.hpp"
#include "Shader.hpp"

int32_t main()
{
	constexpr int32_t width = 800;
	constexpr int32_t height = 800;
	constexpr glm::fvec3 eye{-1.0f, 0.0f, 2.0f};
	constexpr glm::fvec3 center{0.0f, 0.0f, 0.0f};
	constexpr glm::fvec3 up{0.0f, 1.0f, 0.0f};

	RenderSurface buffer(width, height);
	buffer.setLookat(eye, center, up);
	buffer.setPerspective(glm::length(eye - center));
	buffer.setViewport(width / 16, height / 16, width * 7 / 8, height * 7 / 8);

	constexpr Col colors[3]{
		Colors::Blue,
		Colors::LightBlue,
		Colors::Purple
	};
	
	LambertLighting shadowShader{};
	shadowShader.c[0] = colors[0];
	shadowShader.c[1] = colors[1];
	shadowShader.c[2] = colors[2];

	ColorShader normalShader{};
	normalShader.c[0] = colors[0];
	normalShader.c[1] = colors[1];
	normalShader.c[2] = colors[2];

	NeatLighting niceShadowShader{};
	niceShadowShader.c[0] = colors[0];
	niceShadowShader.c[1] = colors[1];
	niceShadowShader.c[2] = colors[2];
	niceShadowShader.ambient = 0.3f;

	DummyShader dummyShader{};

	Model model("diablo3_pose.obj", &niceShadowShader);

	mfb_window* window = mfb_open_ex("Software Renderer", width, height, 0);

	float dir = 0.0f;
	float speed = 2.0f;
	mfb_set_keyboard_callback([&](mfb_window* window, mfb_key key, mfb_key_mod mod, bool isPressed){
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
		case KB_KEY_1:
			speed = 0.5f;
			return;
		case KB_KEY_2:
			speed = 1.0f;
			return;
		case KB_KEY_3:
			speed = 2.0f;
			return;
		case KB_KEY_4:
			speed = 4.0f;
			return;
		case KB_KEY_5:
			speed = 8.0f;
			return;
		case KB_KEY_Z:
			model.setShader(&normalShader);
			return;
		case KB_KEY_X:
			model.setShader(&shadowShader);
			return;
		case KB_KEY_C:
			model.setShader(&niceShadowShader);
			return;
		case KB_KEY_V:
			model.setShader(&dummyShader);
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

		angle += dir * speed * frametime * std::numbers::pi * 2.0f / secondsPerSpin;

		buffer.setLookat(glm::vec3(cosf(angle), 0.0f, sinf(angle)), center, up);
		shadowShader.l = glm::vec3(cosf(angle - 0.75f), 0.0f, sinf(angle - 0.75f));
		niceShadowShader.l = glm::vec3(buffer.getModelView() * glm::vec4(cosf(angle - 0.75f), 0.0f, sinf(angle - 0.75f), 0.0f));
		dummyShader.counter = 0;

		buffer.renderModel(model);

		int32_t state = mfb_update_ex(window, buffer.data(), width, height);

		++frames;

		if (state < 0)
		{
			window = nullptr;
			break;
		}
	} while (mfb_wait_sync(window));

	std::cout << "Average fps: " << frames / elapsedTime.elapsed() << '\n';
	std::cout << "Dummy fragment shader calls: " << dummyShader.counter << '\n';

	/*auto filename = "framebuffer.ppm";
	auto bufferFilename = "framebuffer_z_buffer.ppm";
	buffer.savePpmUpsideDown(filename);
	buffer.rawZBuffer().savePpmUpsideDown(bufferFilename);
	std::cout << "Image saved to '" << filename << "', buffer saved to: '" << bufferFilename << "'.\n";*/
}