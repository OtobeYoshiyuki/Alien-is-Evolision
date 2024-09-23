#include "pch.h"
#include "LevelUp.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

LevelUp::LevelUp(ITextureBase* texture):
	TextureBase(texture)//基底クラスに情報を送信する
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Level"))
	{
		//アニメーションの使用するテクスチャを取得する
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//アニメーション情報を設定する
		this->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//次のアニメーションの情報を設定する
		this->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//アニメーションの終了時間を設定する
		this->SetEndTime(importer->GetAnimation(), importer->GetEnd());
	}

	//アニメーションのフラグを設定する
	this->SetAniFlag(false);
	this->SetLoopFlag(false);

	//現在のアニメーション情報を設定する
	this->SetNowInfo("Level1");
	this->SetLoopInfo("Level10");
	this->SetNowAnimation(this->GetNowAnimation());

	//実行関数を起こす
	this->SetActive(true);
}

void LevelUp::Initialize()
{
	//座標を設定する
	this->SetPosition(Vector3(600, 100, 0));
}

void LevelUp::Update()
{
	//UIの表示が終わるまで何もしない
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//実行関数が落ちているときは、何もしない
	if (!this->GetActive())return;

	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove())return;

	//ステートの更新処理
	this->UpdateState();

	//アニメーションの更新処理
	Animation::Update();

	//画像の更新処理
	TextureBase::Update();

	//画像の切り出し位置を更新する
	this->SetRect(this->GetNowAnimation());
}

void LevelUp::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void LevelUp::Finalize()
{
}

void LevelUp::UpdateState()
{
	//Switch文で処理を分ける

	switch (this->GetNowState())
	{
	case LEVELUP::NONE:
		this->State_None();
		break;
	case LEVELUP::LEVELUP:
		this->State_LevelUp();
		break;
	case LEVELUP::ANIMATION:
		this->State_Animation();
		break;
	case LEVELUP::DOWN:
		this->State_Down();
		break;
	case LEVELUP::RESULT:
		this->State_Result();
		break;
	}
}

void LevelUp::State_None()
{
	//State_Noneの処理

	//描画を切る
	this->SetState(STATE::NONE);

	//色を設定する
	this->SetColor(Colors::White);
}

void LevelUp::State_LevelUp()
{
	//State_LevelUpの処理

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//描画を起こす
	this->SetState(STATE::DRAW);

	//アニメーションを起動させる
	this->SetAniFlag(true);

	//ステートを切り替える
	this->SetNowState(LEVELUP::ANIMATION);

	//SEを再生する
	soundManager->Play(CRI_CUESHEET_0__12_レベルアップ);
}

void LevelUp::State_Animation()
{
	//State_Animationの処理

	//アニメーションが終了するまで待つ
	if (!this->GetAniFlag())
	{
		//アニメーションが終了したら、ステートを切り替える
		this->SetNowState(LEVELUP::DOWN);
	}
}

void LevelUp::State_Down()
{
	//State_Downの処理

	//色を取得する
	Color color = this->GetColor();

	//アルファ値を取得する
	float alfa = color.A();

	//アルファ値を計算する
	alfa -= 0.01f;

	//アルファ値を再設定する
	color.A(alfa);

	//アルファ値が0未満の場合
	if (alfa < 0.0f)
	{
		//アルファ値を補正する
		alfa = 0.0f;

		//ステートを切り替える
		this->SetNowState(LEVELUP::RESULT);
	}

	//色を再設定する
	this->SetColor(color);
}

void LevelUp::State_Result()
{
	//State_Resultの処理

	//経験値ゲージを取得する
	ExperienceGage* exGage = GameContext::Get<CoroKun>()->GetExperienceGage();

	//ステートを切り替える
	this->SetNowState(LEVELUP::NONE);

	//自身のステートを切り替える
	exGage->SetNowState(CALC::CALCING);
}
