#pragma once

#include "PixelGrid.hpp"

class RenderSurface;

class Texture
{
public:

	Texture(PixelGrid&& pixelGrid) : m_PixelGrid(std::move(pixelGrid)) {}

	int32_t width() const { return m_PixelGrid.width(); }
	int32_t height() const { return m_PixelGrid.height(); }

private:

	friend RenderSurface;

	PixelGrid m_PixelGrid;
};