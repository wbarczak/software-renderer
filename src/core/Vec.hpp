#pragma once

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <iostream>

template <typename T, size_t N> class Vec
{
public:
	Vec(std::initializer_list<T> data)
	{
		assert(data.size() == N && "A vector of a spefic size needs an initializer list of that size");

		size_t dataIt = 0;
		for (auto num : data) m_Data[dataIt++] = num;
	}

	Vec(T x = 0, T y = 0) requires(N == 2) : m_Data{ x, y } {}
	Vec(T x = 0, T y = 0, T z = 0) requires(N == 3) : m_Data{ x, y, z } {}
	Vec(T x = 0, T y = 0, T z = 0, T w = 0) requires(N == 4) : m_Data{ x, y, z, w } {}

	T& x() requires(N > 0) { return m_Data[0]; }
	T& y() requires(N > 1) { return m_Data[1]; }
	T& z() requires(N > 2) { return m_Data[2]; }
	T& w() requires(N > 3) { return m_Data[3]; }
	const T& x() const requires(N > 0) { return m_Data[0]; }
	const T& y() const requires(N > 1) { return m_Data[1]; }
	const T& z() const requires(N > 2) { return m_Data[2]; }
	const T& w() const requires(N > 3) { return m_Data[3]; }

	template <typename U, size_t M>
	explicit Vec(const Vec<U, M>& v)
	{
		static_assert(N <= M && "Cannot construct a larger vector from a smaller one");

		for (size_t i = 0; i < N; ++i)
		{
			m_Data[i] = static_cast<T>(v[i]);
		}
	}

	template <typename U, size_t M>
	explicit operator Vec<U, M>() const
	{
		static_assert(M <= N && "Cannot cast to a larger vector");

		Vec<U, M> out;
		for (size_t i = 0; i < M; ++i)
		{
			out[i] = static_cast<U>(m_Data[i]);
		}

		return out;
	}

	T& operator[](const size_t i) { assert(i < N); return m_Data[i]; }
	const T& operator[](const size_t i) const { assert(i < N); return m_Data[i]; }

	T dot(const Vec& v) const;
	T length() const { return sqrt(dot(*this)); }
	Vec& normalize();
	Vec normalized() const;

	Vec cross(const Vec& v) const requires (N == 3)
	{
		return Vec(
			y() * v.z() - z() * v.y(),
			z() * v.x() - x() * v.z(),
			x() * v.y() - y() * v.x()
		);
	}

private:

	T m_Data[N]{};
};

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const Vec<T, N>& v) {
	os << '(' << v[0];
	for (size_t i = 1; i < N; ++i) os << ", " << v[i];
	return os << ')';
}

template <typename T, size_t N>
Vec<T, N> operator-(const Vec<T, N>& v)
{
	Vec<T, N> out;
	for (size_t i = 0; i < N; ++i) out[i] = -v[i];
	return out;
}

template <typename T, size_t N>
Vec<T, N> operator+(const Vec<T, N>& a, const Vec<T, N>& b)
{
	Vec<T, N> out;
	for (size_t i = 0; i < N; ++i) out[i] = a[i] + b[i];
	return out;
}

template <typename T, size_t N>
Vec<T, N> operator-(const Vec<T, N>& a, const Vec<T, N>& b)
{
	Vec<T, N> out;
	for (size_t i = 0; i < N; ++i) out[i] = a[i] - b[i];
	return out;
}

template <typename T, size_t N>
Vec<T, N> operator*(const Vec<T, N>& a, const Vec<T, N>& b)
{
	Vec<T, N> out;
	for (size_t i = 0; i < N; ++i) out[i] = a[i] * b[i];
	return out;
}

template <typename T, size_t N>
Vec<T, N> operator/(const Vec<T, N>& a, const Vec<T, N>& b)
{
	Vec<T, N> out;
	for (size_t i = 0; i < N; ++i)
	{
		assert(b[i] != 0);
		out[i] = a[i] / b[i];
	}
	return out;
}

template <typename T, size_t N>
Vec<T, N>& operator+=(Vec<T, N>& a, const Vec<T, N>& b)
{
	for (size_t i = 0; i < N; ++i) a[i] += b[i];
	return a;
}

template <typename T, size_t N>
Vec<T, N>& operator-=(Vec<T, N>& a, const Vec<T, N>& b)
{
	for (size_t i = 0; i < N; ++i) a[i] -= b[i];
	return a;
}

template <typename T, size_t N>
Vec<T, N>& operator*=(Vec<T, N>& a, const Vec<T, N>& b)
{
	for (size_t i = 0; i < N; ++i) a[i] *= b[i];
	return a;
}

template <typename T, size_t N>
Vec<T, N>& operator/=(Vec<T, N>& a, const Vec<T, N>& b)
{
	for (size_t i = 0; i < N; ++i)
	{
		assert(b[i] != 0);
		a[i] /= b[i];
	}
	return a;
}

template <typename T, size_t N>
Vec<T, N> operator*(const Vec<T, N>& v, const T num)
{
	Vec<T, N> out;
	for (size_t i = 0; i < N; ++i) out[i] = v[i] * num;
	return out;
}

template <typename T, size_t N>
Vec<T, N> operator*(const T num, const Vec<T, N>& v)
{
	Vec<T, N> out;
	for (size_t i = 0; i < N; ++i) out[i] = num * v[i];
	return out;
}

template <typename T, size_t N>
Vec<T, N> operator/(const Vec<T, N>& v, const T num)
{
	assert(num != 0);

	Vec<T, N> out;
	for (size_t i = 0; i < N; ++i)
	{
		out[i] = v[i] / num;
	}
	return out;
}

template <typename T, size_t N>
Vec<T, N>& operator*=(Vec<T, N>& v, const T num)
{
	for (size_t i = 0; i < N; ++i) v[i] *= num;
	return v;
}

template <typename T, size_t N>
Vec<T, N>& operator/=(Vec<T, N>& v, const T num)
{
	assert(num != 0);

	for (size_t i = 0; i < N; ++i)
	{
		v[i] /= num;
	}
	return v;
}

template <typename T, size_t N>
T Vec<T, N>::dot(const Vec<T, N>& v) const
{
	T out = 0;
	for (size_t i = 0; i < N; ++i) out += m_Data[i] * v[i];
	return out;
}

template <typename T, size_t N>
Vec<T, N>& Vec<T, N>::normalize()
{
	bool allZeros = true;
	for (size_t i = 0; i < N; ++i)
	{
		if (m_Data[i] != 0)
		{
			allZeros = false;
			break;
		}
	}

	if (allZeros) return *this;

	return *this /= length();
}

template <typename T, size_t N>
Vec<T, N> Vec<T, N>::normalized() const
{
	bool allZeros = true;
	for (size_t i = 0; i < N; ++i)
	{
		if (m_Data[i] != 0)
		{
			allZeros = false;
			break;
		}
	}

	if (allZeros) return *this;

	return *this / length();
}

using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;
using Vec4f = Vec<float, 4>;

using Vec2i = Vec<int32_t, 2>;
using Vec3i = Vec<int32_t, 3>;
using Vec4i = Vec<int32_t, 4>;