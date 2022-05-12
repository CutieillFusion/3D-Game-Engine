#pragma once

class Vector2D
{
public:
	Vector2D() : x(0), y(0)
	{

	} 

	Vector2D(float x, float y) :x(x), y(y)
	{
	}

	Vector2D(const Vector2D& vector) :x(vector.x), y(vector.y)
	{

	}

	static float Distance(const Vector2D& v1, const Vector2D& v2) 
	{
		return std::sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
	}

	static float Dot(const Vector2D& v1, const Vector2D& v2) 
	{
		return (v1.x * v2.x) + (v1.y * v2.y);
	}

	static Vector2D Lerp(const Vector2D& start, const Vector2D& end, float delta) 
	{
		Vector2D v;

		v.x = start.x * (1.0f - delta) + end.x * (delta);
		v.y = start.y * (1.0f - delta) + end.y * (delta);

		return v;
	}

	Vector2D operator *(float num)
	{
		return Vector2D(x * num, y * num);
	}

	Vector2D operator /(float num)
	{
		return Vector2D(num / x, num / y);
	}

	Vector2D operator +(Vector2D vec)
	{
		return Vector2D(x + vec.x, y + vec.y);
	}

	Vector2D operator -(Vector2D vec)
	{
		return Vector2D(x - vec.x, y - vec.y);
	}

	~Vector2D()
	{

	}
public:
	float x, y;
};


