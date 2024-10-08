#include "pch.h"
#include "Floor.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/CheckPoint/CheckPoint.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Floor::Floor(ITextureBase* texture):MapTip(texture)
{
}

void Floor::Initialize()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Floor"))
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
	this->SetNowInfo("Floor1");

	//ループするアニメーション情報を設定する
	this->SetLoopInfo("Floor13");

	//アニメーションを設定する
	this->SetNowAnimation(this->GetRect());
}

void Floor::Update()
{
	MapTip::Update();

	Animation::Update();

	//現在のアニメーション情報を設定する
	this->SetRect(Animation::GetNowAnimation());

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//当たり判定を追加する
	GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);

	//ニードルの長距離範囲攻撃を取得する
	std::vector<Needle*> needleList = GameContext::Get<NeedleOverall>()->GetNeedleList();

	for (Needle* needle : needleList)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 100.0f);
	}

	//とても強い敵のvectorを取得する
	std::vector<HyperEnemy*>HyperEnemyList = GameContext::Get<Stage>()->GetHyperEnemies();

	for (HyperEnemy* Hyper : HyperEnemyList)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, Hyper, 100.0f);
	}

	//アイテムのリストを取得する
	std::vector<Item*> itemList = GameContext::Get<PlayScene>()->GetHealManager()->GetAllItem();

	for (Item* item : itemList)
	{
		//当たり判定を設定する
		GameContext::Get<CollisionManager>()->AddUseOther(this, item, 100.0f);
	}

}

void Floor::Render()
{
	MapTip::Render();
}

void Floor::Finalize()
{
}

void Floor::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);
}

void Floor::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}
