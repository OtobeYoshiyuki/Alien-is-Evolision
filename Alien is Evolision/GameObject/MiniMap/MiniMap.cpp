#include "pch.h"
#include "MiniMap.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Stage/Stage.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameObject/Player/CoroKun.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

MiniMap::MiniMap():m_world(Matrix::Identity)
{
}

void MiniMap::Initialize(std::vector<std::vector<std::string>>& before, std::vector<std::vector<int>>& after)
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//使うテクスチャを取得する
	ITextureBase* useTexture = resourceManager->GetTexture("WhiteBack");

	//TextureManagerを取得する
	TextureManager* texManager = GameContext::Get<PlayScene>()->GetMiniMapManager().get();

	//ステージ情報を取得する
	Stage* stage = GameContext::Get<Stage>();

	//初期値を設定する
	Vector3 firstPos = Vector3(820, 60, 0);
	RECT firstRect = RECT{ 0,0,4,4};

	m_miniMapTex.resize(stage->GetMap().size());

	//プレイヤーのインスタンスを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	for (int i = 0; i < (int)after.size(); i++)
	{
		for (int j = 0; j < (int)after[i].size(); j++)
		{
			//データが空の場合は、処理を飛ばす
			if (after[i][j] == (int)MapTip::MAP_TYPE::NONE)continue;

			//ミニマップのテクスチャを生成する
			std::unique_ptr<ITextureBase> pmap = std::make_unique<TextureBase>(useTexture);

			//座標を設定する
			pmap->SetPosition(Vector3(10 * j, 10 * i, 0) + firstPos);

			//色を設定する
			Color color = Colors::White;

			//切り取り位置を設定する
			RECT rect = firstRect;

			//処理を分岐させる
			switch (MapTip::MAP_TYPE(after[i][j]))
			{
				//動くオブジェクトを追加する
			case MapTip::MAP_TYPE::WALL:
				//色を変更する
				color = Colors::Black;
				break;
			case MapTip::MAP_TYPE::BABUL:
				//色を変更する
				color = Colors::Cyan;
				break;
			case MapTip::MAP_TYPE::REBOUND:
				//色を変更する
				color = Colors::Coral;
				break;
			case MapTip::MAP_TYPE::GOAL:
				//色を変更する
				color = Colors::Lime;
				break;
			case MapTip::MAP_TYPE::PLAYER:
				m_moveTex.push_back(pmap.get());
				//プレイヤーの色を変更する
				color = Colors::Yellow;
				//切り取り位置を変更する
				rect = RECT{ 0,0,10,10 };
				//名前を変更する
				pmap->SetName("SickChar");
				break;
			case MapTip::MAP_TYPE::ENEMYN:
				m_moveTex.push_back(pmap.get());
				//色を変更する
				color = Colors::Green;
				//切り取り位置を変更する
				rect = RECT{ 0,0,8,8 };
				//名前を変更する
				pmap->SetName("NormalEnemy");
				break;
			case MapTip::MAP_TYPE::ENEMYS:
				m_moveTex.push_back(pmap.get());
				//色を変更する
				color = Colors::Orange;
				//切り取り位置を変更する
				rect = RECT{ 0,0,8,8 };
				//名前を変更する
				pmap->SetName("SuperEnemy");
				break;
			case MapTip::MAP_TYPE::ENEMYH:
				m_moveTex.push_back(pmap.get());
				//色を変更する
				color = Colors::Blue;
				//切り取り位置を変更する
				rect = RECT{ 0,0,8,8 };
				//名前を変更する
				pmap->SetName("HyperEnemy");
				break;
			case MapTip::MAP_TYPE::LEFTJUMPPANEL:
			case MapTip::MAP_TYPE::RIGHTJUMPPANEL:
			case MapTip::MAP_TYPE::LEFTPANEL:
			case MapTip::MAP_TYPE::RIGHTPANEL:
				//色を変更する
				color = Colors::SlateBlue;
				break;
			}

			//複数あるものとして考えているので、Switchでは制御しない
			if ((MapTip::MAP_TYPE)after[i][j] >= MapTip::MAP_TYPE::HOLE)
			{
				//起爆スイッチの設定をする(起爆スイッチは必ず偶数)
				if (after[i][j] % 2 == 0)
				{
					//色を変更する
					color = Colors::Red;
				}
				//落とし穴の設定をする(落とし穴は必ず奇数)
				else
				{
					//色を変更する
					color = Colors::DimGray;
				}
			}
			
			pmap->SetColor(color);
			pmap->SetRect(rect);

			//配列に生ポインタを登録する
			m_miniMapTex[i].push_back(pmap.get());
			texManager->Add(std::move(pmap));
		}
	}
}

void MiniMap::Update()
{
	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	for (int i = 0;i < (int)m_moveTex.size();i++)
	{
		//ゲームデータを取得する
		ITextureBase* gameData = m_gameTex[i];

		//動くミニマップのデータを取得する
		ITextureBase* moveMap = m_moveTex[i];

		//座標を取得する
		Vector3 pos = gameData->GetPosition();

		//ゲーム画面とミニマップの距離Xを求める
		float x = pos.x - 225.0f;
		x *= 0.25f;

		//画像の拡大倍率を取得する
		Vector3 scale = gameData->GetScale();

		if (scale.x <= 1.0f)scale.x = 1.0f;
		if (scale.y <= 1.0f)scale.y = 1.0f;
		if (scale.z <= 1.0f)scale.y = 1.0f;

		//画像の拡大倍率を設定する
		moveMap->SetScale(scale);

		//座標の補正を行う
		moveMap->SetPosition(Vector3(820 + x,pos.y * 0.27f,0.0f));
	}

	//ワールド行列を設定する
	//カメラの行列をもとに、ゲーム画面とミニマップの比率をかける
	Vector3 trans = nowCamera->GetMatrix().Translation();
	trans *= 0.25f;

	m_world = Matrix::CreateScale(Vector3(1.0f, 0.7f, 1.0f));

	//限界値よりはスクロールしない
	if (trans.x > -530.0f)
	{
		m_world *= Matrix::CreateTranslation(Vector3(trans.x - 15.0f, 12.5f, 0.0f));
	}
	else
	{
		m_world *= Matrix::CreateTranslation(Vector3(-530.0f, 12.5f, 0.0f));
	}

	//RECTアニメーションをミニマップに適応する
	for (int i = 0; i < (int)m_miniAnimeTex.size(); i++)
	{
		//ミニマップのテクスチャを取得する
		ITextureBase* miniMapTex = m_miniAnimeTex[i];

		//ゲーム用のテクスチャを取得する
		ITextureBase* gameTex = m_gameAnimeTex[i];

		//ゲーム用の原点を取得する
		Vector2 origin = gameTex->GetOrigin();

		//原点を補正する
		origin *= 0.25f;

		//原点を補正する
		miniMapTex->SetOrigin(origin);

		//座標を下にずらす
		miniMapTex->SetPosition(miniMapTex->GetPosition() + Vector3(0.0f, 10.0f, 0.0f));

		//SpriteEffectをゲームデータと連動させる
		miniMapTex->SetSpriteEffect(gameTex->GetSpriteEffect());

		//ゲーム用のRECTを取得する
		RECT gameRect = gameTex->GetRect();

		//ミニマップ用のRECTを取得する
		RECT miniRect = miniMapTex->GetRect();

		//ゲーム用のRECTをミニマップとゲーム画面の比率に直す
		gameRect.right *= 0.25f;

		//補正した値をミニマップのRECTに渡す
		miniRect.right = gameRect.right;

		//RECTをゲームデータと連動させる
		miniMapTex->SetRect(miniRect);
	}

	//ステージのインスタンスを取得
	Stage* stage = GameContext::Get<Stage>();

	//マップチップの配列を取得する
	std::vector<std::vector<ITextureBase*>> mapArray = stage->GetMap();

	//ステートに変更があった場合は、変更する
	for (int i = 0; i < (int)m_miniMapTex.size(); i++)
	{
		for (int j = 0; j < (int)m_miniMapTex[i].size(); j++)
		{
			//ミニマップのテクスチャを取得する
			ITextureBase* miniMap = m_miniMapTex[i][j];

			//マップチップのテクスチャを取得する
			ITextureBase* maptip = mapArray[i][j];

			//ステートは、ゲームデータに合わせる
			miniMap->SetState(maptip->GetState());
		}
	}
}

void MiniMap::Render()
{
}

void MiniMap::ArraySetUp()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//使うテクスチャを取得する
	ITextureBase* useTexture = resourceManager->GetTexture("WhiteBack");

	//ステージのインスタンスを取得する
	Stage* stage = GameContext::Get<Stage>();

	//マップチップの配列を取得する
	std::vector<std::vector<ITextureBase*>>mapArray = stage->GetMap();

	//色情報を設定する（デフォルト）
	XMVECTORF32 color = Colors::White;

	//切り取り位置を設定する(デフォルト)
	RECT rect = RECT{ 0,0,2,2, };

	//配列の情報を設定する
	for (std::vector<ITextureBase*>mapVec:mapArray)
	{
		for (ITextureBase* map:mapVec)
		{
			//インスタンスを生成
			std::unique_ptr<ITextureBase>pchild = std::make_unique<TextureBase>(useTexture);

			//子供かどうか調べる
			if (this->CheckChild(map->GetName()))
			{
				//ミニマップ用の配列に追加する
				if (map->GetName() == "Hypersyabondama")
				{
					//色情報を設定する
					color = Colors::DeepSkyBlue;
				}
				else {
					//色情報を設定する
					color = Colors::Aqua;
				}

				//ミニマップのインスタンスを生成する
				this->CreateMiniMap(pchild, m_miniMapTex.size() - 1, color, map->GetName(), rect);
			}

			//プレイヤー関連のものかどうか
			if (this->CheckPlayers(map->GetName()))
			{
				//衝突の場合
				if (map->GetName() == "衝突")
				{
					//色情報を設定する
					color = Colors::DeepPink;

					//切り取り位置を設定する
					rect = RECT{ 0,0,10,10 };
				}

				//ウイルスビームの場合
				if (map->GetName() == "ウイルスビーム")
				{
					//色情報を設定する
					color = Colors::MediumVioletRed;

					//切り取り位置を設定する
					rect = RECT{ 0,0,5,5 };

					//RECTアニメーション用のミニマップの配列に追加する
					m_miniAnimeTex.push_back(pchild.get());

					//RECTアニメーション用のゲームデータの配列に追加する
					m_gameAnimeTex.push_back(map);
				}

				//針の場合
				if (map->GetName() == "shortNeedle" || map->GetName() == "longNeedle")
				{
					//色情報を設定する
					color = Colors::Black;

					//切り取り位置を設定する
					rect = RECT{ 0,0,2,5 };
				}

				//回復アイテムの場合
				if (map->GetName() == "回復" || map->GetName() == "回復1" || 
					map->GetName() == "回復2" || map->GetName() == "回復3")
				{
					//色情報を設定する
					color = Colors::Lime;

					//切り取り位置を設定する
					rect = RECT{ 0,0,4,4 };
				}

				//ミニマップのインスタンスを生成する
				this->CreateMiniMap(pchild, m_miniMapTex.size() - 1, color, map->GetName(), rect);
			}

			//動くものかどうか調べる
			if (this->CheckArray(map->GetName()))
			{
				//ステージのゲームデータ用の配列に追加する
				m_gameTex.push_back(map);
			}
		}
	}
}

const bool MiniMap::CheckArray(const std::string& target)
{
	//チェックするリストの中身
	const std::vector<std::string> ARRAY_LIST =
	{ "SickChar","NormalEnemy","SuperEnemy","HyperEnemy","Hypersyabondama","Supersyabondama",
		"衝突","ウイルスビーム","中心の粒子","周りの粒子","shortNeedle","longNeedle","回復","回復1","回復2","回復3"};

	for (const std::string& name : ARRAY_LIST)
	{
		//目的物が見つかった場合は、trueを返す
		if (target == name)return true;
	}
	return false;
}

const bool MiniMap::CheckChild(const std::string& child)
{
	//チェックするリストの中身
	const std::vector<std::string> ARRAY_LIST =
	{ "Hypersyabondama","Supersyabondama"};

	for (const std::string& name : ARRAY_LIST)
	{
		//目的物が見つかった場合は、trueを返す
		if (child == name)return true;
	}

	return false;
}

const bool MiniMap::CheckPlayers(const std::string& players)
{
	//チェックするリストの中身
	const std::vector<std::string> ARRAY_LIST =
	{ "衝突","ウイルスビーム","中心の粒子","周りの粒子","shortNeedle","longNeedle","回復","回復1","回復2","回復3" };

	for (const std::string& name : ARRAY_LIST)
	{
		//目的物が見つかった場合は、trueを返す
		if (players == name)return true;
	}

	return false;
}

void MiniMap::CreateMiniMap(std::unique_ptr<OtobeLib::ITextureBase>& pchild,const int& col,
	const XMVECTORF32& color, const std::string& name, const RECT& rect)
{
	//TextureManagerを取得する
	TextureManager* texManager = GameContext::Get<PlayScene>()->GetMiniMapManager().get();

	//描画は切る
	pchild->SetState(ITextureBase::STATE::NONE);

	//画像の幅を設定する
	pchild->SetRect(rect);

	//色を変更する
	pchild->SetColor(color);

	//名前を変更する
	pchild->SetName(name);

	//ポインタを登録する
	m_miniMapTex[col].push_back(pchild.get());

	//ポインタを登録する
	m_moveTex.push_back(pchild.get());

	//TextureManagerに追加する
	texManager->Add(std::move(pchild));
}
