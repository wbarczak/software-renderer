#include "ZBuffer.hpp"

#include <iostream>

bool ZBuffer::depthTest(int32_t x, int32_t y, float depth)
{
	int32_t testPoint = y * m_Width + x;

	if (testPoint > m_Height * m_Width || testPoint < 0) return false;

	if (m_Depth[testPoint] > depth) return false;

	m_Depth[testPoint] = depth;
	return true;
}

void ZBuffer::clear()
{
	const size_t bufferSize = m_Width * m_Height;

	for (size_t i = 0; i < bufferSize; ++i) m_Depth[i] = m_MaxDepth;
}