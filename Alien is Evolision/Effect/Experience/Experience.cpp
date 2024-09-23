#include "pch.h"
#include "Experience.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//敵が消滅してから、経験値が動き出すまでの待ち時間
const float Experience::WAIT_TIME = 1.0f;

Experience::Experience(ITextureBase* texture) :
	TextureBase(texture),//基底クラスに情報を送信する
	m_tracking(nullptr),//追尾用のステート
	m_homing(nullptr),//ホーミング用のステート
	m_timer(0.0f)//経過時間
{
	//追跡用のステートを生成する
	m_tracking = std::make_unique<TrackingState>();

	//ステートを切る
	m_tracking->SetExecute(false);

	//ホーミング用のステータスを生成する
	m_homing = std::make_unique<HomingState>();

	//ステートを切る
	m_homing->SetExecute(false);

	//パラメーターの初期化をする
	m_homing->SetMaxVel(Vector3(0.0f, -300.0f, 0.0f));
	m_homing->SetMinVel(Vector3::Zero);
	m_homing->SetDirVel(Vector3(0.0f, -5.0f, 0.0f));

	//リストに追加する
	m_stateList.push_back(m_tracking.get());
	m_stateList.push_back(m_homing.get());
}

void Experience::Initialize()
{
}

void Experience::Update()
{
	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

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
		GameContext::Get<CollisionManager>()->AddUseOther(this,player, 200.0f);
	}
}

void Experience::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void Experience::Finalize()
{
}

void Experience::OnCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::OnCollisionEnter(obj);

	//プレイヤーと当たった時の処理
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);
}

void Experience::NonCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::NonCollisionEnter(obj);
}

void Experience::UpdateState()
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

void Experience::State_Wait()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//elapsedTimeを取得する
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//時間が経過したら
	if (m_timer > WAIT_TIME)
	{
		//ステートを切り替える
		this->SetNowState(TARGET_STATE::TRACKING);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__32_アイテム追跡);

		//時間を初期化する
		m_timer = 0.0f;

		//時間を更新しない
		return;
	}

	//時間を更新する
	m_timer += elapsedTime;
}

void Experience::State_Tracking()
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

void Experience::OnCollision_Player(ITextureBase* player)
{
	//基底クラスから、派生クラスにキャストする
	CoroKun* coro = dynamic_cast<CoroKun*>(player);

	//所有者（Enemy）のステータスを取得する
	IStatusBase* enemyStatus = this->GetOwner()->GetStatus();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//経験値が追尾中の時
	if (this->GetNowState() == TARGET_STATE::TRACKING)
	{
		//描画を切る
		this->SetState(STATE::NONE);

		//ベクトルを初期化する
		this->SetVelocity(Vector3::Zero);

		//ステートを切り替える
		this->SetNowState(TARGET_STATE::CHANGE);

		//経験値を取得していない場合
		if (!coro->GetExperienceGage()->CheckExp())
		{
			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__33_ゲージ上昇);
		}

		//変数の初期化
		int Exp, texExp;
		float ratio;

		//取得経験値を求める
		StatusCalc::CalcExp(enemyStatus->GetLevel(), enemyStatus->GetCompensation(), 50,
			coro->GetExperienceGage()->GetMaxTexLength(),coro->GetMaxNextLevelPoint(),Exp,texExp,ratio);

		//経験値を設定する(画像用)
		coro->GetExperienceGage()->PushExp(texExp);
		coro->GetExperienceGage()->SetRatio(ratio);

		//経験値を設定する(プレイヤー用)
		coro->SetNextLevelPoint(coro->GetNextLevelPoint() + Exp);

		//経験値ゲージのステートを設定する
		coro->GetExperienceGage()->SetNowState(CALC::CALCING);
	}
}
