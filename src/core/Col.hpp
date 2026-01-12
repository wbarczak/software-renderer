#pragma once

#include <cstdint>

struct Col
{
	// The color data is BGRA cuz of a minor cashe optimization, the struct cosplays as an RGBA value anyways

	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;

	constexpr Col(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255) :
		r(r), g(g), b(b), a(a) {}
	constexpr Col(uint8_t* bgra) :
		b(bgra[0]), g(bgra[1]), r(bgra[2]), a(bgra[3]) {}
};

namespace Colors
{
	constexpr Col Black(0, 0, 0);
	constexpr Col White(255, 255, 255);
	constexpr Col Gray(30, 30, 30);
	constexpr Col LightGray(60, 60, 60);
	constexpr Col DarkTint(0, 0, 0, 50);
	constexpr Col Red(255, 0, 0);
	constexpr Col Orange(255, 153, 0);
	constexpr Col Yellow(255, 255, 0);
	constexpr Col Green(0, 255, 0);
	constexpr Col LightBlue(0, 255, 255);
	constexpr Col Blue(0, 0, 255);
	constexpr Col Purple(255, 0, 255);
}