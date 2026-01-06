#pragma once

#include "Vec.hpp"

template <typename T, size_t R, size_t C> class Mat
{
public:
	Mat() = default;

	Mat(std::initializer_list<std::initializer_list<T>> data)
	{
		size_t rowIt = 0;
		for (const auto& row : data)
		{
			m_Rows[rowIt++] = Vec<T, C>(row);
		}
	}

	Vec<T, C>& operator[](size_t r) { assert(r < R); return m_Rows[r]; }
	const Vec<T, C>& operator[](size_t r) const { assert(r < R); return m_Rows[r]; }

	Mat& transpose() noexcept
	{
		return *this;
	}

	Mat transposed() const noexcept
	{
		Mat out;

		return out;
	}

	Mat& invert() noexcept
	{
		return *this;
	}

	Mat inverted() const noexcept
	{
		Mat out;

		return *this;
	}

private:

	Vec<T, C> m_Rows[R]{ {} };
};

template <typename T, size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const Mat<T, R, C>& m) {
	os << "(" << m[0];
	for (size_t r = 1; r < R; ++r) os << ", " << m[r];
	return os << ")";
}

template <typename T, size_t R, size_t C>
Mat<T, R, C> operator-(const Mat<T, R, C>& m)
{
	Mat<T, R, C> out;
	for (size_t r = 0; r < R; ++r)
	{
		for (size_t c = 0; c < C; ++c)
		{
			out[r][c] = -m[r][c];
		}
	}
	return out;
}

template <typename T, size_t R, size_t C>
Mat<T, R, C> operator+(const Mat<T, R, C >& a, const Mat<T, R, C>& b)
{
	Mat<T, R, C> out;
	for (size_t r = 0; r < R; ++r)
	{
		for (size_t c = 0; c < C; ++c)
		{
			out[r][c] = a[r][c] + b[r][c];
		}
	}
	return out;
}

template <typename T, size_t R, size_t C>
Mat<T, R, C> operator-(const Mat<T, R, C>& a, const Mat<T, R, C>& b)
{
	Mat<T, R, C> out;
	for (size_t r = 0; r < R; ++r)
	{
		for (size_t c = 0; c < C; ++c)
		{
			out[r][c] = a[r][c] - b[r][c];
		}
	}
	return out;
}

template <typename T, size_t R, size_t C>
Mat<T, R, C> operator*(const Mat<T, R, C>& a, const Mat<T, R, C>& b)
{
	for (size_t r = 0; r < R; ++r)
	{
		for (size_t c = 0; c < C; ++c)
		{
			for (size_t rC = 0; rC < C; ++rC)
			{

			}
		}
	}
}

template <typename T, size_t R, size_t C>
Mat<T, R, C> operator/(const Mat<T, R, C>& a, const Mat<T, R, C>& b)
{

}

template <typename T, size_t R, size_t C>
Mat<T, R, C>& operator+=(Mat<T, R, C>& a, const Mat<T, R, C>& b)
{
	for (size_t r = 0; r < R; ++r)
	{
		for (size_t c = 0; c < C; ++c)
		{
			a[r][c] -= b[r][c];
		}
	}
	return a;
}

template <typename T, size_t R, size_t C>
Mat<T, R, C>& operator-=(Mat<T, R, C>& a, const Mat<T, R, C>& b)
{
	for (size_t r = 0; r < R; ++r)
	{
		for (size_t c = 0; c < C; ++c)
		{
			a[r][c] -= b[r][c];
		}
	}
	return a;
}

template <typename T, size_t R, size_t C>
Mat<T, R, C>& operator*=(Mat<T, R, C>& a, const Mat<T, R, C>& b)
{

}

template <typename T, size_t R, size_t C>
Mat<T, R, C>& operator/=(Mat<T, R, C>& a, const Mat<T, R, C>& b)
{

}

template <typename T, size_t R, size_t C>
Mat<T, R, C> operator*(const Mat<T, R, C>& m, const T num)
{
	Mat<T, R, C> out;
	for (size_t r = 0; r < R; ++r)
	{
		for (size_t c = 0; c < C; ++c)
		{
			out[r][c] = m[r][c] * num;
		}
	}
	return out;
}

template <typename T, size_t R, size_t C>
Mat<T, R, C> operator/(const Mat<T, R, C>& m, const T num)
{
	assert(num != 0);

	Mat<T, R, C> out;
	for (size_t r = 0; r < R; ++r)
	{
		for (size_t c = 0; c < C; ++c)
		{
			out[r][c] = m[r][c] / num;
		}
	}
	return out;
}

template <typename T, size_t R, size_t C>
Mat<T, R, C>& operator*=(Mat<T, R, C>& m, const T num)
{
	for (size_t r = 0; r < R; ++r)
	{
		for (size_t c = 0; c < C; ++c)
		{
			m[r][c] *= num;
		}
	}
	return m;
}

template <typename T, size_t R, size_t C>
Mat<T, R, C>& operator/=(Mat<T, R, C>& m, const T num)
{
	assert(num != 0);

	for (size_t r = 0; r < R; ++r)
	{
		for (size_t c = 0; c < C; ++c)
		{
			m[r][c] /= num;
		}
	}
	return m;
}

using Mat2 = Mat<float, 2, 2>;
using Mat3 = Mat<float, 3, 3>;
using Mat4 = Mat<float, 4, 4>;

template <size_t R, size_t C>
using Matf = Mat<float, R, C>;