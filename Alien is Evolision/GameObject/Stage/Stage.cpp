#include "pch.h"
#include "Stage.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Stage/Maptip/Floor/Floor.h"
#include "GameObject/Stage/Maptip/Wall/Wall.h"
#include "GameObject/Stage/Maptip/Babul/Babul.h"
#include "GameObject/Stage/Maptip/Rebound/Rebound.h"
#include "GameObject/Stage/Maptip/HoleSwitch/HoleSwitch.h"
#include "GameObject/Stage/Maptip/HoleFloor/HoleFloor.h"
#include "GameObject/Enemy/Normal/NormalEnemy.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Technique/BodyBlow/BodyBlow.h"
#include "GameObject/CheckPoint/CheckPoint.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

const float Stage::SCROLL_SPEED = 5.0f;

Stage::Stage():m_StageFile(FIRST_STAGE),m_CoroKun(nullptr)
{
}

void Stage::Initialize(const std::wstring& filenamemain, const std::wstring& filenamesub)
{
	//ファイルの状態を取得する
	Stream* File = Stream::GetInstance();

	//ミニマップを取得する
	MiniMap* miniMap = GameContext::Get<MiniMap>();

	//ファイルを開く

	if (File->Open(filenamesub.c_str(), Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		std::vector<std::vector<std::string>>LoadData = File->Load_Data();
		//変換前のデータを保存する
		m_subBeforeData = LoadData;
		//StringからintにConvertする
		std::vector<std::vector<int>>ConvertData = this->FileDataConvert(LoadData);
		//変換後のデータを保存する
		m_subAfterData = ConvertData;
		//配列の縦の要素を確保する
		m_MapTip.resize(ConvertData.size() + 1);
		//マップチップを生成する
		this->CreateMapTip(ConvertData);
		//ファイルを閉じる
		File->Close();
	}

	//ファイルを開く
	if (File->Open(filenamemain.c_str(), Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		std::vector<std::vector<std::string>>LoadData = File->Load_Data();
		//変換前のデータを保存する
		m_mainBeforeData = LoadData;
		//StringからintにConvertする
		std::vector<std::vector<int>>ConvertData = this->FileDataConvert(LoadData);
		//変換後のデータを保存する
		m_mainAfterData = ConvertData;
		//マップチップを生成する
		this->CreateMapTip(ConvertData);
		//ファイルを閉じる
		File->Close();
	}

	//役割ごとに、リストにポインタを登録する
	this->AddVector();

	//それぞれの初期化を呼ぶ
	for (std::vector<ITextureBase*>mapVec : m_MapTip)
	{
		for (ITextureBase* map : mapVec)
		{
			map->Initialize();
		}
	}
}

void Stage::Update()
{
}

const std::vector<std::vector<int>> Stage::FileDataConvert(const std::vector<std::vector<std::string>>& Input)
{
	//変数を宣言する
	std::vector<std::vector<int>>ConvertData;

	//縦の要素数だけ確保する
	ConvertData.resize(Input.size());

	for (size_t i = 0; i < Input.size(); i++)
	{
		for (size_t j = 0; j < Input[i].size(); j++)
		{
			//intに変換したものを追加する
			ConvertData[i].push_back(std::stoi(Input[i][j]));
		}
	}

	//Vectorを返す
	return ConvertData;
}

void Stage::CreateMapTip(const std::vector<std::vector<int>>& Input)
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* mapTip = resourceManager->GetTexture("マップチップ");

	//TextureManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	for (size_t i = 0; i < Input.size(); i++)
	{
		for (size_t j = 0; j < Input[i].size(); j++)
		{
			//std::unique_ptrを宣言する
			std::unique_ptr<ITextureBase>pMapTip;
			//データが空に等しいときは、処理を飛ばす
			if ((MapTip::MAP_TYPE)Input[i][j] == MapTip::MAP_TYPE::NONE)continue;
			//マップチップを生成して、必要な情報を変換する
			MapTip::CreateMapTip(Input[i][j],pMapTip);
			//座標を設定する
			float x = j * /*(pMapTip->GetRect().right - pMapTip->GetRect().left)*/40 + CoroKun::MIN_LIMIT.x;
			float y = i * /*(pMapTip->GetRect().bottom - pMapTip->GetRect().top)*/40 + CoroKun::MIN_LIMIT.y;
			Vector3 pos = Vector3(x, y, 0.0f);
			//マップチップの情報を設定する
			pMapTip->SetPosition(pos);
			//Vector配列に追加する
			m_MapTip[i].push_back(pMapTip.get());
			//TextureManagerに追加する
			gameManager->Add(std::move(pMapTip));
		}
	}
}

void Stage::AddVector()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//ファイルの状態を取得する
	Stream* File = Stream::GetInstance();

	for (size_t i = 0; i < m_MapTip.size(); i++)
	{
		for (size_t j = 0; j < m_MapTip[i].size(); j++)
		{
			//マップチップのポインタを取得
			ITextureBase* mapTip = m_MapTip[i][j];
			//情報が登録されていなかったら処理を飛ばす
			if (!mapTip)continue;
			//if文で条件を分ける
			//プレイヤーの時
			if (mapTip->GetName() == "SickChar") {
				m_CoroKun = dynamic_cast<CoroKun*>(mapTip);
				GameContext::Register<CoroKun>(m_CoroKun);
			}
			//床の時
			if (mapTip->GetName() == "floor")m_Floors.push_back(dynamic_cast<Floor*>(mapTip));
			//壁の時
			if (mapTip->GetName() == "wall")m_Walls.push_back(dynamic_cast<Wall*>(mapTip));
			//泡の時
			if (mapTip->GetName() == "babul")m_Babuls.push_back(dynamic_cast<Babul*>(mapTip));
			//反転台の時
			if (mapTip->GetName() == "rebound")m_Rebounds.push_back(dynamic_cast<Rebound*>(mapTip));
			//起爆スイッチの時
			if (mapTip->GetName().substr(0, 6) == "switch")m_Switches.push_back(dynamic_cast<HoleSwitch*>(mapTip));
			//落とし穴の時
			if (mapTip->GetName().substr(0, 4) == "hole")m_HoleFloors.push_back(dynamic_cast<HoleFloor*>(mapTip));
			//普通の敵の時
			if (mapTip->GetName() == "NormalEnemy")m_NormalEnemies.push_back(dynamic_cast<NormalEnemy*>(mapTip));
			//少し強い敵の時
			if (mapTip->GetName() == "SuperEnemy")m_SuperEnemies.push_back(dynamic_cast<SuperEnemy*>(mapTip));
			//とても強い敵の時
			if (mapTip->GetName() == "HyperEnemy")m_HyperEnemies.push_back(dynamic_cast<HyperEnemy*>(mapTip));
			//パネルの時
			if (mapTip->GetName() == "LeftJumpPanel" || mapTip->GetName() == "RightJumpPanel" ||
				mapTip->GetName() == "LeftPanel" || mapTip->GetName() == "RightPanel")
			{
				m_CheckPoints.push_back(dynamic_cast<CheckPoint*>(mapTip));
			}
		}
	}

	//ファイルパスを設定する
	std::wstring filePath = L"FileData/Stage" + 
		std::to_wstring(staticData->Get_IntData("ステージ")) + L"ExproInfo.csv";

	//ファイルを開く
	if (File->Open(filePath.c_str(),Stream::FILE::STREAM_IFSTREAM))
	{
		std::vector<std::vector<std::string>>LoadData = File->Load_Data();
		std::vector<float>ConvertData = HoleFloor::FileDataConvert(LoadData);
		//落とし穴の爆発するタイミングを設定する
		for (int i = 0; i < (int)m_HoleFloors.size();i++)
		{
			//HoleFloorのポインタを取得する
			HoleFloor* hole = m_HoleFloors[i];
			//爆発するタイミングを取得する
			float timing = ConvertData[i];
			//爆発するタイミングを設定する
			hole->SetExproTiming(timing);
		}
		File->Close();
	}
}


ITextureBase* Stage::DirectlyBlowMap(ITextureBase* target,MapTip* mapType)
{
	//調べる座標を取得する
	Vector3 enemyPos = target->GetPosition() + target->AskOrigin();

	//サイズを取得する
	Vector3 size = target->AskOrigin() * 2;

	//上下の座標を決める
	float left = target->GetPosition().x;
	float right = left + size.x;
	float top = enemyPos.y;
	float botom = top + size.y;

	//方向を取得する
	int direct = target->DirectX();

	//代入する座標を宣言する
	Vector3 pos = Vector3::Zero;

	//左移動の時(右向き)
	if (direct == 1)
	{
		//座標を原点(0,0)に合わせる
		pos = mapType->GetPosition();
	}
	//右移動の時(左向き)
	else if (direct == -1)
	{
		//座標を原点(MAPSIZE_X,0)に合わせる
		pos = mapType->GetPosition() + Vector2(MapTip::MAPTIP_SIZEX, 0.0f);
	}

	//AABBで判定を取る
	if (pos.x >= left && pos.x <= right)
	{
		if (pos.y >= top && pos.y <= botom)
		{
			return mapType;
		}
	}

	return nullptr;
}

void Stage::MoveMapActive(const bool& active)
{
	//プレイヤーの実行関数を設定する
	m_CoroKun->SetActive(active);

	//体当たりを取得する
	BodyBlow* body = GameContext::Get<BodyBlow>();

	//体当たりの実行関数を設定する
	body->SetActive(active);

	//ウイルスビームを取得する
	VirusBeam* beam = GameContext::Get<VirusBeam>();

	//ウイルスビームの実行関数を設定する
	beam->SetActive(active);

	//ビームのパーティクルの実行関数を設定する
	for (Particle* patticle : beam->GetAllParticleList())
	{
		//実行関数を設定する
		patticle->SetActive(active);
	}

	//ニードルインクジェクションを取得する
	NeedleOverall* needleOver = GameContext::Get<NeedleOverall>();

	//針を取得する
	for (Needle* needle : needleOver->GetNeedleList())
	{
		//実行関数を設定する
		needle->SetActive(active);
	}

	//リザレクションを取得する
	Resurrection* resurrection = GameContext::Get<Resurrection>();

	//針を取得する
	for (Needle* needle : resurrection->GetNeedleList())
	{
		//実行関数を設定する
		needle->SetActive(active);
	}

	//ちょっと強い敵の実行関数を設定する
	for (SuperEnemy* enemy : m_SuperEnemies)
	{
		//実行関数を設定する
		enemy->SetActive(active);

		//固定砲台を取得する
		for (std::unique_ptr<FixedTurret>& turret : enemy->GetTurretList())
		{
			//泡を取得する
			for (SoapBabble* babble : turret->GetBabbleList())
			{
				//実行関数を設定する
				babble->SetActive(active);
			}
		}
	}

	//強い敵の実行関数を設定する
	for (HyperEnemy* enemy : m_HyperEnemies)
	{
		//実行関数を設定する
		enemy->SetActive(active);

		//泡を取得する
		for (SoapBabble* babble : enemy->GetBabbleList())
		{
			//実行関数を設定する
			babble->SetActive(active);
		}
	}
}
