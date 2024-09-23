#include "pch.h"
#include "HealManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//アイテムをだす間隔(1分)
const float HealManager::FIRING_INTERVAL = 60.0f;

HealManager::HealManager():
	m_timer(0.0f)//経過時間
{
}

void HealManager::Initialize()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//使用するテクスチャを取得する
	ITextureBase* useTexture = resourceManager->GetTexture("回復");

	//PlaySceneを取得する
	PlayScene* play = GameContext::Get<PlayScene>();

	//添え字を初期化する
	this->SetNowState(NULL);

	for (int i = 0; i < HEALITEM_NUM; i++)
	{
		//回復アイテムを生成する
		this->CreateItem(useTexture, [&](Item* item)
			{
				Heal* healItem = dynamic_cast<Heal*>(item);
				m_healItem.push_back(healItem); });
	}

	//前回取得したアイテムを反映する
	std::vector<std::string> itemData = play->GetSaveDataArray((int)PlayScene::SAVE_DATA::HEAL_INFO);

	//アイテムを獲得していない場合は、何もしない
	if (itemData[0] == "なし")return;

	//取得したアイテムの数だけ、獲得した扱いにする
	for (std::string& itemStr : itemData)
	{
		//使用するテクスチャを取得する
		ITextureBase* itemTexture = resourceManager->GetTexture(itemStr);

		//アイテムを生成する
		std::unique_ptr<Item> pItem = std::make_unique<Item>(itemTexture);

		//描画を切る
		pItem->SetState(ITextureBase::STATE::NONE);

		//追加する
		this->AddItem(std::move(pItem));
	}
}

void HealManager::Update()
{
	//UIの表示が終わるまで何もしない
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//前のフレームからの経過時間
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove())return;

	//添え字を取得する
	int firing = this->GetNowState();

	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//自身の座標を取得する
	Vector3 pos = m_healItem[firing]->GetPosition();

	//座標を設定する
	pos = Vector3(player->GetPosition().x, pos.y, pos.z);

	//1分経過したら、回復アイテムを出す
	if (m_timer > FIRING_INTERVAL)
	{
		//添え字が範囲内を参照する場合のみ
		if (firing < (int)m_healItem.size())
		{
			//落下処理を実行する
			m_healItem[firing]->OnFall(pos);

			//音を再生する
			soundManager->Play(CRI_CUESHEET_0__35_落下);

			//添え字を更新する
			firing++;

			//ステートを更新する
			this->SetNowState(firing);

			//経過時間を初期化する
			m_timer = 0.0f;
		}

		//時間の更新は行わない
		return;
	}

	//時間を更新する
	m_timer += elapsedTime;
}

//回復アイテムを生成する
void HealManager::CreateItem(ITextureBase* refTex, std::function<void(Item*)> func)
{
	//TextureManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//Stageを取得する
	Stage* stage = GameContext::Get<Stage>();

	//マップ情報を取得する
	std::vector<std::vector<ITextureBase*>>& mapAll = stage->GetMap();

	//テクスチャを生成する
	std::unique_ptr<Item> phealItem = std::make_unique<Heal>(refTex);

	//描画を消す
	phealItem->SetState(ITextureBase::STATE::NONE);

	//初期位置を設定する
	phealItem->SetPosition(Heal::INIT_POS);

	//原点を設定する
	phealItem->SetOrigin(phealItem->AskOrigin());

	//拡大倍率を設定する
	phealItem->SetScale(Heal::INIT_SCALE);

	//生ポインタを登録する
	func(phealItem.get());

	//生ポインタを登録する
	m_allItem.push_back(phealItem.get());

	//マップ情報を追加する
	mapAll[mapAll.size() - 1].push_back(phealItem.get());

	//TextureManagerに追加する
	gameManager->Add(std::move(phealItem));
}
