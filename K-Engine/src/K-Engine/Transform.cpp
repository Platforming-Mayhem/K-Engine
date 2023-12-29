#include "Transform.h"
namespace K 
{
	void MultiplyMatrixVector(K::Vector3& i, K::Vector3& o, K::Matrix4x4& m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}

	K::Matrix4x4* LookAt(K::Vector3 position, K::Vector3 target, K::Vector3 up)
	{
		K::Vector3 newForward = (target - position).normalise();

		K::Vector3 dot = newForward * K::Vector3::DotProduct(up, newForward);
		K::Vector3 newUp = (up - dot).normalise();

		K::Vector3 newRight = K::Vector3::CrossProduct(newUp, newForward).normalise();

		K::Matrix4x4 matrix;
		matrix.m[0][0] = newRight.x;
		matrix.m[0][1] = newRight.y;
		matrix.m[0][2] = newRight.z;
		matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = newUp.x;
		matrix.m[1][1] = newUp.y;
		matrix.m[1][2] = newUp.z;
		matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = newForward.x;
		matrix.m[2][1] = newForward.y;
		matrix.m[2][2] = newForward.z;
		matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = position.x;
		matrix.m[3][1] = position.y;
		matrix.m[3][2] = position.z;
		matrix.m[3][3] = 1.0f;
		return &matrix;
	}

	K::Matrix4x4 QuickInverse(K::Matrix4x4& m) // Only for Rotation/Translation Matrices
	{
		K::Matrix4x4 matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	K::Vector2::Vector2(float x, float y) 
	{
		this->x = x;
		this->y = y;
	}

	K::Vector3::Vector3() 
	{
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
	}

	K::Vector3::Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	K::Vector3 K::Vector3::normalise() 
	{
		this->x /= magnitude();
		this->y /= magnitude();
		this->z /= magnitude();
		return *this;
	}

	float K::Vector2::magnitude()
	{
		return sqrtf(powf(x, 2) + powf(y, 2));
	}

	float K::Vector3::magnitude()
	{
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	K::Transform::Transform(K::Vector3* newPos, K::Vector3* newRot, K::Vector3* newScale)
	{
		this->position = newPos;
		this->rotation = newRot;
		this->scale = newScale;
	}

	K::Transform::~Transform() 
	{
		delete this->position;
		delete this->rotation;
		delete this->scale;
	}

	void K::Transform::PassModelMatrix() 
	{
		this->modelMatrix = this->modelMatrix.IdentityMatrix();
		//Scaling Matrix
		this->modelMatrix.m[0][0] *= this->scale->x;
		this->modelMatrix.m[1][1] *= this->scale->y;
		this->modelMatrix.m[2][2] *= this->scale->z;
		//Rotation Matrix
		this->modelMatrix = K::Matrix4x4::Matrix_MultiplyMatrix(this->modelMatrix, *K::Quaternion::Euler(new K::Vector3(this->rotation->x, this->rotation->y, this->rotation->z))->QuaternionToMatrix());
		//Translation Matrix
		this->modelMatrix.m[3][0] = this->position->x;
		this->modelMatrix.m[3][1] = this->position->y;
		this->modelMatrix.m[3][2] = this->position->z;
	}

	K::Quaternion::~Quaternion() 
	{

	}

	K::Quaternion::Quaternion(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	K::Quaternion* K::Quaternion::Normalize() 
	{
		float n = sqrtf(powf(this->x, 2.0f) + powf(this->y, 2.0f) + powf(this->z, 2.0f) + powf(this->w, 2.0f));
		this->x /= n;
		this->y /= n;
		this->z /= n;
		this->w /= n;
		return this;
	}

	K::Matrix4x4* K::Quaternion::QuaternionToMatrix()
	{
		K::Matrix4x4 mat = K::Matrix4x4::IdentityMatrix();
		this->Normalize();
		float qx = this->x;
		float qy = this->y;
		float qz = this->z;
		float qw = this->w;

		//NOTE(JAWAD) Rotation Matrix
		mat.m[0][0] = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
		mat.m[1][0] = 2.0f * qx * qy - 2.0f * qz * qw;
		mat.m[2][0] = 2.0f * qx * qz + 2.0f * qy * qw;
		mat.m[0][1] = 2.0f * qx * qy + 2.0f * qz * qw;
		mat.m[1][1] = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
		mat.m[2][1] = 2.0f * qy * qz - 2.0f * qx * qw;
		mat.m[0][2] = 2.0f * qx * qz - 2.0f * qy * qw;
		mat.m[1][2] = 2.0f * qy * qz + 2.0f * qx * qw;
		mat.m[2][2] = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;
		return &mat;
	}

	K::Quaternion* K::Quaternion::Euler(K::Vector3* rotation)
	{
		float cy = cosf(rotation->z * 0.5 / 57.2958);
		float sy = sinf(rotation->z * 0.5 / 57.2958);
		float cp = cosf(rotation->y * 0.5 / 57.2958);
		float sp = sinf(rotation->y * 0.5 / 57.2958);
		float cr = cosf(rotation->x * 0.5 / 57.2958);
		float sr = sinf(rotation->x * 0.5 / 57.2958);

		K::Quaternion* q = new K::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		q->w = cr * cp * cy + sr * sp * sy;
		q->x = sr * cp * cy - cr * sp * sy;
		q->y = cr * sp * cy + sr * cp * sy;
		q->z = cr * cp * sy - sr * sp * cy;
		return q;
	}
}