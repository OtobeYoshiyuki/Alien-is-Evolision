#include "pch.h"
#include "CollisionUI.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "Effect/SoapBabble/SoapBabble.h"
#include "GameObject/FixdTurret/FixedTurret.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CollisionUI::CollisionUI(ITextureBase* texture):TextureBase(texture)
{
}

void CollisionUI::Initialize()
{
	//画像の大きさを当たり判定に使用する
	this->SetRadius(Vector3(this->AskOrigin() * 2));
}

void CollisionUI::Update()
{
	TextureBase::Update();

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//Stageを取得する
	Stage* stage = GameContext::Get<Stage>();

	//当たり判定の座標を設定する
	this->SetCenter(this->GetPosition() + this->AskOrigin());

	//当たり判定を追加する
	GameContext::Get<CollisionManager>()->AddUseOther(this, player->GetName());
	GameContext::Get<CollisionManager>()->AddUseOther(this, "shortNeedle");
	GameContext::Get<CollisionManager>()->AddUseOther(this, "通常弾");
	GameContext::Get<CollisionManager>()->AddUseOther(this, "HyperEnemy");
	GameContext::Get<CollisionManager>()->AddUseOther(this, "SuperEnemy");
	GameContext::Get<CollisionManager>()->AddUseOther(this, "Hypersyabondama");
	GameContext::Get<CollisionManager>()->AddUseOther(this, "Supersyabondama");
}

void CollisionUI::Render()
{
	TextureBase::Render();
}

void CollisionUI::Finalize()
{
}

void CollisionUI::OnCollisionEnter(ITextureBase* obj)
{
	//他のオブジェクトと当たった時に呼ばれる関数
	CollisionBase::OnCollisionEnter(obj);
}

void CollisionUI::NonCollisionEnter(ITextureBase* obj)
{
	//他のオブジェクトと当たっていないときに呼ばれる関数
	CollisionBase::NonCollisionEnter(obj);
}

void CollisionUI::WithPlayer(ITextureBase* texture)
{
	//画像を押し出す
	TextureBase::Extrude(texture);
}
