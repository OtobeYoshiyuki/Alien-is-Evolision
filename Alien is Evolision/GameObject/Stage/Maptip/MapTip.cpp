#include "pch.h"
#include "MapTip.h"
#include "GameObject/Player/CoroKun.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Stage/Maptip/Floor/Floor.h"
#include "GameObject/Stage/Maptip/Wall/Wall.h"
#include "GameObject/Stage/Maptip/Babul/Babul.h"
#include "GameObject/Stage/Maptip/Rebound/Rebound.h"
#include "GameObject/Stage/Maptip/HoleSwitch/HoleSwitch.h"
#include "GameObject/Stage/Maptip/HoleFloor/HoleFloor.h"
#include "GameObject/Stage/Maptip/Goal/Goal.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "GameObject/Enemy/Normal/NormalEnemy.h"
#include "GameObject/CheckPoint/CheckPoint.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

MapTip::MapTip(ITextureBase* texture) :TextureBase(texture)
{
	this->SetDrawOrder(3);
}

void MapTip::Initialize()
{
}

void MapTip::Update()
{
	//UIの表示が終わるまで何もしない
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//画像の更新処理
	TextureBase::Update();

	//画像の大きさを当たり判定に使用する
	this->SetRadius(Vector3(this->AskOrigin()));

	//当たり判定の座標を設定する
	this->SetCenter(this->GetPosition() + this->AskOrigin());
}

void MapTip::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void MapTip::Finalize()
{
}

void MapTip::CreateMapTip(const int& tipType, std::unique_ptr<ITextureBase>& maptip)
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//使用するテクスチャを取得する
	ITextureBase* mapTip = resourceManager->GetTexture("マップチップ");
	ITextureBase* goal = resourceManager->GetTexture("ゴール");
	ITextureBase* player = resourceManager->GetTexture("SickChar");
	ITextureBase* Hyper = resourceManager->GetTexture("ハイパー1");
	ITextureBase* Super = resourceManager->GetTexture("スーパー1");
	ITextureBase* Normal = resourceManager->GetTexture("ノーマル4");

	//switch文で条件を分ける
	switch ((MAP_TYPE)tipType)
	{
	case MAP_TYPE::PLAYER:
		maptip = std::make_unique<CoroKun>(player);
		break;
	case MAP_TYPE::FLOOR:
		maptip = std::make_unique<Floor>(mapTip);
		maptip->SetName("floor");
		maptip->SetRect(RECT{ 0,0,40,40 });
		break;
	case MAP_TYPE::WALL:
		maptip = std::make_unique<Wall>(mapTip);
		maptip->SetName("wall");
		maptip->SetRect(RECT{ 0,40,40,80 });
		break;
	case MAP_TYPE::BABUL:
		maptip = std::make_unique<Babul>(mapTip);
		maptip->SetName("babul");
		maptip->SetRect(RECT{ 0,80,40,120 });
		break;
	case MAP_TYPE::REBOUND:
		maptip = std::make_unique<Rebound>(mapTip);
		maptip->SetName("rebound");
		maptip->SetRect(RECT{ 0,120,40,160 });
		break;
	case MAP_TYPE::GOAL:
		maptip = std::make_unique<Goal>(goal);
		break;
	case MAP_TYPE::ENEMYN:
		maptip = std::make_unique<NormalEnemy>(Normal);
		maptip->SetName("NormalEnemy");
		break;
	case MAP_TYPE::ENEMYS:
		maptip = std::make_unique<SuperEnemy>(Super);
		maptip->SetName("SuperEnemy");
		break;
	case MAP_TYPE::ENEMYH:
		maptip = std::make_unique<HyperEnemy>(Hyper);
		maptip->SetName("HyperEnemy");
		break;
	case MAP_TYPE::LEFTPANEL:
		maptip = std::make_unique<CheckPoint>(mapTip);
		maptip->SetName("LeftPanel");
		maptip->SetRect(RECT{ 0,0,1,1 });
		maptip->SetState(STATE::DRAW);
		break;
	case MAP_TYPE::RIGHTPANEL:
		maptip = std::make_unique<CheckPoint>(mapTip);
		maptip->SetName("RightPanel");
		maptip->SetRect(RECT{ 0,0,1,1 });
		maptip->SetState(STATE::DRAW);
		break;
	case MAP_TYPE::LEFTJUMPPANEL:
		maptip = std::make_unique<CheckPoint>(mapTip);
		maptip->SetName("LeftJumpPanel");
		maptip->SetRect(RECT{ 0,0,1,1 });
		maptip->SetState(STATE::DRAW);
		break;
	case MAP_TYPE::RIGHTJUMPPANEL:
		maptip = std::make_unique<CheckPoint>(mapTip);
		maptip->SetName("RightJumpPanel");
		maptip->SetRect(RECT{ 0,0,1,1 });
		maptip->SetState(STATE::DRAW);
		break;
	}

	//複数あるものとして考えているので、Switchでは制御しない
	if ((MAP_TYPE)tipType >= MAP_TYPE::HOLE)
	{
		//起爆スイッチの設定をする(起爆スイッチは必ず偶数)
		if (tipType % 2 == 0)
		{
			int SwitchSeleal = tipType / 2 - 7;
			maptip = std::make_unique<HoleSwitch>(mapTip);
			maptip->SetName("switch");
			maptip->SetRect(RECT{ 0,160,40,200 });
			dynamic_cast<HoleSwitch*>(maptip.get())->SetSerealNumber(SwitchSeleal);
		}
		//落とし穴の設定をする(落とし穴は必ず奇数)
		else
		{
			int HoleSeleal = (tipType - 1) / 2 - 7;
			maptip = std::make_unique<HoleFloor>(mapTip);
			maptip->SetName("hole");
			maptip->SetRect(RECT{ 0,0,40,40 });
			dynamic_cast<HoleFloor*>(maptip.get())->SetSerealNumber(HoleSeleal);
		}
	}

}

void MapTip::OnCollisionEnter(ITextureBase* obj)
{
	//他のオブジェクトと当たった時に呼ばれる関数
	CollisionBase::OnCollisionEnter(obj);
}

void MapTip::NonCollisionEnter(ITextureBase* obj)
{
	//他のオブジェクトと当たっていないときに呼ばれる関数
	CollisionBase::NonCollisionEnter(obj);
}
