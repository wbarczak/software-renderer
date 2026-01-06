#include <algorithm>

#include "Rect.hpp"
#include "Cir.hpp"

bool Rect::colide(const Vec2f& v) noexcept
{
	return v.x() >= pos.x()
		&& v.x() <= pos.x() + w
		&& v.y() >= pos.y()
		&& v.y() <= pos.y() + h;
}

bool Rect::colide(const Rect& r) noexcept
{
	return pos.x() <= r.pos.x() + r.w
		&& pos.x() + w >= r.pos.x()
		&& pos.y() <= r.pos.y() + r.h
		&& pos.y() + h >= r.pos.y();
}

bool Rect::colide(const Cir& c) noexcept
{
	Vec2f testPoint(
		std::clamp(c.pos.x(), pos.x(), pos.x() + w),
		std::clamp(c.pos.y(), pos.y(), pos.y() + h)
	);
	Vec2f difference = testPoint - c.pos;
	return difference.dot(difference) <= c.rad * c.rad;
}

bool Cir::colide(const Vec2f& v) noexcept
{
	return (Vec2f(pos.x(), pos.y()) - v).length() <= rad;
}

bool Cir::colide(const Rect& r) noexcept
{
	Vec2f testPoint(
		std::clamp(pos.x(), r.pos.x(), r.pos.x() + r.w),
		std::clamp(pos.y(), r.pos.y(), r.pos.y() + r.h)
	);
	Vec2f difference = testPoint - pos;
	return difference.dot(difference) <= rad * rad;
}

bool Cir::colide(const Cir& c) noexcept
{
	return (pos - c.pos).length() <= rad + c.rad;
}

// All the algorithms for colision resolution I shamelessly stole off the internet so I can't explain these

Vec2f Rect::resolve(const Rect& r) noexcept
{
	bool noOverlap =
		pos.x() >= r.pos.x() + r.w ||
		pos.x() + w <= r.pos.x() ||
		pos.y() >= r.pos.y() + r.h ||
		pos.y() + h <= r.pos.y();

	if (noOverlap) return Vec2f();

	float left = (r.pos.x() + r.w) - pos.x();
	float right = (pos.x() + w) - r.pos.x();
	float top = (r.pos.y() + r.h) - pos.y();
	float bottom = (pos.y() + h) - r.pos.y();

	float minX = std::min(left, right);
	float minY = std::min(top, bottom);

	Vec2f resolution;

	if (minX < minY)
	{
		if (left < right)
		{
			resolution.x() = -left;
		}
		else
		{
			resolution.x() = right;
		}
	}
	else
	{
		if (top < bottom)
		{
			resolution.y() = -top;
		}
		else
		{
			resolution.y() = bottom;
		}
	}

	return resolution;
}

Vec2f Rect::resolve(const Cir& c) noexcept
{
	Vec2f closest(
		std::clamp(c.pos.x(), pos.x(), pos.x() + w),
		std::clamp(c.pos.y(), pos.y(), pos.y() + h)
	);
	Vec2f difference = closest - c.pos;
	float distanceSquared = difference.dot(difference);

	if (distanceSquared >= c.rad * c.rad)
	{
		return Vec2f();
	}

	float distance = sqrtf(distanceSquared);

	if (distance == 0.0f)
	{
		float left = c.pos.x() - pos.x();
		float right = pos.x() + w - c.pos.x();
		float top = c.pos.y() - pos.y();
		float bottom = pos.y() + h - c.pos.y();

		float minX = std::min(left, right);
		float minY = std::min(top, bottom);

		if (minX < minY)
		{
			if (left < right)
			{
				return Vec2f(c.rad - left, 0.0f);
			}

			return Vec2f(-(c.rad - right), 0.0f);
		}

		if (top < bottom)
		{
			return Vec2f(0.0f, -(c.rad - top));
		}

		return Vec2f(0.0f, -(c.rad - bottom));
	}

	Vec2f normal = difference / distance;
	float penetration = c.rad - distance;

	return normal * penetration;
}

Vec2f Cir::resolve(const Rect& r) noexcept
{
	Vec2f closest(
		std::clamp(pos.x(), r.pos.x(), r.pos.x() + r.w),
		std::clamp(pos.y(), r.pos.y(), r.pos.y() + r.h)
	);
	Vec2f difference = closest - pos;
	float distanceSquared = difference.dot(difference);

	if (distanceSquared >= rad * rad)
	{
		return Vec2f();
	}

	float distance = sqrtf(distanceSquared);

	if (distance == 0.0f)
	{
		float left = pos.x() - r.pos.x();
		float right = r.pos.x() + r.w - pos.x();
		float top = pos.y() - r.pos.y();
		float bottom = r.pos.y() + r.h - pos.y();

		float minX = std::min(left, right);
		float minY = std::min(top, bottom);

		if (minX < minY)
		{
			if (left < right)
			{
				return Vec2f(rad - left, 0.0f);
			}

			return Vec2f(-(rad - right), 0.0f);
		}

		if (top < bottom)
		{
			return Vec2f(0.0f, -(rad - top));
		}

		return Vec2f(0.0f, -(rad - bottom));
	}

	Vec2f normal = difference / distance;
	float penetration = rad - distance;

	return normal * -penetration;
}

Vec2f Cir::resolve(const Cir& c) noexcept
{
	Vec2f difference = pos - c.pos;
	float distanceSquared = difference.dot(difference);
	float radiusSum = rad + c.rad;

	if (distanceSquared >= radiusSum * radiusSum)
	{
		return Vec2f();
	}

	float distance = sqrtf(distanceSquared);

	if (distance == 0.0f)
	{
		return Vec2f(radiusSum, 0.0f);
	}

	Vec2f normal = difference / distance;
	float penetration = radiusSum - distance;

	return normal * penetration;
}