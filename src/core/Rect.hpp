#pragma once

#include "Vec.hpp"

struct Cir;

struct Rect
{
	Vec2f pos;
	float w;
	float h;

	Rect(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f) noexcept :
		pos(x, y), w(width), h(height) {}
	Rect(Vec2f position, float width, float height) noexcept :
		pos(position), w(width), h(height) {}

	float& x() { return pos.x(); }
	float& y() { return pos.y(); }

	inline bool colide(const Vec2f& v) noexcept;
	inline bool colide(const Rect& r) noexcept;
	inline bool colide(const Cir& c) noexcept;

	inline Vec2f resolve(const Rect& r) noexcept;
	inline Vec2f resolve(const Cir& c) noexcept;
};

inline std::ostream& operator<<(std::ostream& os, const Rect& r) {
	return os << "(" << r.pos.x() << ", " << r.pos.y() << ", " << r.w << ", " << r.h << ")";
}