#pragma once
#include <memory>
class Matrix4x4
{
public:
	Matrix4x4()
	{

	}

	void SetIdentity()
	{
		::memset(mat, 0, sizeof(float) * 16);
		mat[0][0] = 1;
		mat[1][1] = 1;
		mat[2][2] = 1;
		mat[3][3] = 1;
	}

	void SetTranslation(float x, float y, float z)
	{
		mat[3][0] = x;
		mat[3][1] = y;
		mat[3][2] = z;
	}

	void SetScale(float x, float y, float z)
	{
		mat[0][0] = x;
		mat[1][1] = y;
		mat[2][2] = z;
	}

	void SetRotationX(float x)
	{
		mat[1][1] = (float)cos(x);
		mat[1][2] = (float)sin(x);
		mat[2][1] = (float)-sin(x);
		mat[2][2] = (float)cos(x);
	}


	void SetRotationY(float y)
	{
		mat[0][0] = (float)cos(y);
		mat[0][2] = (float)sin(y);
		mat[2][0] = (float)-sin(y);
		mat[2][2] = (float)cos(y);
	}

	void SetRotationZ(float z)
	{
		mat[0][0] = (float)cos(z);
		mat[0][1] = (float)sin(z);
		mat[1][0] = (float)-sin(z);
		mat[1][1] = (float)cos(z);
	}

	float GetDeterminant() 
	{
		float det = -(
			mat[0][3] *  (mat[1][0] * (mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1]) - mat[2][0] * (mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1]) + mat[3][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][1]))
			+ 
			mat[1][3] * -(mat[0][0] * (mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1]) - mat[2][0] * (mat[0][1] * mat[3][2] - mat[0][2] * mat[3][1]) + mat[3][0] * (mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1]))
			+ 
			mat[2][3] *  (mat[0][0] * (mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1]) - mat[1][0] * (mat[0][1] * mat[3][2] - mat[0][2] * mat[3][1]) + mat[3][0] * (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]))
			+ 
			mat[3][3] * -(mat[0][0] * (mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1]) - mat[1][0] * (mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1]) + mat[2][0] * (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]))
			);

		return det;
	}

	void Inverse() 
	{
		//int a, i, j;
		//Matrix4x4 out;
		//Vector4D v, vec[3];
		//float det = 0.0f;

		//det = this->getDeterminant();
		//if (!det) return;
		//for (i = 0; i < 4; i++)
		//{
		//	for (j = 0; j < 4; j++)
		//	{
		//		if (j != i)
		//		{
		//			a = j;
		//			if (j > i) a = a - 1;
		//			vec[a].m_x = (this->m_mat[j][0]);
		//			vec[a].m_y = (this->m_mat[j][1]);
		//			vec[a].m_z = (this->m_mat[j][2]);
		//			vec[a].m_w = (this->m_mat[j][3]);
		//		}
		//	}
		//	v.cross(vec[0], vec[1], vec[2]);

		//	out.m_mat[0][i] = (float)pow(-1.0f, i) * v.m_x / det;
		//	out.m_mat[1][i] = (float)pow(-1.0f, i) * v.m_y / det;
		//	out.m_mat[2][i] = (float)pow(-1.0f, i) * v.m_z / det;
		//	out.m_mat[3][i] = (float)pow(-1.0f, i) * v.m_w / det;
		//}

		//this->setMatrix(out);
	}

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

		::memcpy(mat, matrix.mat, sizeof(float) * 16);
	}

	void SetMatrix(const Matrix4x4& matrix) 
	{
		::memcpy(mat, matrix.mat, sizeof(float) * 16);
	}

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

	void SetOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		SetIdentity();
		mat[0][0] = 2.0f / width;
		mat[1][1] = 2.0f / height;
		mat[2][2] = 1.0f / (far_plane - near_plane);
		mat[3][2] = -(near_plane / (far_plane - near_plane));
	}

	~Matrix4x4() 
	{

	}
public:
	float mat[4][4];

};