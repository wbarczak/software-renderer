#pragma once

#include <vector>
#include <cstdint>
#include <limits>
#include "PixelGrid.hpp"

class ZBuffer
{
public:
	
	ZBuffer(int32_t width, int32_t height) :
		m_Depth(width * height, -std::numeric_limits<float>::max()),
		m_Width(width),
		m_Height(height) {}

	float& at(size_t x, size_t y) { return m_Depth[((m_Height - 1 - y) * m_Width + x)]; }
	float& at(Vec2i pos) { return at(pos.x(), pos.y()); }
	const float& at(size_t x, size_t y) const { return m_Depth[((m_Height - 1 - y) * m_Width + x)]; }
	const float& at(Vec2i pos) const { return at(pos.x(), pos.y()); }

	void clear();
	PixelGrid getVisual() const;

private:

	std::vector<float> m_Depth;
	int32_t m_Width;
	int32_t m_Height;
};