#pragma once

#include <functional>

#include "PixelGrid.hpp"
#include "ZBuffer.hpp"
#include "Shader.hpp"

class Model;

class RenderSurface
{
public:

	RenderSurface(int32_t width, int32_t height, float maxDepth = -1000.0f,Col backgroundColor = Colors::Black);

	void setViewport(int32_t x, int32_t y, int32_t w, int32_t h);
	void setPerspective(float f);
	void setLookat(glm::fvec3 eye, glm::fvec3 center, glm::fvec3 up);

	void clear(Col color = Colors::Black) { m_PixelGrid.clear(color); m_ZBuffer.clear(); }
	void put(int32_t x, int32_t y, Col color = Colors::White) { m_PixelGrid.put(x, y, color); }
	void put(glm::vec2 pos, Col color = Colors::White) { m_PixelGrid.put(pos.x, pos.y, color); }

	void line(glm::vec2 a, glm::vec2 b, Col color = Colors::White);
	void rastorize(glm::vec4 v[3], Col c[3], Shader* shader);

	void savePpm(const char* path) const { m_PixelGrid.savePpm(path); }
	void savePpmUpsideDown(const char* path) const { m_PixelGrid.savePpmUpsideDown(path); };

	void renderModel(Model& model);

	uint8_t* data() { return m_PixelGrid.data(); }

private:

	PixelGrid m_PixelGrid;
	ZBuffer m_ZBuffer;
	glm::mat4 m_ModelView;
	glm::mat4 m_Viewport;
	glm::mat4 m_Perspective;
};