#include "pch.h"
#include "AccelState.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

AccelState::AccelState():mAccel(Vector3::Zero),mAccelCount(NULL)
{
}

const Vector3 AccelState::Update()
{
	//ベクトルの初期化
	Vector3 vel = Vector3::Zero;

	//実行関数がfalseの時は、何もしない
	if (!this->GetExecute())return vel;

	//方向を決める
	float dir = 0.0f;

	//ベクトルの向き
	if (mAccel.x > NULL)dir = 1.0f;

	if (mAccel.x < NULL)dir = -1.0f;

	//計算回数がMAX_ACCELを上回ったとき、これ以上加速しない
	if (mAccelCount > MAX_ACCEL)
	{
		return mAccel;
	}

	//加速度の計算
	mAccel += Vector3(dir, 0.0f, 0.0f);

	vel = mAccel;

	//計算回数の更新
	mAccelCount++;

	return vel;
}

void AccelState::InitState()
{
	//ステートの初期化をする
	//加速度を初期化する
	mAccel = Vector3::Zero;

	//加速回数を初期化する
	mAccelCount = NULL;

	//実行関数を初期化する
	this->SetExecute(false);
}
