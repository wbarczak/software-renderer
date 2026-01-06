#pragma once

#include <chrono>

class SecClock
{
public:

	SecClock() : m_StartingTime(std::chrono::steady_clock::now()) {}

	float restart()
	{
		auto now = std::chrono::steady_clock::now();
		float difference = std::chrono::duration<float>(now - m_StartingTime).count();
		m_StartingTime = now;

		return difference;
	}

	float elapsed()
	{
		auto now = std::chrono::steady_clock::now();
		return std::chrono::duration<float>(now - m_StartingTime).count();
	}

private:

	std::chrono::steady_clock::time_point m_StartingTime;
};