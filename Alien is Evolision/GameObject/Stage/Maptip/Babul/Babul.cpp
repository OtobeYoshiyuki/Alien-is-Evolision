#include "pch.h"
#include "Babul.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameObject/Technique/NeedleOverall/NeedleOverall.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Babul::Babul(ITextureBase* texture):MapTip(texture)
{
}

void Babul::Initialize()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Babble"))
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

	//アニメーションの情報を設定する

	//更新フラグを設定する
	this->SetAniFlag(true);

	//ループフラグを設定する
	this->SetLoopFlag(true);

	//現在のアニメーション情報を設定する
	this->SetNowInfo("Babble1");

	//ループするアニメーション情報を設定する
	this->SetLoopInfo("Babble9");

	//アニメーションを設定する
	this->SetNowAnimation(this->GetRect());
}

void Babul::Update()
{
	MapTip::Update();

	Animation::Update();

	//現在のアニメーション情報を設定する
	this->SetRect(Animation::GetNowAnimation());

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//プレイヤーが生存しているとき
	if (player->GetHP() > NULL)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);
	}

	//ニードルの長距離範囲攻撃を取得する
	std::vector<Needle*> overNeedleList = GameContext::Get<NeedleOverall>()->GetNeedleList();

	for (Needle* needle : overNeedleList)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 100.0f);
	}

	//ニードルの短距離範囲攻撃を取得する
	std::vector<Needle*> reNeedleList = GameContext::Get<Resurrection>()->GetNeedleList();

	for (Needle* needle : reNeedleList)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 300.0f);
	}
}

void Babul::Render()
{
	MapTip::Render();
}

void Babul::Finalize()
{
}

void Babul::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);
}

void Babul::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}
