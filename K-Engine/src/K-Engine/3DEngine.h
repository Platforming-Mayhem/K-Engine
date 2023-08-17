#pragma once
#include "build.h"
int screenWidth = 1920;
int screenHeight = 1080;

struct Vector3
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
	float magnitude() {
		return sqrt((x * x) + (y * y));
	}
};

struct Triangle
{
	Vector3 p[3];
	Vector3 n;
};

struct Mesh
{
	std::vector<Triangle> tris;

	std::vector<Vector3> normals;

	bool LoadObjectFromFile(std::string File)
	{
		std::ifstream newMesh(File);
		if (!newMesh.is_open())
			return false;
		std::vector<Vector3> verts;
		while (!newMesh.eof())
		{
			char currentLine[128];

			newMesh.getline(currentLine, 128);

			std::strstream s;
			s << currentLine;

			char junkCharacter;

			if (currentLine[0] == 'v' && currentLine[1] != 'n')
			{
				Vector3 v;
				s >> junkCharacter >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
			else if (currentLine[0] == 'f')
			{
				int i[3];
				s >> junkCharacter >> i[0] >> i[1] >> i[2];
				tris.push_back({ verts[i[0] - 1], verts[i[1] - 1], verts[i[2] - 1] });
			}
		}
		return true;
	}
};

struct Matrix4x4
{
	float m[4][4] = { 0 };
};

Matrix4x4 XRotationMatrix(float rotationX) 
{
	Matrix4x4 matX;
	matX.m[0][0] = 1.0f;
	matX.m[1][1] = cosf(rotationX / 57.2958f);
	matX.m[2][1] = -sinf(rotationX / 57.2958f);
	matX.m[1][2] = sinf(rotationX / 57.2958f);
	matX.m[2][2] = cosf(rotationX / 57.2958f);
	return matX;
}

Matrix4x4 YRotationMatrix(float rotationY)
{
	Matrix4x4 matY;
	matY.m[0][0] = cosf(rotationY / 57.2958f);
	matY.m[0][2] = -sinf(rotationY / 57.2958f);
	matY.m[2][0] = sinf(rotationY / 57.2958f);
	matY.m[2][2] = cosf(rotationY / 57.2958f);
	matY.m[1][1] = 1.0f;
	return matY;
}

Matrix4x4 ZRotationMatrix(float rotationZ)
{
	Matrix4x4 matZ;
	matZ.m[0][0] = cosf(rotationZ / 57.2958f);
	matZ.m[0][1] = sinf(rotationZ / 57.2958f);
	matZ.m[1][0] = -sinf(rotationZ / 57.2958f);
	matZ.m[2][2] = 1.0f;
	matZ.m[1][1] = cosf(rotationZ / 57.2958f);
	return matZ;
}

Matrix4x4 IdentityMatrix() 
{
	Matrix4x4 identity;
	identity.m[0][0] = 1.0f;
	identity.m[1][1] = 1.0f;
	identity.m[2][2] = 1.0f;
	return identity;
}

Matrix4x4 MultiplyMatrices(Matrix4x4 &a, Matrix4x4 &b)
{
	Matrix4x4 c;
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++)
		{
			c.m[i][j] = a.m[j][0] * b.m[0][i] + a.m[j][1] * b.m[1][i] + a.m[j][2] * b.m[2][i] + a.m[j][3] * b.m[3][i];
		}
	}
	return c;
}

Matrix4x4 TranslationMatrix(float x, float y, float z)
{
	Matrix4x4 a;
	a.m[0][0] = 1.0f;
	a.m[1][1] = 1.0f;
	a.m[2][2] = 1.0f;
	a.m[3][3] = 1.0f;
	a.m[3][0] = x;
	a.m[3][1] = y;
	a.m[3][2] = z;
	return a;
}

Vector3 AddVectors(Vector3 &a, Vector3 &b)
{
	Vector3 c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}

Vector3 SubtractVectors(Vector3 &a, Vector3 &b)
{
	Vector3 c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return c;
}

Vector3 CrossProductVector(Vector3 &a, Vector3 &b)
{
	Vector3 o;
	o.x = (a.y * b.z) - (a.z * b.y);
	o.y = (a.z * b.x) - (a.x * b.z);
	o.z = (a.x * b.y) - (a.y * b.x);
	return o;
}

Vector3 DivideVector(Vector3 &a, float value)
{
	return {a.x / value, a.y / value, a.z / value};
}

Vector3 MultiplyVector(Vector3 &a, float value)
{
	return { a.x * value, a.y * value, a.z * value };
}

float DotProduct(Vector3 &a, Vector3 &b)
{
	float dot = a.x * b.x + a.y * b.y + a.z * b.z;
	return dot;
}

float LengthVector(Vector3 &a)
{
	float length = sqrtf(a.x * a.x + a.y * a.y);
	return length;
}

Vector3 NormaliseVector(Vector3 &a) {
	float length = LengthVector(a);
	Vector3 o;
	o.x = a.x / length;
	o.y = a.y / length;
	//o.z = a.z / length;
	return o;
}

std::vector<Vector3> CalculateMeshNormals(Mesh mesh)
{
	std::vector<Vector3> normals;
	for (auto tri : mesh.tris)
	{
		Vector3 normal, a, b;

		a = SubtractVectors(tri.p[1], tri.p[0]);

		b = SubtractVectors(tri.p[2], tri.p[0]);

		normal = CrossProductVector(a, b);

		normal = NormaliseVector(normal);

		normals.push_back(normal);
	}
	return normals;
}

//Projection Matrix

Matrix4x4 ProjectionMatrix(int ScreenHeight, int ScreenWidth) {
	Matrix4x4 proj;
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFOV = 135.0f;
	float fAspectRatio = (float)ScreenWidth / (float)ScreenHeight;
	float fFOVRad = 1.0f / tanf(fFOV * 0.5f / 180.0f * 3.14159f);
	proj.m[0][0] = fAspectRatio * fFOVRad;
	proj.m[1][1] = fFOVRad;
	proj.m[2][2] = fFar / (fFar - fNear);
	proj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	proj.m[2][3] = 1.0f;
	proj.m[3][3] = 0.0f;
	return proj;
}

void MultiplyMatrixVector(Vector3 &i, Vector3 &o, Matrix4x4 &m)
{
	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	o.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
}