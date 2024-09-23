#include "pch.h"
#include "TargetScrollCamera.h"
#include "GameObject/GameCamera/GameCamera.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Stage/Maptip/HoleFloor/HoleFloor.h"
#include "GameObject/Stage/Stage.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//対象物に移動する移動量
const float TargetScrollCamera::TARGET_TO_VECTOL = 0.05f;

//プレイヤーと対象物との距離
const float TargetScrollCamera::TARGET_TO_LENGTH = 450.0f;

TargetScrollCamera::TargetScrollCamera():
	m_target(Vector3::Zero),//スクロールさせる位置
	m_targetNumber(NULL)//追跡するテクスチャ
{
}

void TargetScrollCamera::Initialize()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//ターゲットスクロールカメラをRegisterに登録する
	GameContext::Register<TargetScrollCamera>(this);

	//ゲームカメラの情報を受け取る
	GameCamera* gameCamera = GameContext::Get<GameCamera>();

	//座標を受け取る
	this->SetPosition(gameCamera->GetPosition());

	//行列を受け取る
	this->SetMatrix(gameCamera->GetMatrix());

	//オブジェクトの移動を止める
	this->SetObjectMove(false);

	//ターゲットを設定する
	this->SetTarget(staticData->Get_Vector3("ターゲット"));

	//ターゲットナンバーを設定する
	this->SetTargetNumber(staticData->Get_IntData("ターゲット"));

	//ステートを設定する
	this->SetNowState(TARGET_STATE::TRACKING);
}

void TargetScrollCamera::Update()
{
	//ステートの更新
	this->UpdateState();
}

void TargetScrollCamera::Finalize()
{
}

void TargetScrollCamera::UpdateState()
{
	//ステートの更新処理

	//Switch文で処理を分ける
	switch (this->GetNowState())
	{
	case TARGET_STATE::TRACKING:
		this->State_Tracking();
		break;
	case TARGET_STATE::WAIT:
		this->State_Wait();
		break;
	case TARGET_STATE::FADEOUT:
		this->State_FadeOut();
		break;
	case TARGET_STATE::FADEIN:
		this->State_FadeIn();
		break;
	case TARGET_STATE::CHANGE:
		this->State_Change();
		break;
	}
}

void TargetScrollCamera::State_Tracking()
{
	//State_Trackingの処理

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//移動量を計算する
	Vector3 vel = player->GetPosition() - m_target;

	//距離を求める
	Vector3 length = Length(player->GetPosition(),m_target);

	//移動量を補正する
	vel *= TARGET_TO_VECTOL;

	//座標を設定する
	this->SetPosition(Vector3(this->GetPosition().x + vel.x, 0.0f, 0.0f));

	//ターゲットを更新する
	m_target += vel;

	//行列を設定する
	this->SetMatrix(Matrix::CreateTranslation(this->GetPosition()));

	//スクロールが完了したら、フラグを起こす
	if (length.x < 10.0f)
	{
		this->SetNowState(TARGET_STATE::WAIT);
	}
}

void TargetScrollCamera::State_Wait()
{
	//State_Waitの処理

	//カウントを初期化する
	int count = NULL;

	//対応する落とし穴のリストを取得する
	std::list<HoleFloor*>holeFloors = HoleFloor::GetHoleGrop(this->GetTargetNumber());

	//対応する落とし穴を取得する
	for (HoleFloor* hole : holeFloors)
	{
		//描画されていなければ、カウントを更新する
		if (hole->GetState() == ITextureBase::STATE::NONE)count++;
	}

	//対応する全ての落とし穴が消されていれば、ステートを切り替える
	if (count >= (int)holeFloors.size())this->SetNowState(TARGET_STATE::FADEOUT);
}

void TargetScrollCamera::State_FadeOut()
{
	//State_FadeOutの処理

	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//キー入力待ちの時のみ、処理をする
	if (sceneState->GetFade() == SceneState::WAITKEY)
	{
		//フェードアウトを行う
		sceneState->SetFadeState(SceneState::FADEOUT);

		//フェードの加算速度を設定する
		sceneState->SetFadeSpeed(0.025f);
	}

	//フェードが次の状態になったら
	if (sceneState->GetFade() == SceneState::FADERESULT)
	{
		//ステートを切り替える
		this->SetNowState(TARGET_STATE::FADEIN);

		//フェードインを行う
		sceneState->SetFadeState(SceneState::FADEIN);

		//CameraManagerを取得する
		CameraManager* cameraManager = GameContext::Get<CameraManager>();

		//カメラを切り替える
		cameraManager->PopCamera();
	}
}

void TargetScrollCamera::State_FadeIn()
{
	//State_FadeInの処理

	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//キー入力待ちのとき
	if (sceneState->GetFade() == SceneState::WAITKEY)
	{
		//フェードの加算速度を設定する
		sceneState->SetFadeSpeed(SceneState::ALFA_SPEED);

		//ステートを切り替える
		this->SetNowState(TARGET_STATE::CHANGE);
	}
}

void TargetScrollCamera::State_Change()
{
	//State_Changeの処理

	//オブジェクトの更新を許可する
	this->SetObjectMove(true);
}
