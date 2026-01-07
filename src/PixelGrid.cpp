#include <fstream>

#include "PixelGrid.hpp"
#include "Core.hpp"

PixelGrid::PixelGrid(int32_t width, int32_t height, Col backgroundColor) :
	m_Width(width),
	m_Height(height)
{
	const size_t pixelCount = m_Width * m_Height;
	m_Pixels.reserve(4 * pixelCount);

	for (int32_t i = 0; i < pixelCount; ++i)
	{
		m_Pixels.push_back(backgroundColor.b);
		m_Pixels.push_back(backgroundColor.g);
		m_Pixels.push_back(backgroundColor.r);
		m_Pixels.push_back(backgroundColor.a);
	}
}

PixelGrid::PixelGrid(int32_t width, int32_t height, std::vector<uint8_t>&& pixelData) : 
	m_Width(width),
	m_Height(height)
{
	m_Pixels = std::move(pixelData);
}

void PixelGrid::savePpm(const char* path) const
{
	std::ofstream file(path, std::ios::binary);
	file << "P6\n";
	file << m_Width << ' ' << m_Height << '\n';
	file << "255\n";

	constexpr size_t colorComponentSize = sizeof(uint8_t);
	const size_t pixelCount = m_Pixels.size() / 4;

	for (int32_t i = 0; i < pixelCount; ++i)
	{
		file.write(reinterpret_cast<const char*>(&m_Pixels[4 * i + 2]), colorComponentSize);
		file.write(reinterpret_cast<const char*>(&m_Pixels[4 * i + 1]), colorComponentSize);
		file.write(reinterpret_cast<const char*>(&m_Pixels[4 * i]), colorComponentSize);
	}
}

void PixelGrid::savePpmUpsideDown(const char* path) const
{
	std::ofstream file(path, std::ios::binary);
	file << "P6\n";
	file << m_Width << ' ' << m_Height << '\n';
	file << "255\n";

	constexpr size_t colorComponentSize = sizeof(uint8_t);

	for (int32_t y = m_Height - 1; y >= 0; --y) for (int32_t x = 0; x < m_Width; ++x)
	{
		file.write(reinterpret_cast<const char*>(&m_Pixels[4 * (m_Width * y + x) + 2]), colorComponentSize);
		file.write(reinterpret_cast<const char*>(&m_Pixels[4 * (m_Width * y + x) + 1]), colorComponentSize);
		file.write(reinterpret_cast<const char*>(&m_Pixels[4 * (m_Width * y + x)]), colorComponentSize);
	}
}

void PixelGrid::clear(Col color)
{
	const size_t pixelCount = m_Width * m_Height;

	for (int32_t i = 0; i < pixelCount; ++i)
	{
		m_Pixels[4 * i] = color.b;
		m_Pixels[4 * i + 1] = color.g;
		m_Pixels[4 * i + 2] = color.r;
		m_Pixels[4 * i + 3] = color.a;
	}
}

void PixelGrid::put(int32_t x, int32_t y, Col color)
{
	if (x < 0 || x >= m_Width || y < 0 || y >= m_Height) return;

	const size_t pixelPosition = 4 * ((m_Height - 1 - y) * m_Width + x);

	m_Pixels[pixelPosition] = color.b;
	m_Pixels[pixelPosition + 1] = color.g;
	m_Pixels[pixelPosition + 2] = color.r;
	m_Pixels[pixelPosition + 3] = color.a;
}

void PixelGrid::putHorizontalLine(int32_t y, int32_t firstX, int32_t lastX, Col color)
{
	if (y < 0 || y >= m_Height || firstX > lastX) return;

	firstX = std::max(0, firstX);
	lastX = std::min(m_Width, lastX);

	for (int32_t x = firstX; x <= lastX; ++x)
	{
		const size_t pixelPosition = 4 * ((m_Height - 1 - y) * m_Width + x);

		m_Pixels[pixelPosition] = color.b;
		m_Pixels[pixelPosition + 1] = color.g;
		m_Pixels[pixelPosition + 2] = color.r;
		m_Pixels[pixelPosition + 3] = color.a;
	}
}

Col PixelGrid::get(int32_t x, int32_t y)
{
	const size_t pixelPosition = 4 * ((m_Height - 1 - y) * m_Width + x);
	if (pixelPosition >= m_Pixels.size()) return Colors::White;

	return Col(&m_Pixels.data()[pixelPosition]);
}