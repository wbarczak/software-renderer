#pragma once

#include "PixelGrid.hpp"

class Model;

class RenderSurface
{
public:

	RenderSurface(int32_t width, int32_t height, Col backgroundColor = Colors::Black) :
		m_PixelGrid(width, height, backgroundColor),
		m_ZBuffer(width, height, Colors::Black) {}

	void clear(Col color = Colors::Black) { m_PixelGrid.clear(color); }
	void clearZBuffer() { m_ZBuffer.clear(Colors::Black); }
	void put(int32_t x, int32_t y, Col color = Colors::White) { m_PixelGrid.put(x, y, color); }
	void put(Vec2i pos, Col color = Colors::White) { m_PixelGrid.put(pos.x(), pos.y(), color); }

	void line(Vec2i a, Vec2i b, Col color = Colors::White);
	void triangleFrame(Vec2i a, Vec2i b, Vec2i c, Col color = Colors::White);
	void triangleScanline(Vec2i a, Vec2i b, Vec2i c, Col color = Colors::White);
	void trianglePixelCheck(Vec3i a, Vec3i b, Vec3i c, Col cA = Colors::White, Col cB = Colors::White, Col cC = Colors::White);
	void triangle(Vec3i a, Vec3i b, Vec3i c, Col color = Colors::White) { trianglePixelCheck(a, b, c, color, color, color); }

	void savePpm(const char* path) const { m_PixelGrid.savePpm(path); }
	void savePpmUpsideDown(const char* path) const { m_PixelGrid.savePpmUpsideDown(path); };

	void renderModel(const Model& model);

	PixelGrid& rawPixels() { return m_PixelGrid; }
	PixelGrid& rawZBuffer() { return m_ZBuffer; }

private:

	Vec3f project(Vec3f vertice);

	PixelGrid m_PixelGrid;
	PixelGrid m_ZBuffer;
};