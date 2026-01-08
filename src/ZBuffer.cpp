#include "ZBuffer.hpp"

void ZBuffer::clear()
{
	const size_t bufferSize = m_Width * m_Height;

	for (size_t i = 0; i < bufferSize; ++i) m_Depth[i] = -std::numeric_limits<float>::max();
}

PixelGrid ZBuffer::getVisual() const
{
	const size_t bufferSize = m_Width * m_Height;
	float maximum = 0.0f;
	for (size_t i = 0; i < bufferSize; ++i)
	{
		if (m_Depth[i] == std::numeric_limits<float>::max()) continue;

		maximum = std::max(m_Depth[i], maximum);
	}

	PixelGrid out(m_Width, m_Height);
	for (size_t y = 0; y < m_Height; ++y)
	{
		for (size_t x = 0; x < m_Width; ++x)
		{
			uint8_t value = m_Depth[(m_Height - 1 - y) * m_Width + x] / maximum * 255;
			out.put(x, y,
				Col(value, value, value)
			);
		}
	}

	return out;
}