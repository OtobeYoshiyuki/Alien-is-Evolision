#include "pch.h"
#include "CheckPoint.h"
#include "GameContext.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Enemy/Normal/NormalEnemy.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CheckPoint::CheckPoint(ITextureBase* texture):TextureBase(texture)
{
	//画像の大きさを当たり判定に使用する
	this->SetRadius(Vector3(this->AskOrigin()*2));
}

void CheckPoint::Initialize()
{
}

void CheckPoint::Update()
{
	//画像の更新処理
	TextureBase::Update();

	//当たり判定の座標を設定する
	this->SetCenter(this->GetPosition() + this->AskOrigin());

	//HyperEnemyのvectorを取得する
	std::vector<HyperEnemy*>HyperEnemyList = GameContext::Get<Stage>()->GetHyperEnemies();

	for (HyperEnemy* Hyper : HyperEnemyList)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, Hyper, 200.0f);
	}

	//NormalEnemyのvectorを取得する
	std::vector<NormalEnemy*>NormalEnemyList = GameContext::Get<Stage>()->GetNormalEnemies();

	//NormalEnemyのvectorを取得する
	for (NormalEnemy* Normal : NormalEnemyList)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, Normal, 200.0f);
	}
}

void CheckPoint::Render()
{
	////画像の描画処理
	//TextureBase::Render();
}

void CheckPoint::Finalize()
{
}

void CheckPoint::OnCollisionEnter(ITextureBase* obj)
{
	CollisionBase::OnCollisionEnter(obj);
}

void CheckPoint::NonCollisionEnter(ITextureBase* obj)
{
	CollisionBase::NonCollisionEnter(obj);
}
