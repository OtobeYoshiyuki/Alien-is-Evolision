#include "pch.h"
#include "Heal.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//1フレームに加算する速度
const float	Heal::JUMP_SPEED = 5.0f;

//ジャンプの限界値(最初)
const float	Heal::JUMP_LIMIT_FIRST = 50.0f;

//初期位置
const Vector3 Heal::INIT_POS = Vector3(0.0f, -200.0f, 0.0f);

//初期倍率
const Vector3 Heal::INIT_SCALE = Vector3(0.1f);

//上昇から、落下までの補正値
const float	Heal::CORRECTION = 2.0f;

//バウンド係数
const float	Heal::BOUND_POWER = 5.0f;

Heal::Heal(ITextureBase* texture):
	Item(texture),//基底クラスに情報を送信する
	m_jumpState(nullptr),//JumpState
	m_healPower(0.0f)//回復量
{
	//JumpStateを生成する
	m_jumpState = std::make_unique<JumpState>();
	m_jumpState->SetExecute(false);
	m_jumpState->SetJumpSpeed(JUMP_SPEED);
	m_jumpState->SetJumpLimit(JUMP_LIMIT_FIRST);
	m_jumpState->SetJumpLimitCount(JUMP_COUNT);
	m_jumpState->SetCorrection(CORRECTION);
	m_jumpState->SetJumpOption(false);

	//MoveStateに追加する
	m_stateList.push_back(m_jumpState.get());
}

void Heal::Initialize()
{
}

void Heal::Update()
{
	//UIの表示が終わるまで何もしない
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//CollisionManagerを取得する
	CollisionManager* collsionManager = GameContext::Get<CollisionManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove())return;

	//ステートの更新処理
	this->UpdateState();

	//画像の更新処理
	Item::Update();

	//当たり判定を追加する
	collsionManager->AddUseHero(this);

	//ステートが実行中で、着地したとき
	//バウンド中の時
	if (m_jumpState->GetExecute() && this->GetNowState() == BOUND::BOUND)
	{
		//着地
		if (m_jumpState->GetNowState() == JUMP::LANDING)
		{
			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__34_バウンド);
		}
	}
}

void Heal::Render()
{
	//画像の描画処理
	Item::Render();
}

void Heal::Finalize()
{
}

void Heal::OnCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	Item::OnCollisionEnter(obj);

	//Playerに当たったとき
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);

	//Floorに当たったとき
	if (obj->GetName() == "floor")this->OnCollision_Floor(obj);

	//Wallに当たったとき
	if (obj->GetName() == "wall")this->OnCollision_Wall(obj);

}

void Heal::NonCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	Item::NonCollisionEnter(obj);
}

void Heal::OnFall(const Vector3& pos)
{
	//ステートを切り替える
	this->SetNowState(BOUND::FALL);

	//座標を設定する
	this->SetPosition(pos);

	//実行関数を起こす
	m_jumpState->SetExecute(true);

	//ステートを切り替える
	m_jumpState->SetNowState(JUMP::FALL);

	//描画を起こす
	this->SetState(STATE::DRAW);

	//リストに値を追加する
	m_jumpState->AddFall();
}

void Heal::UpdateState()
{
	//ベクトルの初期化
	Vector3 vel = Vector3::Zero;

	//実行中のステートからベクトルを取得する
	for (MoveState* state : m_stateList)
	{
		vel += state->Update();
	}

	//ベクトルを設定する
	this->SetVelocity(vel);

	//Switch文で処理を分ける
	switch (this->GetNowState())
	{
	case BOUND::BOUND:
		this->State_Bound();
		break;
	case BOUND::RESULT:
		this->State_Result();
		break;
	}

}

void Heal::State_Bound()
{
	//指定した回数ジャンプした
	if (m_jumpState->GetJumpmDesignation())
	{
		//ステートを切り替える
		this->SetNowState(BOUND::RESULT);

		//実行関数を落とす
		m_jumpState->SetExecute(false);
	}
}

void Heal::State_Result()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//SEを停止する
	soundManager->Stop(CRI_CUESHEET_0__35_落下);

	//実行関数を落とす
	m_jumpState->SetExecute(false);

	//ステートを切り替える
	this->SetNowState(BOUND::NONE);
}

void Heal::OnCollision_Player(ITextureBase* obj)
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//SEを再生する
	soundManager->Play(CRI_CUESHEET_0__22_獲得);

	//画像を押し出す
	this->Extrude(this);

	//取得したアイテムを追加する
	GameContext::Get<PlayScene>()->GetHealManager()->AddItem(this);

	//描画を切る
	this->SetState(STATE::NONE);

	//ステートを切り替える
	this->SetNowState(BOUND::RESULT);
}

void Heal::OnCollision_Floor(ITextureBase* obj)
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//画像を押し出す
	this->Extrude(this);
	
	//落下中の時は、ジャンプさせる
	if (this->GetNowState() == BOUND::FALL)
	{
		//ステートを切り替える
		this->SetNowState(BOUND::BOUND);

		//ステートを切り替える
		m_jumpState->SetNowState(JUMP::LANDING);

		//リストを空にする
		m_jumpState->RemoveFall();

		//ジャンプオプションを設定する
		m_jumpState->SetJumpOption(true);

		//バウンドする値を設定する
		m_jumpState->SetBoundPower(BOUND_POWER);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__34_バウンド);
	}
}

void Heal::OnCollision_Wall(ITextureBase* obj)
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//画像を押し出す
	this->Extrude(this);

	//落下中の時は、ジャンプさせる
	if (this->GetNowState() == BOUND::FALL)
	{
		//ステートを切り替える
		this->SetNowState(BOUND::BOUND);

		//ステートを切り替える
		m_jumpState->SetNowState(JUMP::LANDING);

		//リストを空にする
		m_jumpState->RemoveFall();

		//ジャンプオプションを設定する
		m_jumpState->SetJumpOption(true);

		//バウンドする値を設定する
		m_jumpState->SetBoundPower(BOUND_POWER);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__34_バウンド);
	}
}
