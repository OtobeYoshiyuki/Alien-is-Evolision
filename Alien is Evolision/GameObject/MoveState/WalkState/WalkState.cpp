#include "pch.h"
#include "WalkState.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "GameObject/Player/CoroKun.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

WalkState::WalkState():mWalk(Vector3::Zero)
{
}

const Vector3 WalkState::Update()
{
	//キーの状態を取得する
	Input* input = Input::GetInstance();

	//キーボードの状態を取得する
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//実行関数がfalse
	if (!this->GetExecute())return vel;

	//ベクトルを設定する
	if (keyState.Left)vel.x = -CoroKun::SPEED;

	if (keyState.Right)vel.x = CoroKun::SPEED;

	if (keyState.Up)vel.y = -CoroKun::SPEED;

	if (keyState.Down)vel.y = CoroKun::SPEED;

	//ベクトルを返す
	return vel;
}
