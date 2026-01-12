#pragma once

#include <algorithm>
#include <cstdint>

#include "glm.hpp"

#include "Core.hpp"

struct Shader
{
	glm::mat4 M{}, P{};
	Col c[3]{};
	glm::vec3 tri[3]{};

	Shader() = default;
	virtual ~Shader() = default;
	virtual Col fragment(glm::vec3 barycentric) = 0;
	virtual glm::vec4 vertex(glm::vec3 v, size_t i) = 0;
};

struct BasicVertex : Shader
{
	BasicVertex() = default;
	virtual ~BasicVertex() = default;
	virtual glm::vec4 vertex(glm::vec3 v, size_t i) override
	{
		tri[i] = v;
		return P * M * glm::vec4(v, 1.0f);
	}
};

struct NiceVertex : Shader
{
	NiceVertex() = default;
	virtual ~NiceVertex() = default;
	virtual glm::vec4 vertex(glm::vec3 v, size_t i) override
	{
		glm::vec4 position = M * glm::vec4(v, 1.0f);
		tri[i] = position;
		return P * position;
	}
};

struct ColorShader : BasicVertex
{
	ColorShader() = default;
	virtual ~ColorShader() = default;
	virtual Col fragment(glm::vec3 bar) override
	{
		return Col(
			c[0].r * bar.x + c[1].r * bar.y + c[2].r * bar.z,
			c[0].g * bar.x + c[1].g * bar.y + c[2].g * bar.z,
			c[0].b * bar.x + c[1].b * bar.y + c[2].b * bar.z
		);
	}
};

struct LambertLighting : BasicVertex
{
	glm::vec3 l{};

	LambertLighting() = default;
	virtual ~LambertLighting() = default;
	virtual Col fragment(glm::vec3 bar) override
	{
		glm::vec3 facing = glm::normalize(glm::cross(tri[1] - tri[0], tri[2] - tri[0]));
		float lightStrength = std::max(0.0f, glm::dot(facing, l));
		return Col(
			(c[0].r * bar.x + c[1].r * bar.y + c[2].r * bar.z) * lightStrength,
			(c[0].g * bar.x + c[1].g * bar.y + c[2].g * bar.z) * lightStrength,
			(c[0].b * bar.x + c[1].b * bar.y + c[2].b * bar.z) * lightStrength
		);
	}
};

struct NeatLighting : NiceVertex
{
	glm::vec3 l{};
	float ambient{};

	NeatLighting() = default;
	virtual ~NeatLighting() = default;
	virtual Col fragment(glm::vec3 bar) override
	{
		glm::vec3 triangleNormal = glm::normalize(glm::cross(tri[1] - tri[0], tri[2] - tri[0]));
		glm::vec3 reflectedDirection = glm::normalize(triangleNormal * glm::dot(triangleNormal, l) * 2.0f - l);

		float diffusion = std::max(0.0f, glm::dot(triangleNormal, l));
		float specular = std::pow(std::max(0.0f, reflectedDirection.z), 35.0f);

		float lightStrength = std::min(1.0f, ambient + 0.4f * diffusion + 0.9f * specular);

		return Col(
			(c[0].r * bar.x + c[1].r * bar.y + c[2].r * bar.z) * lightStrength,
			(c[0].g * bar.x + c[1].g * bar.y + c[2].g * bar.z) * lightStrength,
			(c[0].b * bar.x + c[1].b * bar.y + c[2].b * bar.z) * lightStrength
		);
	}
};

struct DummyShader : BasicVertex
{
	int32_t counter{};

	virtual ~DummyShader() = default;
	virtual Col fragment(glm::vec3 bar) override
	{
		++counter;
		return Colors::Black;
	}
};