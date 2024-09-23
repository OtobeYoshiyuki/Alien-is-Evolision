#include "pch.h"
#include "DamageState.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "GameObject/Player/CoroKun.h"
#include "GameObject/MoveState/WalkState/WalkState.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

DamageState::DamageState():mDamage(Vector3::Zero),mDamageCount(NULL),mFirstDamage(Vector3::Zero)
{
}

const Vector3 DamageState::Update()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//ベクトルの初期化
	Vector3 vel = Vector3::Zero;

	//プレイヤー情報を取得
	CoroKun* player = GameContext::Get<CoroKun>();

	//実行関数がfalseのときは何もしない
	if (!this->GetExecute())return vel;

	//7回以上ダメージを与えたら何もしない
	if (mDamageCount > MAX_DAMAGE)
	{
		//ステートの初期化
		this->InitDamageState();
		return vel;
	}

	//ダメージからベクトルを算出
	vel = mDamage * abs(mDamageCount - MAX_DAMAGE) + mDamage;

	//初回ダメージを記憶する
	if (mDamageCount == NULL)mFirstDamage = vel / 8;

	//ダメージ数を更新する
	mDamageCount++;

	//ベクトルを返す
	return vel;
}

void DamageState::InitDamageState()
{
	//ステートの初期化を行う
	mDamage = Vector3::Zero;
	mDamageCount = NULL;
	mFirstDamage = Vector3::Zero;
	this->SetExecute(false);
}
