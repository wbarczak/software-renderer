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

	for (size_t i = 0; i < bufferSize; ++i) m_Depth[i] = -std::numeric_limits<float>::max();
}

PixelGrid ZBuffer::getVisual() const
{
	const size_t bufferSize = m_Width * m_Height;
	float minimum = 0;
	for (size_t i = 0; i < bufferSize; ++i)
	{
		if (m_Depth[i] == -std::numeric_limits<float>::max()) continue;

		minimum = std::min(m_Depth[i], minimum);
	}

	PixelGrid out(m_Width, m_Height);
	for (size_t y = 0; y < m_Height; ++y)
	{
		for (size_t x = 0; x < m_Width; ++x)
		{
			uint8_t value = m_Depth[y * m_Width + x] / minimum * 255;

			out.put(x, y,
				Col(value, value, value)
			);
		}
	}

	return out;
}