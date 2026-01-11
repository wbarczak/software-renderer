#pragma once

#include <limits>
#include <cstdint>
#include <cassert>

#include "Col.hpp"

class Random
{
public:

	Random(uint32_t seed = 1) : m_Current(seed != 0? seed : 1) {}
	void setSeed(uint32_t seed) { m_Current = seed != 0? seed : 1; }

	uint32_t randomInt()
	{
		m_Current ^= m_Current << 13;
		m_Current ^= m_Current >> 17;
		m_Current ^= m_Current << 5;
		return m_Current * 0x9E3779BBu;
	}

	uint32_t randomInt(uint32_t starting, uint32_t ending)
	{
		assert(ending > starting);

		const uint32_t range = ending - starting;
		const uint32_t max = std::numeric_limits<uint32_t>::max() / range * range;

		uint32_t out = randomInt();
		while (out >= max)
		{
			out = randomInt();
		}

		return out % range + starting;
	}

	float randomFloat() { return (randomInt() >> 8) * (1.0f / 16777216.0f);; }

	Col randomColor() { return Col(randomInt(0, 256), randomInt(0, 256), randomInt(0, 256)); }

private:

	uint32_t m_Current;
};