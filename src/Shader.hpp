#pragma once

#include <algorithm>
#include <cstdint>

#include "glm.hpp"

#include "Core.hpp"

struct Shader
{
	virtual ~Shader() = default;
	virtual Col fragment(glm::vec3 barycentric, Col colors[3]) = 0;
};

struct BaseShader : Shader
{
	virtual ~BaseShader() = default;
	virtual Col fragment(glm::vec3 bar, Col c[3]) override
	{
		return Col(
			c[0].r * bar.x + c[1].r * bar.y + c[2].r * bar.z,
			c[0].g * bar.x + c[1].g * bar.y + c[2].g * bar.z,
			c[0].b * bar.x + c[1].b * bar.y + c[2].b * bar.z
		);
	}
};

struct ShadowShader : Shader
{
	glm::vec3 tri[3];
	glm::vec3 l;

	virtual ~ShadowShader() = default;
	virtual Col fragment(glm::vec3 bar, Col c[3]) override
	{
		glm::vec3 facing = glm::normalize(glm::cross(tri[1] - tri[0], tri[2] - tri[0]));
		uint8_t lightStrength = std::max(0.0f, glm::dot(facing, l)) * 255;
		return Col(lightStrength, lightStrength, lightStrength);
	}
};