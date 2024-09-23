#include "pch.h"
#include "SoapBabble.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameObject/TimeLag/TimeLag.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Player/NormalTechnique/NormalTechnique.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

SoapBabble::SoapBabble(ITextureBase* texture):TextureBase(texture)
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("シャボン玉"))
	{
		//アニメーションの使用するテクスチャを取得する
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//アニメーション情報を設定する
		this->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//次のアニメーションの情報を設定する
		this->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//アニメーションの終了時間を設定する
		this->SetEndTime(importer->GetAnimation(),importer->GetEnd());
	}

	this->SetLoopFlag(true);
	this->SetAniFlag(true);

	this->SetNowInfo("シャボン玉1");
	this->SetLoopInfo("シャボン玉6");

	this->SetNowAnimation(this->GetRect());

	//更新関数を起こす
	this->SetActive(true);
}

void SoapBabble::Initialize()
{
}

void SoapBabble::Update()
{
	//UIの表示が終わるまで何もしない
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//描画されていなければ、何もしない
	if (this->GetState() == STATE::NONE)return;

	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove())return;

	//実行関数が落ちているときは、何もしない
	if (!this->GetActive())return;
	
	//アニメーションの更新の処理
	Animation::Update();

	//画像の更新処理
	TextureBase::Update();

	//切り出し位置の設定
	this->SetRect(this->GetNowAnimation());

	//当たり判定の座標を設定する
	this->SetCenter(this->GetPosition());

	//画像の大きさを当たり判定に使用する
	this->SetRadius(Vector3(this->AskOrigin() * this->GetScale()));

	//当たり判定を追加する
	GameContext::Get<CollisionManager>()->AddUseHero(this);

	//NormalTequniqueを取得する
	NormalTechnique* nTequ = GameContext::Get<NormalTechnique>();

	//NeedleOverAllを取得する
	NeedleOverall* needleOverAll = GameContext::Get<NeedleOverall>();

	//Resurrectionを取得する
	Resurrection* resurrection = GameContext::Get<Resurrection>();

	for (NormalBullet* bullet : nTequ->GetBulletsList())
	{
		//敵のHPが0より大きいときに当たり判定を追加する
		if (this->GetOwner()->GetStatus()->GetHP() > NULL)
		{
			//当たり判定を追加する
			GameContext::Get<CollisionManager>()->AddUseOther(this, bullet, 200.0f);
		}
	}

	for (Needle* needle : needleOverAll->GetNeedleList())
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 300.0f);
	}

	for (Needle* needle : resurrection->GetNeedleList())
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 400.0f);
	}
}

void SoapBabble::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void SoapBabble::Finalize()
{
}

void SoapBabble::OnCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::OnCollisionEnter(obj);

	//プレイヤーと当たった時の処理
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);

	//ウイルスビームと当たったときの処理
	if (obj->GetName() == "ウイルスビーム")this->OnCollision_VirusBeam(obj);

	//NormalBulletと当たった時の処理
	if (obj->GetName() == "通常弾")this->OnCollision_NormalBullet(obj);

	//WhiteBackと当たった時の処理
	if (obj->GetName() == "WhiteBack")this->OnCollision_WhiteBack(obj);

	//Needleと当たったときの処理
	if (obj->GetName() == "shortNeedle" || obj->GetName() == "longNeedle")
	{
		this->OnCollision_Needle(obj);
	}
}

void SoapBabble::NonCollisionEnter(ITextureBase* obj)
{
	//当たり判定の処理
	CollisionBase::NonCollisionEnter(obj);
}

void SoapBabble::OnCollision_Player(ITextureBase* player)
{
	//プレイヤーと当たった時の処理
	
	//描画を消す
	this->SetState(STATE::NONE);
}

void SoapBabble::OnCollision_VirusBeam(ITextureBase* beam)
{
	//ウイルスビームと当たった時の処理

	//描画を消す
	this->SetState(STATE::NONE);

}

void SoapBabble::OnCollision_NormalBullet(ITextureBase* bullet)
{
	//通常弾と当たった時の処理

	//描画を消す
	this->SetState(STATE::NONE);
}

void SoapBabble::OnCollision_WhiteBack(ITextureBase* whiteback)
{
	//背景と当たった時の処理
	int i = 0;

	//描画を消す
	this->SetState(STATE::NONE);
}

void SoapBabble::OnCollision_Needle(ITextureBase* needle)
{
	//針と当たった時の処理

	//描画を消す
	this->SetState(STATE::NONE);

}
