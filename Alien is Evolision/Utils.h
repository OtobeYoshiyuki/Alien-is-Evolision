// --------------------------------------------------------------------------------------
// File: Util.h
//
// 汎用的な関数
//
// Date: 2020.9.15
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once
#include "pch.h"
#include <random>
#include <cassert>

// x～yの間の数をランダムで返す関数
inline int RandInt(int x, int y)
{
	assert(y >= x && "<RandInt>: xの値の方が大きい");

	return rand() % (y - x + 1) + x;
}

// 0～1の間の数をランダムで返す関数
inline float RandFloat()
{
	return ((rand()) / (float)(RAND_MAX + 1));
}

// x～yの間の数をランダムで返す関数
inline float RandInRange(float x, float y)
{
	return x + RandFloat() * (y - x);
}

// -1.0f～1.0fの数をランダムで返す関数
inline float RandomClamped()
{
	return RandFloat() - RandFloat();
}

// float型の値が一緒かどうか判定する関数
inline bool isEqual(float a, float b)
{
	if (fabsf(a - b) < FLT_MIN) return true;
	return false;
}

// Vector2をVector3に変換する関数
inline DirectX::SimpleMath::Vector3 V2ToV3(const DirectX::SimpleMath::Vector2& v2)
{
	return DirectX::SimpleMath::Vector3(v2.x, 0.0f, v2.y);
}

// ２つの位置の距離を求める関数
inline float Vec2Distance(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return (a - b).Length();
}

// ２つの位置の距離の平方を求める関数
inline float Vec2DistanceSq(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return (a - b).LengthSquared();
}

// ベクトルを回転させる関数
inline void Vec2RotateAroundOrigin(DirectX::SimpleMath::Vector2* v, float ang)
{
	DirectX::SimpleMath::Matrix mat = DirectX::SimpleMath::Matrix::CreateRotationZ(ang);

	*v = DirectX::SimpleMath::Vector2::Transform(*v, mat);
}

// ２つの値の間にクランプする関数
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

// 垂直なベクトルを求める関数
inline DirectX::SimpleMath::Vector2 Perp(DirectX::SimpleMath::Vector2 v)
{
	return DirectX::SimpleMath::Vector2(-v.y, v.x);
}

// 2つのベクトルから距離を算出する
inline const DirectX::SimpleMath::Vector3 Length(const DirectX::SimpleMath::Vector3& v1, const DirectX::SimpleMath::Vector3& v2)
{
	//v1からv2を引いて、ベクトルを求める
	DirectX::SimpleMath::Vector3 vA = v1 - v2;

	//求めたベクトルを絶対値に直す
	vA = DirectX::SimpleMath::Vector3(fabsf(vA.x), fabsf(vA.y), fabsf(vA.z));

	//変換したベクトルを返す
	return vA;
}

//Vector v1が Vector v2以上の時trueを返す
// それ以外の場合は、falseを返す
inline const bool RargeVector(const DirectX::SimpleMath::Vector3& v1, const DirectX::SimpleMath::Vector3& v2)
{
	//x,y,zの比較を行う
	//v1がv2以上の場合
	if (v1.x >= v2.x && v1.y >= v2.y && v1.z >= v2.z)return true;

	//v1がv2より小さい場合
	return false;
}

//Vector v1が Vector v2以上の時trueを返す
// それ以外の場合は、falseを返す
inline const bool SmallVector(const DirectX::SimpleMath::Vector3& v1, const DirectX::SimpleMath::Vector3& v2)
{
	//x,y,zの比較を行う
	//v1がv2以下の場合
	if (v1.x <= v2.x && v1.y <= v2.y && v1.z <= v2.z)return true;

	//v1がv2より大きい場合
	return false;
}

