#pragma once
#include "build.h"

namespace K 
{
	struct K_API Matrix4x4
	{
		float m[4][4] = { 0 };

		~Matrix4x4() 
		{
			//std::cout << "Destroy Matrix4x4" << std::endl;
		}

		static Matrix4x4 IdentityMatrix()
		{
			Matrix4x4 identity;
			identity.m[0][0] = 1.0f;
			identity.m[1][1] = 1.0f;
			identity.m[2][2] = 1.0f;
			identity.m[3][3] = 1.0f;
			return identity;
		}

		static Matrix4x4 Matrix_MultiplyMatrix(Matrix4x4& lhs, Matrix4x4& rhs)
		{
			Matrix4x4 matrix;
			for (int c = 0; c < 4; c++)
				for (int r = 0; r < 4; r++)
					matrix.m[r][c] = lhs.m[r][0] * rhs.m[0][c] + lhs.m[r][1] * rhs.m[1][c] + lhs.m[r][2] * rhs.m[2][c] + lhs.m[r][3] * rhs.m[3][c];
			return matrix;
		}

		bool operator == (const K::Matrix4x4 other)
		{
			for (int i = 0; i < 4; i++) 
			{
				for (int j = 0; j < 4; j++) 
				{
					if (other.m[i][j] != this->m[i][j]) 
					{
						return false;
					}
				}
			}
			return true;
		}
	};

	struct K_API Vector2 
	{
		Vector2(float x, float y);

		float x = 0.0f, y = 0.0f;

		float magnitude();
	};

	struct K_API Vector3
	{
	public:
		float x = 0.0f, y = 0.0f, z = 0.0f;

		float magnitude();

		Vector3 normalise();

		Vector3();

		Vector3(float x, float y, float z);

		void Reset() 
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}

		void ResetScale() 
		{
			this->x = 1.0f;
			this->y = 1.0f;
			this->z = 1.0f;
		}

		~Vector3();

		bool operator == (const K::Vector3 other) 
		{
			if (this->x == other.x && this->y == other.y && this->z == other.z)
				return true;
			else
				return false;
		}

		bool operator != (const K::Vector3 other)
		{
			if (this->x == other.x && this->y == other.y && this->z == other.z)
				return false;
			else
				return true;
		}

		K::Vector3 operator - (const K::Vector3 other)
		{
			K::Vector3 temp = *this;
			temp.x -= other.x;
			temp.y -= other.y;
			temp.z -= other.z;
			return temp;
		}

		K::Vector3 operator + (const K::Vector3 other)
		{
			K::Vector3 temp = *this;
			temp.x += other.x;
			temp.y += other.y;
			temp.z += other.z;
			return temp;
		}

		K::Vector3 operator * (const K::Vector3 other)
		{
			K::Vector3 temp = *this;
			temp.x *= other.x;
			temp.y *= other.y;
			temp.z *= other.z;
			return temp;
		}

		K::Vector3 operator / (const K::Vector3 other)
		{
			K::Vector3 temp = *this;
			temp.x /= other.x;
			temp.y /= other.y;
			temp.z /= other.z;
			return temp;
		}

		K::Vector3& operator * (const float& other)
		{
			K::Vector3 temp = *this;
			temp.x *= other;
			temp.y *= other;
			temp.z *= other;
			return temp;
		}

		// Example: this->vertices[i].position += *other;

		K::Vector3 &operator += (const K::Vector3 &other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}

		// Example: *(Player::parent->GetTransform()->position) += new K::Vector3(0.0f, 1.0f, 0.0f);

		K::Vector3* operator += (const K::Vector3* other)
		{
			this->x += other->x;
			this->y += other->y;
			this->z += other->z;
			return this;
		}

		K::Vector3& operator -= (const K::Vector3& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;
		}

		K::Vector3* operator -= (const K::Vector3* other)
		{
			this->x -= other->x;
			this->y -= other->y;
			this->z -= other->z;
			return this;
		}

		K::Vector3& operator *= (const K::Vector3& other)
		{
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			return *this;
		}

		K::Vector3& operator /= (const K::Vector3& other)
		{
			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			return *this;
		}

		static K::Vector3 Lerp(K::Vector3& a, K::Vector3& b, float t) 
		{
			t = std::clamp(t, 0.0f, 1.0f);
			return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
		}

		static K::Vector3 CrossProduct(K::Vector3& a, K::Vector3& b) 
		{
			K::Vector3 o = K::Vector3(0.0f, 0.0f, 0.0f);
			o.x = (a.y * b.z) - (a.z * b.y);
			o.y = (a.z * b.x) - (a.x * b.z);
			o.z = (a.x * b.y) - (a.y * b.x);
			return o;
		}

		static float DotProduct(K::Vector3& a, K::Vector3& b)
		{
			float dot = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
			return dot;
		}

		static float AngleBetweenVectors(K::Vector3& a, K::Vector3& b) 
		{
			float theta = acosf(DotProduct(a, b) / (a.magnitude() * b.magnitude()));
			if (isnan(theta))
			{
				return 0.0f;
			}
			return theta * 57.2958f;
		}
	};

	struct K_API Quaternion
	{
		float x, y, z, w;

		Quaternion(float x, float y, float z, float w);

		~Quaternion();

		K::Quaternion* Normalize();

		K::Matrix4x4 QuaternionToMatrix();

		static K::Quaternion* Euler(Vector3* rotation);

		K::Vector3* ToEuler();
	};

	class K_API Transform 
	{
	private:
		Vector3 previousPosition;
		Vector3 previousRotation;
		Vector3 previousScale;

	public:
		Vector3* position;
		Vector3* rotation;
		Vector3* scale;

		Vector3* localPosition;
		Vector3* localRotation;
		Vector3* localScale;

		K::Matrix4x4 modelMatrix;

		K::Matrix4x4 PositionMatrix();

		K::Matrix4x4 ScaleMatrix();

		K::Matrix4x4 LocalScaleMatrix(K::Transform* parent);

		Transform();

		Transform(Vector3* newPos, Vector3* newRot, Vector3* newScale);

		~Transform();

		void PassModelMatrix(K::Transform* parent = nullptr);
	};

	K_API void MultiplyMatrixVector(Vector3& i, Vector3& o, Matrix4x4& m);

	K_API K::Matrix4x4 LookAt(K::Vector3 position, K::Vector3 target, K::Vector3 up);

	K_API K::Matrix4x4 QuickInverse(K::Matrix4x4& m);
}