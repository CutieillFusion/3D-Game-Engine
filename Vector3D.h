#pragma once

class Vector3D
{
public:
	Vector3D() : x(0), y(0), z(0)
	{

	} 

	Vector3D(float x, float y, float z) : x(x), y(y), z(z)
	{
	}

	Vector3D(const Vector3D& vector) :x(vector.x), y(vector.y), z(vector.z)
	{

	}

	static float Distance(const Vector3D& v1, const Vector3D& v2) 
	{
		return std::sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
	}

	static float Dot(const Vector3D& v1, const Vector3D& v2) 
	{
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	}
	
	static Vector3D Cross(const Vector3D& v1, const Vector3D& v2) 
	{
		Vector3D v;

		v.x = (v1.y * v2.z) - (v1.z * v2.y);
		v.y = (v1.z * v2.x) - (v1.x * v2.z);
		v.z = (v1.x * v2.y) - (v1.y * v2.x);

		return v;
	}

	static Vector3D Lerp(const Vector3D& start, const Vector3D& end, float delta) 
	{
		Vector3D v;

		v.x = start.x * (1.0f - delta) + end.x * (delta);
		v.y = start.y * (1.0f - delta) + end.y * (delta);
		v.z = start.z * (1.0f - delta) + end.z * (delta);

		return v;
	}

	Vector3D operator *(float num)
	{
		return Vector3D(x * num, y * num, z * num);
	}

	Vector3D operator /(float num)
	{
		return Vector3D(num / x, num / y, num / z);
	}

	Vector3D operator +(Vector3D vec)
	{
		return Vector3D(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3D operator -(Vector3D vec)
	{
		return Vector3D(x - vec.x, y - vec.y, z - vec.z);
	}

	~Vector3D()
	{

	}
public:
	float x, y, z;
};


