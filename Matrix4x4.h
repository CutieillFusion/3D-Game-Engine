#pragma once
#include <memory>

//This doesn't require any other classes but inturn is harder to read

//To modify to add vector classes just swap out x, y, z, and w with correct vector classes

class Matrix4x4
{
public:
	//Constructor
	Matrix4x4()
	{

	}

	//Sets Identity
	void SetIdentity()
	{
		::memset(mat, 0, sizeof(float) * 16);
		mat[0][0] = 1.0f;
		mat[1][1] = 1.0f;
		mat[2][2] = 1.0f;
		mat[3][3] = 1.0f;
	}

	//Sets a Translation
	void SetTranslation(float x, float y, float z)
	{
		mat[3][0] = x;
		mat[3][1] = y;
		mat[3][2] = z;
	}

	//Sets a Scale
	void SetScale(float x, float y, float z)
	{
		mat[0][0] = x;
		mat[1][1] = y;
		mat[2][2] = z;
	}

	//Sets a X Rotation
	void SetRotationX(float x)
	{
		mat[1][1] = (float)cos(x);
		mat[1][2] = (float)sin(x);
		mat[2][1] = (float)-sin(x);
		mat[2][2] = (float)cos(x);
	}

	//Sets a Y Rotation
	void SetRotationY(float y)
	{
		mat[0][0] = (float)cos(y);
		mat[0][2] = (float)-sin(y);
		mat[2][0] = (float)sin(y);
		mat[2][2] = (float)cos(y);
	}

	//Sets a Z Rotation
	void SetRotationZ(float z)
	{
		mat[0][0] = (float)cos(z);
		mat[0][1] = (float)sin(z);
		mat[1][0] = (float)-sin(z);
		mat[1][1] = (float)cos(z);
	}

	//Gets the Determinant
	float GetDeterminant()
	{
		return -(
			mat[0][3] * (mat[1][0] * (mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1]) - mat[2][0] * (mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1]) + mat[3][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]))
			+
			mat[1][3] * -(mat[0][0] * (mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1]) - mat[2][0] * (mat[0][1] * mat[3][2] - mat[0][2] * mat[3][1]) + mat[3][0] * (mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1]))
			+
			mat[2][3] * (mat[0][0] * (mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1]) - mat[1][0] * (mat[0][1] * mat[3][2] - mat[0][2] * mat[3][1]) + mat[3][0] * (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]))
			+
			mat[3][3] * -(mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) - mat[1][0] * (mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1]) + mat[2][0] * (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]))
			);
	}

	//Gets the Inverse
	void Inverse()
	{
		int a, i, j;
		Matrix4x4 out;
		float x1, y1, z1, w1;
		float x[3], y[3], z[3], w[3];
		float det = 0.0f;

		det = this->GetDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i)
					{
						--a;
					}
					x[a] = mat[j][0];
					y[a] = mat[j][1];
					z[a] = mat[j][2];
					w[a] = mat[j][3];
				}
			}
			out.mat[0][i] = (float)pow(-1.0f, i) * (y[0] * (z[1] * w[2] - z[2] * w[1]) - z[0] * (y[1] * w[2] - y[2] * w[1]) + w[0] * (y[1] * z[2] - z[1] * y[2])) / det;
			out.mat[1][i] = (float)pow(-1.0f, i) * -(x[0] * (z[1] * w[2] - z[2] * w[1]) - z[0] * (x[1] * w[2] - x[2] * w[1]) + w[0] * (x[1] * z[2] - x[2] * z[1])) / det;
			out.mat[2][i] = (float)pow(-1.0f, i) * (x[0] * (y[1] * w[2] - y[2] * w[1]) - y[0] * (x[1] * w[2] - x[2] * w[1]) + w[0] * (x[1] * y[2] - x[2] * y[1])) / det;
			out.mat[3][i] = (float)pow(-1.0f, i) * -(x[0] * (y[1] * z[2] - y[2] * z[1]) - y[0] * (x[1] * z[2] - x[2] * z[1]) + z[0] * (x[1] * y[2] - x[2] * y[1])) / det;
		}

		this->SetMatrix(out);
	}

	//Multiplies Matrices
	void operator *=(const Matrix4x4 matrix)
	{
		Matrix4x4 out;

		for (unsigned i = 0; i < 4; i++)
		{
			for (unsigned j = 0; j < 4; j++)
			{
				out.mat[i][j] =
					mat[i][0] * matrix.mat[0][j] + mat[i][1] * matrix.mat[1][j] +
					mat[i][2] * matrix.mat[2][j] + mat[i][3] * matrix.mat[3][j];
			}
		}

		::memcpy(mat, out.mat, sizeof(float) * 16);
	}

	//Copies a Matrix
	void SetMatrix(const Matrix4x4& matrix)
	{
		::memcpy(mat, matrix.mat, sizeof(float) * 16);
	}

	//Creates a Perspective Projection Matrix
	void SetPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
	{
		SetIdentity();
		float yscale = 1.0f / tan(fov / 2.0f);
		float xscale = yscale / aspect;
		mat[0][0] = xscale;
		mat[1][1] = yscale;
		mat[2][2] = zfar / (zfar - znear);
		mat[2][3] = 1.0f;
		mat[3][2] = (-znear * zfar) / (zfar - znear);
	}

	//Creates a Orthographic Projection Matrix
	void SetOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		SetIdentity();
		mat[0][0] = 2.0f / width;
		mat[1][1] = 2.0f / height;
		mat[2][2] = 1.0f / (far_plane - near_plane);
		mat[3][2] = -(near_plane / (far_plane - near_plane));
	}

	//Prints Matrix
	void PrintMatrix() 
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				std::cout << mat[i][j] << " ";
			}
			std::cout << "\n";
		}
	}

	//Grabs the Translation
	void GetTranslation(float& x, float& y, float& z) 
	{
		x = mat[3][0];
		y = mat[3][1];
		z = mat[3][2];
	}

	//Grabs the X Direction
	void GetXDirection(float& x, float& y, float& z)
	{
		x = mat[0][0];
		y = mat[0][1];
		z = mat[0][2];
	}

	//Grabs the Y Direction
	void GetYDirection(float& x, float& y, float& z)
	{
		x = mat[1][0];
		y = mat[1][1];
		z = mat[1][2];
	}

	//Grabs the Z Direction
	void GetZDirection(float& x, float& y, float& z)
	{
		x = mat[2][0];
		y = mat[2][1];
		z = mat[2][2];
	}

	//Grabs the Scale
	void GetScale(float& x, float& y, float& z) 
	{
		x = mat[0][0];
		y = mat[1][1];
		z = mat[2][2];
	}

	//Deconstructor
	~Matrix4x4()
	{

	}

public:
	//Matrix 
	float mat[4][4];
};


