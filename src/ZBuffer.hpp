#pragma once

#include <vector>
#include <cstdint>
#include <limits>

#include "glm.hpp"

#include "PixelGrid.hpp"

class ZBuffer
{
public:
	
	ZBuffer(int32_t width, int32_t height, float maxDepth = -1000.0f) :
		m_Depth(width * height, maxDepth),
		m_Width(width),
		m_Height(height),
		m_MaxDepth(maxDepth) {}

	bool depthTest(int32_t x, int32_t y, float depth);
	bool depthTest(glm::vec2 pos, float depth) { return depthTest(pos.x, pos.y, depth); }

	void clear();

private:

	std::vector<float> m_Depth;
	int32_t m_Width;
	int32_t m_Height;
	float m_MaxDepth;
};