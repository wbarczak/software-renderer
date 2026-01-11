#pragma once

#include <vector>
#include <string_view>
#include <cstdint>

#include "glm.hpp"

#include "Core.hpp"

class PixelGrid
{
public:

	PixelGrid(int32_t width, int32_t height, Col backgroundColor = Colors::Black);
	//PixelGrid(std::string_view path);
	PixelGrid(int32_t width, int32_t height, std::vector<uint8_t>&& pixelData);

	void savePpm(const char* path) const;
	void savePpmUpsideDown(const char* path) const;
	//void saveBmp(const char* path) const;

	void clear(Col color);
	int32_t width() const { return m_Width; }
	int32_t height() const { return m_Height; }
	uint8_t* data() { return m_Pixels.data(); };
	void put(int32_t x, int32_t y, Col color = Colors::White);
	void put(glm::vec2 pos, Col color = Colors::White) { put(pos.x, pos.y, color); }
	void putHorizontalLine(int32_t y, int32_t firstX, int32_t lastX, Col color = Colors::White);
	Col get(int32_t x, int32_t y);
	Col get(glm::vec2 pos) { return get(pos.x, pos.y); }

private:

	std::vector<uint8_t> m_Pixels;
	int32_t m_Width;
	int32_t m_Height;
};