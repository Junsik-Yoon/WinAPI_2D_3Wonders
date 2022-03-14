#pragma once

struct iPoint
{
	int x;
	int y;
	iPoint(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

struct Vec2
{
	float x;
	float y;

	float Length()
	{
		return (float)sqrt((double)x * x + (double)y * y);
	}
	Vec2& Normalize()
	{
		float fLen = Length();
		assert(fLen != 0.f);
		x /= fLen;
		y /= fLen;
		return *this;
	}


	Vec2()
	{
		x = 0;
		y = 0;
	}

	Vec2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vec2(const POINT& _pt)
		: x((float)_pt.x)
		, y((float)_pt.y)
	{}


	Vec2& operator=(const Vec2& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	Vec2 operator+(const Vec2& other)
	{
		return Vec2(x + other.x, y + other.y);
	}

	Vec2 operator-(const Vec2& other)
	{
		return Vec2(x - other.x, y - other.y);
	}

	Vec2& operator+= (const Vec2& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	template <typename T>
	Vec2 operator*(T num)
	{
		return Vec2(x * num, y * num);
	}

	template <typename T>
	Vec2 operator/(T num)
	{
		assert(0 != num);

		return Vec2(x / num, y / num);
	}

};

