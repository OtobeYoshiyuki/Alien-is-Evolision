#include "pch.h"
#include "TechniqueRecovery.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//敵が消滅してから、経験値が動き出すまでの待ち時間
const float TechniqueRecovery::WAIT_TIME = 1.0f;

TechniqueRecovery::TechniqueRecovery(ITextureBase* texture):
	TextureBase(texture),//基底クラスに情報を送信する
	m_tracking(nullptr),//追跡用のステート
	m_timer(0.0f),//時間
	m_targetGage(nullptr)//回復させる対象のゲージ
{
	//追跡用のステートを生成する
	m_tracking = std::make_unique<TrackingState>();

	//ステートを切る
	m_tracking->SetExecute(false);

	//リストに追加する
	m_stateList.push_back(m_tracking.get());
}

void TechniqueRecovery::Initialize()
{
}

void TechniqueRecovery::Update()
{
	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//ステートの更新処理
	this->UpdateState();

	//画像の更新処理
	TextureBase::Update();

	//プレイヤーと経験値の当たり判定を設定する
	if (this->GetNowState() == TARGET_STATE::TRACKING)
	{
		//当たり判定を設定する
		GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);
	}
}

void TechniqueRecovery::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void TechniqueRecovery::Finalize()
{
}

void TechniqueRecovery::OnCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::OnCollisionEnter(obj);

	//プレイヤーと当たった時の処理
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);
}

void TechniqueRecovery::NonCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::NonCollisionEnter(obj);
}

void TechniqueRecovery::UpdateState()
{
	//switch文で処理を分ける
	switch (this->GetNowState())
	{
	case TARGET_STATE::WAIT:
		this->State_Wait();
		break;
	case TARGET_STATE::TRACKING:
		this->State_Tracking();
		break;
	}
}

void TechniqueRecovery::State_Wait()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//elapsedTimeを取得する
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//時間が経過したら
	if (m_timer > WAIT_TIME)
	{
		//ステートを切り替える
		this->SetNowState(TARGET_STATE::TRACKING);

		//時間を初期化する
		m_timer = 0.0f;

		//時間を更新しない
		return;
	}

	//時間を更新する
	m_timer += elapsedTime;
}

void TechniqueRecovery::State_Tracking()
{
	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//実行中のベクトルを加算していく
	for (MoveState* state : m_stateList)
	{
		vel += state->Update();
	}

	//ベクトルを設定する
	this->SetVelocity(vel);
}

void TechniqueRecovery::OnCollision_Player(ITextureBase* player)
{
	//基底クラスから、派生クラスにキャストする
	CoroKun* coro = dynamic_cast<CoroKun*>(player);

	//所有者（Enemy）のステータスを取得する
	IStatusBase* enemyStatus = this->GetOwner()->GetStatus();

	//経験値が追尾中の時
	if (this->GetNowState() == TARGET_STATE::TRACKING)
	{
		//描画を切る
		this->SetState(STATE::NONE);

		//ベクトルを初期化する
		this->SetVelocity(Vector3::Zero);

		//ステートを切り替える
		this->SetNowState(TARGET_STATE::CHANGE);

	}

}
