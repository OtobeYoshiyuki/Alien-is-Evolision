// --------------------------------------------------------------------------------------
// File: Util.h
//
// �ėp�I�Ȋ֐�
//
// Date: 2020.9.15
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once
#include "pch.h"
#include <random>
#include <cassert>

// x�`y�̊Ԃ̐��������_���ŕԂ��֐�
inline int RandInt(int x, int y)
{
	assert(y >= x && "<RandInt>: x�̒l�̕����傫��");

	return rand() % (y - x + 1) + x;
}

// 0�`1�̊Ԃ̐��������_���ŕԂ��֐�
inline float RandFloat()
{
	return ((rand()) / (float)(RAND_MAX + 1));
}

// x�`y�̊Ԃ̐��������_���ŕԂ��֐�
inline float RandInRange(float x, float y)
{
	return x + RandFloat() * (y - x);
}

// -1.0f�`1.0f�̐��������_���ŕԂ��֐�
inline float RandomClamped()
{
	return RandFloat() - RandFloat();
}

// float�^�̒l���ꏏ���ǂ������肷��֐�
inline bool isEqual(float a, float b)
{
	if (fabsf(a - b) < FLT_MIN) return true;
	return false;
}

// Vector2��Vector3�ɕϊ�����֐�
inline DirectX::SimpleMath::Vector3 V2ToV3(const DirectX::SimpleMath::Vector2& v2)
{
	return DirectX::SimpleMath::Vector3(v2.x, 0.0f, v2.y);
}

// �Q�̈ʒu�̋��������߂�֐�
inline float Vec2Distance(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return (a - b).Length();
}

// �Q�̈ʒu�̋����̕��������߂�֐�
inline float Vec2DistanceSq(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return (a - b).LengthSquared();
}

// �x�N�g������]������֐�
inline void Vec2RotateAroundOrigin(DirectX::SimpleMath::Vector2* v, float ang)
{
	DirectX::SimpleMath::Matrix mat = DirectX::SimpleMath::Matrix::CreateRotationZ(ang);

	*v = DirectX::SimpleMath::Vector2::Transform(*v, mat);
}

// �Q�̒l�̊ԂɃN�����v����֐�
template <class T, class U, class V>
inline void Clamp(T* arg, const U& minVal, const V& maxVal)
{
	assert(((float)minVal < (float)maxVal) && "<Clamp>MaxVal < MinVal!");

	if ((*arg) < (T)minVal)
	{
		(*arg) = (T)minVal;
	}

	if ((*arg) >(T)maxVal)
	{
		(*arg) = (T)maxVal;
	}
}

// �����ȃx�N�g�������߂�֐�
inline DirectX::SimpleMath::Vector2 Perp(DirectX::SimpleMath::Vector2 v)
{
	return DirectX::SimpleMath::Vector2(-v.y, v.x);
}

// 2�̃x�N�g�����狗�����Z�o����
inline const DirectX::SimpleMath::Vector3 Length(const DirectX::SimpleMath::Vector3& v1, const DirectX::SimpleMath::Vector3& v2)
{
	//v1����v2�������āA�x�N�g�������߂�
	DirectX::SimpleMath::Vector3 vA = v1 - v2;

	//���߂��x�N�g�����Βl�ɒ���
	vA = DirectX::SimpleMath::Vector3(fabsf(vA.x), fabsf(vA.y), fabsf(vA.z));

	//�ϊ������x�N�g����Ԃ�
	return vA;
}

//Vector v1�� Vector v2�ȏ�̎�true��Ԃ�
// ����ȊO�̏ꍇ�́Afalse��Ԃ�
inline const bool RargeVector(const DirectX::SimpleMath::Vector3& v1, const DirectX::SimpleMath::Vector3& v2)
{
	//x,y,z�̔�r���s��
	//v1��v2�ȏ�̏ꍇ
	if (v1.x >= v2.x && v1.y >= v2.y && v1.z >= v2.z)return true;

	//v1��v2��菬�����ꍇ
	return false;
}

//Vector v1�� Vector v2�ȏ�̎�true��Ԃ�
// ����ȊO�̏ꍇ�́Afalse��Ԃ�
inline const bool SmallVector(const DirectX::SimpleMath::Vector3& v1, const DirectX::SimpleMath::Vector3& v2)
{
	//x,y,z�̔�r���s��
	//v1��v2�ȉ��̏ꍇ
	if (v1.x <= v2.x && v1.y <= v2.y && v1.z <= v2.z)return true;

	//v1��v2���傫���ꍇ
	return false;
}

