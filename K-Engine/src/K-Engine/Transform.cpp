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

	//This Is Just Brute Force Inverse
	K::Matrix4x4 QuickInverse(K::Matrix4x4& m)
	{
		Matrix4x4 mat;

		float A2323 = m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2];
		float A1323 = m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1];
		float A1223 = m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1];
		float A0323 = m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0];
		float A0223 = m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0];
		float A0123 = m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0];
		float A2313 = m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2];
		float A1313 = m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1];
		float A1213 = m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1];
		float A2312 = m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2];
		float A1312 = m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1];
		float A1212 = m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1];
		float A0313 = m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0];
		float A0213 = m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0];
		float A0312 = m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0];
		float A0212 = m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0];
		float A0113 = m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0];
		float A0112 = m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0];

		float det = m.m[0][0] * (m.m[1][1] * A2323 - m.m[1][2] * A1323 + m.m[1][3] * A1223) - m.m[0][1] * (m.m[1][0] * A2323 - m.m[1][2] * A0323 + m.m[1][3] * A0223) + m.m[0][2] * (m.m[1][0] * A1323 - m.m[1][1] * A0323 + m.m[1][3] * A0123) - m.m[0][3] * (m.m[1][0] * A1223 - m.m[1][1] * A0223 + m.m[1][2] * A0123);
		det = 1 / det;

		mat.m[0][0] = det * (m.m[1][1] * A2323 - m.m[1][2] * A1323 + m.m[1][3] * A1223);
		mat.m[0][1] = det * -(m.m[0][1] * A2323 - m.m[0][2] * A1323 + m.m[0][3] * A1223);
		mat.m[0][2] = det * (m.m[0][1] * A2313 - m.m[0][2] * A1313 + m.m[0][3] * A1213);
		mat.m[0][3] = det * -(m.m[0][1] * A2312 - m.m[0][2] * A1312 + m.m[0][3] * A1212);
		mat.m[1][0] = det * -(m.m[1][0] * A2323 - m.m[1][2] * A0323 + m.m[1][3] * A0223);
		mat.m[1][1] = det * (m.m[0][0] * A2323 - m.m[0][2] * A0323 + m.m[0][3] * A0223);
		mat.m[1][2] = det * -(m.m[0][0] * A2313 - m.m[0][2] * A0313 + m.m[0][3] * A0213);
		mat.m[1][3] = det * (m.m[0][0] * A2312 - m.m[0][2] * A0312 + m.m[0][3] * A0212);
		mat.m[2][0] = det * (m.m[1][0] * A1323 - m.m[1][1] * A0323 + m.m[1][3] * A0123);
		mat.m[2][1] = det * -(m.m[0][0] * A1323 - m.m[0][1] * A0323 + m.m[0][3] * A0123);
		mat.m[2][2] = det * (m.m[0][0] * A1313 - m.m[0][1] * A0313 + m.m[0][3] * A0113);
		mat.m[2][3] = det * -(m.m[0][0] * A1312 - m.m[0][1] * A0312 + m.m[0][3] * A0112);
		mat.m[3][0] = det * -(m.m[1][0] * A1223 - m.m[1][1] * A0223 + m.m[1][2] * A0123);
		mat.m[3][1] = det * (m.m[0][0] * A1223 - m.m[0][1] * A0223 + m.m[0][2] * A0123);
		mat.m[3][2] = det * -(m.m[0][0] * A1213 - m.m[0][1] * A0213 + m.m[0][2] * A0113);
		mat.m[3][3] = det * (m.m[0][0] * A1212 - m.m[0][1] * A0212 + m.m[0][2] * A0112);

		return mat;
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

	K::Vector3::~Vector3()
	{

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

	K::Transform::Transform() 
	{
		this->position = new K::Vector3();
		this->rotation = new K::Vector3();
		this->scale = new K::Vector3(1.0f, 1.0f, 1.0f);
		this->localPosition = new K::Vector3();
		this->localRotation = new K::Vector3();
		this->localScale = new K::Vector3(1.0f, 1.0f, 1.0f);
	}

	K::Transform::Transform(K::Vector3* newPos, K::Vector3* newRot, K::Vector3* newScale)
	{
		this->position = newPos;
		this->rotation = newRot;
		this->scale = newScale;
		this->localPosition = new K::Vector3();
		this->localRotation = new K::Vector3();
		this->localScale = new K::Vector3(1.0f, 1.0f, 1.0f);
	}

	K::Transform::~Transform() 
	{
		std::cout << "Started Transform Destructor..." << std::endl;
		delete this->position;
		delete this->rotation;
		delete this->scale;
		delete this->localPosition;
		delete this->localRotation;
		delete this->localScale;
		std::cout << "Transform Destroyed" << std::endl;
	}

	K::Matrix4x4 K::Transform::PositionMatrix() 
	{
		K::Matrix4x4 temp = K::Matrix4x4::IdentityMatrix();
		//Translation Matrix
		temp.m[3][0] += this->position->x;
		temp.m[3][1] += this->position->y;
		temp.m[3][2] += this->position->z;

		return temp;
	}

	K::Matrix4x4 K::Transform::RotationMatrix()
	{
		K::Matrix4x4 temp = K::Matrix4x4::IdentityMatrix();

		//Rotation Matrix
		temp = K::Matrix4x4::Matrix_MultiplyMatrix(temp, *K::Quaternion::Euler(this->rotation)->QuaternionToMatrix());
		
		return temp;
	}

	K::Matrix4x4 K::Transform::ScaleMatrix()
	{
		K::Matrix4x4 temp = K::Matrix4x4::IdentityMatrix();

		//Scaling Matrix
		temp.m[0][0] *= this->scale->x;
		temp.m[1][1] *= this->scale->y;
		temp.m[2][2] *= this->scale->z;

		return temp;
	}

	void K::Transform::PassModelMatrix(K::Transform* parent) 
	{

		K::Matrix4x4 globalModelMatrix = K::Matrix4x4::IdentityMatrix();

		if (parent == nullptr) 
		{
			//World space transforms are in control
			*this->localScale = *this->scale;
			*this->localRotation = *this->rotation;
			*this->localPosition = *this->position;
			//Scaling Matrix
			globalModelMatrix.m[0][0] *= this->scale->x;
			globalModelMatrix.m[1][1] *= this->scale->y;
			globalModelMatrix.m[2][2] *= this->scale->z;
			//Rotation Matrix
			globalModelMatrix = K::Matrix4x4::Matrix_MultiplyMatrix(globalModelMatrix, *K::Quaternion::Euler(this->rotation)->QuaternionToMatrix());
			//Translation Matrix
			globalModelMatrix.m[3][0] += this->position->x;
			globalModelMatrix.m[3][1] += this->position->y;
			globalModelMatrix.m[3][2] += this->position->z;
		}
		else 
		{
			if (*this->position != this->previousPosition)
			{
				*this->localPosition = *this->position;
				K::Matrix4x4 invert = K::QuickInverse(parent->modelMatrix);
				K::MultiplyMatrixVector(*this->position, *this->localPosition, invert);
			}
			if (*this->rotation != this->previousRotation)
			{
				*this->localRotation = *this->rotation - *parent->rotation;
			}
			if (*this->scale != this->previousScale)
			{
				*this->localScale = *this->scale / *parent->scale;
			}

			//Local space transforms are in control
			K::Vector3 localScaleInWorldSpace = *this->localScale * *parent->scale;
			K::Vector3 localRotationInWorldSpace = *this->localRotation + *parent->rotation;
			K::Vector3 localPositionInWorldSpace;
			K::MultiplyMatrixVector(*this->localPosition, localPositionInWorldSpace, parent->modelMatrix);
			//Scaling Matrix
			globalModelMatrix.m[0][0] *= localScaleInWorldSpace.x;
			globalModelMatrix.m[1][1] *= localScaleInWorldSpace.y;
			globalModelMatrix.m[2][2] *= localScaleInWorldSpace.z;
			//Rotation Matrix
			globalModelMatrix = K::Matrix4x4::Matrix_MultiplyMatrix(globalModelMatrix, *K::Quaternion::Euler(&localRotationInWorldSpace)->QuaternionToMatrix());
			//Translation Matrix
			globalModelMatrix.m[3][0] += localPositionInWorldSpace.x;
			globalModelMatrix.m[3][1] += localPositionInWorldSpace.y;
			globalModelMatrix.m[3][2] += localPositionInWorldSpace.z;
		}

		this->modelMatrix = globalModelMatrix;

		this->previousPosition = *this->position;
		this->previousRotation = *this->rotation;
		this->previousScale = *this->scale;
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