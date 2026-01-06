#pragma once

#include "Vec.hpp"

struct Rect;

struct Cir
{
	Vec2f pos;
	float rad;

	Cir(float x = 0.0f, float y = 0.0f, float radius = 0.0f) noexcept :
		pos(x, y), rad(radius) {}
	Cir(Vec2f position, float radius) noexcept :
		pos(position), rad(radius) {}

	float& x() { return pos.x(); }
	float& y() { return pos.y(); }

	inline bool colide(const Vec2f& v) noexcept;
	inline bool colide(const Rect& r) noexcept;
	inline bool colide(const Cir& c) noexcept;

	inline Vec2f resolve(const Rect& r) noexcept;
	inline Vec2f resolve(const Cir& c) noexcept;
};

inline std::ostream& operator<<(std::ostream& os, const Cir& c) {
	return os << "(" << c.pos.x() << ", " << c.pos.y() << ", " << c.rad << ")";
}