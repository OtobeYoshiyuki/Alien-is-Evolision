#include "pch.h"
#include "PlayScene.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "GameObject/Player/CoroKun.h"
#include "GameObject/CollisionUI/CollisionUI.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "GameObject/GameCamera/GameCamera.h"
#include "GameObject/TargetScrollCamera/TargetScrollCamera.h"
#include "GameObject/Item/Drop/ItemDrop.h"
#include "Game.h"
#include "ExternalLib/Easing/Easing.h"
#include "GameObject/Stage/Maptip/HoleFloor/HoleFloor.h"
#include "GameObject/Stage/Maptip/HoleSwitch/HoleSwitch.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace OtobeLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace ExternalLib;

PlayScene::PlayScene():
	m_GameManager(nullptr),//画像の管理クラス
	m_MainInfoManager(nullptr),//画像の管理クラス
	m_SubInfoManager(nullptr),//画像の管理クラス
	m_MiniMapManager(nullptr),//画像の管理クラス
	m_cameraManager(nullptr),//カメラの管理クラス
	m_BG(nullptr),//背景
	m_MiniMap(nullptr),//ミニマップ
	m_Stage(nullptr),//ステージ
	m_TimerJudge(nullptr),//時間の管理
	m_StarManager(nullptr),//星のエフェクトの管理クラス
	m_LagStart(nullptr),//スタートのエフェクト
	m_LagGo(nullptr),//Goのエフェクト
	m_LagFinish(nullptr),//終了のエフェクト
	m_LevelUp(nullptr),//レベルアップのエフェクト
	m_score(nullptr),//スコア
	m_healManager(nullptr),//回復アイテムの管理クラス
	m_easingTime(0.0f)//イージング関数の時間
{
}

void PlayScene::Initialize()
{
	//アニメーションのデータを読み込む
	//Fileの情報を取得する
	Stream* File = Stream::GetInstance();

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//必殺技のデータを追加する(デフォルト)
	staticData->Add("必殺技", (std::string)"デフォルト");

	//カットインのデータを追加する(デフォルト)
	staticData->Add("カットイン", (std::string)"デフォルト");

	//現在のページ数を追加する
	staticData->Add("ページ数", 1);

	//ファイルが開けたら開く
	if (File->Open(L"FileData/AnimationInfomation.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		std::vector<std::vector<std::string>> InputData = File->Load_Data();
		//データを変換する
		std::vector<AnimationImporter> ImportData = AnimationImporter::Cast(InputData);
		//アニメーションのデータを追加する
		Animation::Add(ImportData);
		//ファイルを閉じる
		File->Close();
	}

	//ファイルが開けたら開く
	if (File->Open(L"FileData/ItemDrop.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		std::vector<std::vector<std::string>> InputData = File->Load_Data();
		//データを変換する
		std::vector<ItemDrop> ImportData = ItemDrop::Cast(InputData);
		//アニメーションのデータを追加する
		ItemDrop::AddItem(ImportData);
		//ファイルを閉じる
		File->Close();
	}

	//ファイルが開けたら開く
	if (File->Open(L"FileData/StatusInfo.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		std::vector<std::vector<std::string>> InputData = File->Load_Data();
		//データを変換する
		std::vector<StatusImporter> ImportData = StatusImporter::Cast(InputData);
		//ステータスのデータを追加する
		StatusBase::Add(ImportData);
		//ファイルを閉じる
		File->Close();
	}

	//ファイルのパスを設定する
	std::wstring enemyFile = L"FileData/EnemyInfo" + std::to_wstring(staticData->Get_IntData("ステージ")) + L".csv";

	//ファイルが開けたら開く
	if (File->Open(enemyFile.c_str(),Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		std::vector<std::vector<std::string>> LoadData = File->Load_Data();

		for (int i = 0; i < (int)LoadData.size(); i++)
		{
			//データを追加する
			staticData->Add(LoadData[i][0],std::stoi(LoadData[i][2]));
		}

		//ファイルを閉じる
		File->Close();
	}

	//ファイルが開けたら開く
	if (File->Open(L"Save.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		m_saveData = File->Load_Data();

		//ファイルを閉じる
		File->Close();
	}

	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//フェードインを行う
	state->SetFadeState(SceneState::FADEIN);

	//フェードの加算速度を設定する
	state->SetFadeSpeed(SceneState::ALFA_SPEED);

	//生成の処理
	this->Create();

	//BGMを再生する
	soundManager->Play(CRI_CUESHEET_0__03_プレイ用BGM_);
}

void PlayScene::Update()
{
	//配列のサポートクラスを取得する
	Array* arraySupport = Array::GetInstance();

	//ファイルを取得する
	Stream* file = Stream::GetInstance();

	//キーの状態の取得
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//現在のプレイヤーのステータスを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//カメラの更新
	m_cameraManager->Update();

	//ミニマップの更新
	m_MiniMap->Update();

	//フェードアウト終了後
	if (state->GetFade() == SceneState::FADERESULT)
	{
		//リザルトのステートだった場合
		if (this->GetPreState() == BACKOUT::RESULT)
		{
			//リザルトに遷移する
			Manager->ChangeScene("Result");

			//取得したアイテムの情報を記録する
			std::vector<std::string> itemInfo =
				arraySupport->CreateSingleArray<std::string>((int)m_healManager->GetItem().size(), [&](int i)
					{
						return m_healManager->GetItem()[i]->GetName();
					});

			std::vector<std::string> statusInfo =
				arraySupport->CreateSingleArray<std::string>((int)PLAYER_STATUS::MAX, [&](int i)
					{
						switch ((PLAYER_STATUS)i)
						{
						case PLAYER_STATUS::LEVEL:
							return std::to_string(player->GetLevel());
							break;
						case PLAYER_STATUS::HP:
							return std::to_string(player->GetMaxHP());
							break; 
						case PLAYER_STATUS::MP:
							return std::to_string(player->GetMP());
							break;
						case PLAYER_STATUS::ATACK:
							return std::to_string(player->GetAtack());
							break;
						case PLAYER_STATUS::DEFENCE:
							return std::to_string(player->GetDefence());
							break; 
						case PLAYER_STATUS::SPECIAL:
							return std::to_string(player->GetSpecial());
							break;
						case PLAYER_STATUS::NEXT:
							return std::to_string(player->GetNextLevelPoint());
							break;
						}
					});

			//アイテムを1つ以上取得していたら
			if ((int)itemInfo.size() > NULL)
			{
				//セーブデータを上書きする
				m_saveData[(int)SAVE_DATA::HEAL_INFO] = itemInfo;
			}

			//セーブデータを上書きする
			m_saveData[(int)SAVE_DATA::PLAYER_STATUS] = statusInfo;

			//ファイルを開く
			if (file->Open(L"Save.csv", Stream::FILE::STREAM_OFSTREAM))
			{
				//Csv形式に変換する
				file->Data_Input(m_saveData);

				//ファイルを閉じる
				file->Close();
			}

			//スコアが加算中の時は、反映する
			if (m_score->GetSaving() != NULL)
			{
				//新しくスコアを設定する
				m_score->SetScore(m_score->GetScore() + m_score->GetSaving());

				//貯蓄を0にする
				m_score->SetSaving(NULL);
			}

			float time = m_TimerJudge->GetLimitTimer();

			//残りタイムを保存する
			staticData->Add("のこりタイム", (int)time);

			time = 300 - time;

			//クリアタイムを保存する
			staticData->Add("クリアタイム",(int)time);

			//レベルを保存する
			staticData->Add("レベル", statusInfo[(int)PLAYER_STATUS::LEVEL]);

			//スコアを保存する
			staticData->Add("スコア", (int)m_score->GetScore());

		}

		//タイトルのステートだった場合
		else if(this->GetPreState() == BACKOUT::TITLE ||
				player->GetNowData2() == PLAYERMOVE::ANIMEFINISH)
		{
			Manager->ChangeScene("Title");
		}
	}

	//フェードイン中は、何もしない
	if (state->GetFade() != SceneState::WAITKEY && 
		state->GetFade() != SceneState::FADERESULT)return;

	//Xキーが押されたら、メニューへ移行する
	if (keyTrakker.IsKeyPressed(Keyboard::X))
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__39_メニューを開く);

		Manager->PushScene("Menu");
		return;
	}

	//Espキーが押されたら、ポーズへ移行する
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__39_メニューを開く);

		Manager->PushScene("Poce");
		return;
	}

	//テクスチャの更新
	m_GameManager->Update();
	m_MainInfoManager->Update();
	m_SubInfoManager->Update();
	m_MiniMapManager->Update();
	m_healManager->Update();

	//ステージの更新
	m_Stage->Update();

	//TimeLagの更新
	m_LagStart->Update();
	m_LagGo->Update();
	m_LagFinish->Update();

	//LagEffectの更新
	this->UpdateLag();

	//StarManagerの更新
	m_StarManager->Update();

	//当たり判定の更新
	m_CollisionManager->Update();

	//タイマーの更新
	m_TimerJudge->Update();

	//スコアの更新
	m_score->Update();
}

void PlayScene::Render()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//フォントの状態を取得する
	TextureFont* Font = TextureFont::GetInstance();

	//プレイヤーのポインタを取得
	CoroKun* player = GameContext::Get<CoroKun>();

	//描画系の状態を取得する
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); },
		m_cameraManager->GetNowCamera()->GetMatrix());

	//画像の描画処理
	m_GameManager->Render();

	Graph->GetSpriteBatch()->End();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//画像の描画処理
	m_MainInfoManager->Render();

	////フレームレートを描画する
	//int fps = staticData->Get_IntData("FPS");

	//Font->DrawFormatString("FPS  " + std::to_string(fps), Vector2(1000.0f, 50.0f));

	Graph->GetSpriteBatch()->End();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); },
		m_MiniMap->GetWorld());

	//画像の描画処理
	m_MiniMapManager->Render();

	Graph->GetSpriteBatch()->End();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//画像の描画処理
	m_SubInfoManager->Render();

	//HPを描画する(フォント)
	std::string playerHpString = std::to_string(player->GetHP()) + "/" + std::to_string(player->GetMaxHP());
	Font->DrawFormatString("レべル" + std::to_string(player->GetLevel()), Vector2(210, 10), 1.0f, Colors::Black);
	Font->DrawFormatString(playerHpString, Vector2(500.0f, 10.0f), 1.0f, Colors::Black);

	//MPを描画する(フォント)
	std::string playerMpString = std::to_string(player->GetMP());
	playerMpString = Font->Convert_NumberOfDigits(playerMpString, 3, "0", 1);
	playerMpString += "/" + std::to_string(player->GetMaxMP());
	Font->DrawFormatString(playerMpString, Vector2(500.0f, 80.0f), 0.8f, Colors::Black);

	//スペシャルポイントを描画する(フォント)
	std::string playerSpString = std::to_string(player->GetSpecial());
	playerSpString = Font->Convert_NumberOfDigits(playerSpString, 3, "0", 1);
	playerSpString += "/" + std::to_string(player->GetMaxSpecialPoint());
	Font->DrawFormatString(playerSpString, Vector2(620.0f, 115.0f), 0.8f, Colors::Black);

	//制限時間を取得する
	std::string limitTimer = m_TimerJudge->AskTimeStringMinute() + "：" + m_TimerJudge->AskTimeStringSecound();

	//制限時間を描画する
	Font->DrawFormatString(limitTimer, Vector2(850.0f, 10.0f), 1.0f, Colors::Black);

	//スコアを描画する
	int score = (int)m_score->GetScore();
	std::string scoreStr = std::to_string(score);
	scoreStr = Font->Convert_NumberOfDigits(scoreStr, 7, "0", 1);
	Font->DrawFormatString(scoreStr, Vector2(1050.0f, 10.0f), 1.0f, Colors::White);

	//必殺技のUIの裏面を取得する
	FadeTexture* specialUI = player->GetSpecialUI();
	XMVECTORF32 specialColor = Colors::Black;
	specialColor.f[3] = specialUI->GetColor().A();

	//通常攻撃のUIを描画
	Font->DrawFormatString("Q：チャ－ジショット", Vector2(210.0f, 670.0f), 1.0f, Colors::Black);

	//必殺技のUIを描画
	Font->DrawFormatString("Z+V：SP", Vector2(575.0f, 670.0f), 1.0f, specialColor);

	//メニューのUIを描画
	Font->DrawFormatString("X：メニュ－", Vector2(820.0f, 670.0f), 1.0f, Colors::Black);

	//ポーズのUIを描画
	Font->DrawFormatString("ESC：ポ－ズ", Vector2(1025.0f, 670.0f), 1.0f, Colors::Black);

	//開始のフォントを描画する
	Font->DrawFormatString(m_LagStart->GetNowString(), Vector2(m_LagStart->GetPosition()), 3.0f, Colors::Aqua);

	Font->DrawFormatString(m_LagGo->GetNowString(), Vector2(m_LagGo->GetPosition()), 10.0f, Colors::White,64);

	Font->DrawFormatString(m_LagFinish->GetNowString(), Vector2(m_LagFinish->GetPosition()), 3.0f, Colors::Red);

	Graph->GetSpriteBatch()->End();
}

void PlayScene::Finalize()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//落とし穴のグループ化を解除する
	HoleFloor::ResetHoleGrop();

	//StaticDataの一部のデータを削除する
	for (HoleSwitch* swt : m_Stage->GetHoleSwitches())
	{
		//データが存在する場合
		if (staticData->Request_Data(swt->GetName() + std::to_string(swt->GetSerealNumber())))
		{
			//スイッチのシリアルナンバーを削除する
			staticData->IntRemove(swt->GetName() + std::to_string(swt->GetSerealNumber()));
		}
		
		//データが存在する場合
		if (staticData->Request_Data(swt->GetName() + std::to_string(swt->GetSerealNumber()) + "Count"))
		{
			//起爆カウントを削除する
			staticData->FloatRemove(swt->GetName() + std::to_string(swt->GetSerealNumber()) + "Count");
		}
	}

	//BGMを停止する
	soundManager->Stop(CRI_CUESHEET_0__03_プレイ用BGM_);
}

void PlayScene::Create()
{
	//Managerクラス生成
	this->CreateManager();

	//背景を生成
	this->CreateBack();

	//ステージを生成
	this->CreateStage();

	//カメラを生成
	this->CreateCamera();

	//タイマーを生成
	this->CreateTimer();

	//タイムラグエフェクトを生成
	this->CreateEffect();

	//スコアを生成
	this->CreateScore();
}

//指定したデータを配列にして返す(std::vector<>)
const std::vector<std::string> PlayScene::GetSaveDataArray(const int& data)
{
	//変数を初期化する
	std::vector<std::string> dataArray;

	//指定したデータを取得する
	for (std::string& info : m_saveData[data])
	{
		//データが空の時は、処理を飛ばす
		if (info.empty())continue;

		//データを追加する
		dataArray.push_back(info);
	}

	//指定したデータを返す
	return dataArray;
}

void PlayScene::CreateManager()
{
	//GameContextに登録する
	GameContext::Register<PlayScene>(this);

	//CollisionManagerクラス生成
	m_CollisionManager = std::make_unique<CollisionManager>();
	GameContext::Register<CollisionManager>(m_CollisionManager.get());

	//TextureManagerクラス生成
	m_GameManager = std::make_unique<TextureManager>();
	m_MainInfoManager = std::make_unique<TextureManager>();
	m_SubInfoManager = std::make_unique<TextureManager>();
	m_MiniMapManager = std::make_unique<TextureManager>();
	m_healManager = std::make_unique<HealManager>();

	//StarManagerクラス生成
	m_StarManager = std::make_unique<StarManager>();
	GameContext::Register<StarManager>(m_StarManager.get());
}

void PlayScene::CreateBack()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* BG = resourceManager->GetTexture("ステージ" + staticData->Get_StringData("ステージ"));
	ITextureBase* Comand = resourceManager->GetTexture("コマンド");
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");

	//背景を生成する
	std::unique_ptr<BackGround> pBG = std::make_unique<BackGround>(BG);
	m_BG = pBG.get();

	//プレイヤーコマンドを生成する
	//std::unique_ptr<ITextureBase> pComand = std::make_unique<TextureBase>(Comand);

	//ミニマップを生成する
	m_MiniMap = std::make_unique<MiniMap>();
	GameContext::Register<MiniMap>(m_MiniMap.get());

	//ミニマップ用の背景を生成する
	std::unique_ptr<ITextureBase> pMiniBack = std::make_unique<TextureBase>(whiteBack);
	pMiniBack->SetRect(RECT{ 0,0,480,95 });
	pMiniBack->SetPosition(Vector3(800.0f, 50.0f, 0.0f));
	pMiniBack->SetColor(Colors::Purple);

	//タイマーのUIを生成する
	std::unique_ptr<TextureBase>pTimerUI = std::make_unique<TextureBase>(whiteBack);
	pTimerUI->SetRect(RECT{ 0,0,240,50 });
	pTimerUI->SetPosition(Vector3(800.0f, 0.0f, 0.0f));
	pTimerUI->SetColor(Colors::Yellow);

	//スコアのUIを生成する
	std::unique_ptr<TextureBase>pScoreUI = std::make_unique<TextureBase>(whiteBack);
	pScoreUI->SetRect(RECT{ 0,0,240,50 });
	pScoreUI->SetPosition(Vector3(1040.0f, 0.0f, 0.0f));
	pScoreUI->SetColor(Colors::Magenta);

	//経験値ゲージの裏面を生成する
	std::unique_ptr<ITextureBase>pExperienceBack = std::make_unique<TextureBase>(whiteBack);
	pExperienceBack->SetRect(RECT{ 0,0,600,25 });
	pExperienceBack->SetPosition(Vector3(200.0f, 50.0f, 0.0f));
	pExperienceBack->SetColor(Colors::Gray);

	//コマンド技ゲージの裏面を作成する
	std::unique_ptr<ITextureBase>pTechnique = std::make_unique<TextureBase>(whiteBack);
	pTechnique->SetRect(RECT{ 0,0,500,40 });
	pTechnique->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	pTechnique->SetColor(Colors::SteelBlue);
	pTechnique->SetDrawOrder(3);
		
	//必殺技ゲージの裏面を作成する
	std::unique_ptr<ITextureBase>pDeathBlow = std::make_unique<TextureBase>(whiteBack);
	pDeathBlow->SetRect(RECT{ 0,0,600,70 });
	pDeathBlow->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	pDeathBlow->SetColor(Colors::HotPink);
	pDeathBlow->SetDrawOrder(4);

	//ゲージとミニマップの裏面を生成する
	std::unique_ptr<CollisionUI> pTopBackUI = std::make_unique<CollisionUI>(whiteBack);
	pTopBackUI->SetRect(RECT{ 0,0,4240,141 });
	pTopBackUI->SetPosition(Vector3(200.0f, 0.0f, 0.0f));
	pTopBackUI->SetDrawOrder(4);
	pTopBackUI->Initialize();

	std::unique_ptr<CollisionUI> pBotomBackUI = std::make_unique<CollisionUI>(whiteBack);
	pBotomBackUI->SetRect(RECT{ 0,0,4240,141 });
	pBotomBackUI->SetPosition(Vector3(200.0f, 661.0f, 0.0f));
	pBotomBackUI->SetDrawOrder(6);
	pBotomBackUI->Initialize();

	//限界地点を生成する
	std::unique_ptr<CollisionUI> pLimitUI = std::make_unique<CollisionUI>(whiteBack);
	pLimitUI->SetRect(RECT{ 0,0,200,720 });
	pLimitUI->SetPosition(Vector3(4240.0f, 0.0f, 0.0f));
	pLimitUI->SetDrawOrder(4);
	pLimitUI->Initialize();

	//コマンドの裏面を生成する
	std::unique_ptr<CollisionUI> pCommandBackUI = std::make_unique<CollisionUI>(whiteBack);
	pCommandBackUI->SetRect(RECT{ 0,0,200,720 });
	pCommandBackUI->SetDrawOrder(2);
	pCommandBackUI->SetColor(Colors::DarkCyan);
	pCommandBackUI->Initialize();

	//GameManagerに追加する
	m_GameManager->Add(std::move(pBG));
	m_GameManager->Add(std::move(pLimitUI));

	//MainInfoManagerに追加する
	m_MainInfoManager->Add(std::move(pTimerUI));
	m_MainInfoManager->Add(std::move(pScoreUI));
	m_MainInfoManager->Add(std::move(pMiniBack));
	m_MainInfoManager->Add(std::move(pTopBackUI));
	m_MainInfoManager->Add(std::move(pBotomBackUI));

	//SubInfoManagerに追加する
	m_SubInfoManager->Add(std::move(pCommandBackUI));
	//m_SubInfoManager->Add(std::move(pComand));
	m_SubInfoManager->Add(std::move(pExperienceBack));
	m_SubInfoManager->Add(std::move(pDeathBlow));
	m_SubInfoManager->Add(std::move(pTechnique));
}

void PlayScene::CreateStage()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* mapTip = resourceManager->GetTexture("マップチップ");

	//ステージを生成する
	m_Stage = std::make_unique<Stage>();

	//ステージクラスを登録する
	GameContext::Register<Stage>(m_Stage.get());

	//指定したステージデータを読み込む
	m_Stage->Initialize(L"FileData/Stage" + std::to_wstring(staticData->Get_IntData("ステージ")) + L"main.csv",
		L"FileData/Stage" + std::to_wstring(staticData->Get_IntData("ステージ")) + L"sub.csv");
}

void PlayScene::CreateCamera()
{
	//カメラを生成する
	m_cameraManager = std::make_unique<CameraManager>();

	//関数を登録する
	m_cameraManager->AddCamera("ゲームカメラ", std::make_unique<GameCamera>);
	m_cameraManager->AddCamera("スクロールカメラ", std::make_unique<TargetScrollCamera>);

	//カメラを設定する
	m_cameraManager->ChangeCamera("ゲームカメラ");

	//カメラのインスタンスを生成しておく
	m_cameraManager->Update();

	//カメラを登録する
	GameContext::Register<CameraManager>(m_cameraManager.get());

	//ミニマップの初期化を行う
	//サブデータを読み込む
	m_MiniMap->Initialize(m_Stage->GetSubBeforeData(), m_Stage->GetSubAfterData());

	//メインデータを読み込む
	m_MiniMap->Initialize(m_Stage->GetMainBeforeData(), m_Stage->GetMainAfterData());

	//回復アイテムの初期化
	m_healManager->Initialize();

	//配列情報を設定する
	m_MiniMap->ArraySetUp();

	//行列を更新しておく
	m_MiniMap->Update();
}

void PlayScene::CreateTimer()
{
	//タイマーを生成する
	m_TimerJudge = std::make_unique<TimerJudge>();

	//タイマーの初期化を行う
	m_TimerJudge->Initialize(300, false);

	//タイマーを登録する
	GameContext::Register<TimerJudge>(m_TimerJudge.get());
}

void PlayScene::CreateEffect()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//LagStartを生成する
	m_LagStart = std::make_unique<TimeLag>();
	//LagStartの初期化を行う
	m_LagStart->Initialize(true, "", Vector3(400.0f, Game::MAX_HEIGHT / 2, 0.0f),
		Vector3::Zero);

	//描画の開始時間を設定する
	m_LagStart->SetInTime("", 30);
	m_LagStart->SetInTime("R", 60);
	m_LagStart->SetInTime("RE", 90);
	m_LagStart->SetInTime("REA", 120);
	m_LagStart->SetInTime("READ", 150);
	m_LagStart->SetInTime("READY", 180);
	m_LagStart->SetInTime("READY！", 210);

	//次に表示する文字を設定する
	m_LagStart->SetNextString("", "R");
	m_LagStart->SetNextString("R", "RE");
	m_LagStart->SetNextString("RE", "REA");
	m_LagStart->SetNextString("REA", "READ");
	m_LagStart->SetNextString("READ", "READY");
	m_LagStart->SetNextString("READY", "READY！");

	//描画の終了時間を設定する
	m_LagStart->SetOutTime(360);

	//ジャンプの際のジャンプ回数と、オプションを開放しておく
	m_LagStart->GetJumpState()->SetExecute(true);
	m_LagStart->GetJumpState()->SetJumpLimit(120.0f);
	m_LagStart->GetJumpState()->SetJumpSpeed(10.0f);
	m_LagStart->GetJumpState()->SetJumpLimitCount(2);
	m_LagStart->GetJumpState()->SetJumpOption(true);

	//1フレームに移動する速度を設定する
	m_LagStart->GetAutoMoveState()->SetExecute(true);
	m_LagStart->GetAutoMoveState()->SetSpeed(Vector3(4.0f, 4.0f, 0.0f));

	//LagGoを生成する
	m_LagGo = std::make_unique<TimeLag>();
	//LagGoの初期化を行う
	m_LagGo->Initialize(false,"", Vector3(350,250,0),Vector3::Zero);

	//描画の開始時間を設定する
	m_LagGo->SetInTime("", 0);

	//次に表示する文字を設定する
	m_LagGo->SetNextString("", "GO");

	//描画の終了時間を設定する
	m_LagGo->SetOutTime(80);

	//LagFinishを生成する
	m_LagFinish = std::make_unique<TimeLag>();
	//LagFinishの初期化を行う
	m_LagFinish->Initialize(false, "",Vector3(-500.0f, 300.0f, 0.0f), Vector3::Zero);

	//描画の開始時間を設定する
	m_LagFinish->SetInTime("", 0);
	m_LagFinish->SetInTime("1", 0);

	//次に表示する文字を設定する
	m_LagFinish->SetNextString("", "STAGE" + std::to_string(staticData->Get_IntData("ステージ")) + "CLEAR");
	m_LagFinish->SetNextString("1", "TimeUp Miss");

	//描画の終了時間を設定する
	m_LagFinish->SetOutTime(300);

	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//レベルアップのテクスチャを取得する
	ITextureBase* useTexture = resourceManager->GetTexture("レベルアップ1");

	//レベルアップエフェクトを生成する
	std::unique_ptr<LevelUp> plevelUp = std::make_unique<LevelUp>(useTexture);
	m_LevelUp = plevelUp.get();
	m_LevelUp->SetState(ITextureBase::STATE::NONE);
	m_LevelUp->Initialize();

	//GameContextに登録する
	GameContext::Register<LevelUp>(m_LevelUp);

	//SubInfoManagerに追加する
	m_SubInfoManager->Add(std::move(plevelUp));
}

void PlayScene::CreateScore()
{
	//スコアを生成する
	m_score = std::make_unique<Score>();

	//貯蓄とスコアを設定する
	m_score->SetScore(0.0f);
	m_score->SetSaving(0.0f);
	m_score->SetAdd(1);
}

void PlayScene::UpdateLag()
{
	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//前回のフレームからの経過時間
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//LagEffectの更新

	if (!m_LagStart->GetCustomFlag() && m_LagStart->GetFrameCount() > 30)
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__27_READY);

		//カスタムフラグをtrueにする
		m_LagStart->SetCustomFlag(true);
	}

	if (m_LagStart->GetAllStringFlag())
	{
		if (m_LagStart->GetAutoMoveState()->GetExecute())
		{
			switch (m_LagStart->GetAutoMoveState()->GetNowState())
			{
			case DIRECT::NNONE:
				//左に移動させる
				m_LagStart->GetAutoMoveState()->SetNowState(DIRECT::LEFT);
				break;
			case DIRECT::LEFT:
				//座標がステージの左端を超えようとしたら、ステートを変更する
				if (m_LagStart->GetPosition().x <= 200.0f)
				{
					//右に移動させる
					m_LagStart->GetAutoMoveState()->SetNowState(DIRECT::RIGHT);
					//ジャンプさせる
					m_LagStart->GetJumpState()->SetNowState(JUMP::START);
				}
				break;
			}
		}

		//2回ジャンプして、着地したら
		if (m_LagStart->GetJumpState()->GetJumpmDesignation())
		{
			if (m_LagStart->GetAutoMoveState()->GetNowState() == DIRECT::RIGHT)
			{
				//実行関数を落とす
				m_LagStart->GetAutoMoveState()->SetExecute(false);
				//移動を停止させる
				m_LagStart->GetAutoMoveState()->SetNowState(DIRECT::NNONE);
				//星の初期化処理を行う
				m_StarManager->Initialize(m_LagStart->GetPosition());
			}
		}
	}

	//LagGoの更新

	//赤い星のジャンプが終わって、描画が切られたとき
	if (m_StarManager->GetPopStarRed()->GetJumpState()->GetJumpmDesignation() &&
		m_StarManager->GetPopStarRed()->GetState() == ITextureBase::STATE::NONE)
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__28_GO);

		//更新フラグをtrueにする
		m_LagGo->SetFrameFlag(true);

		//ジャンプ終了フラグを元に戻す
		m_StarManager->GetPopStarRed()->GetJumpState()->SetJumpmDesignation(false);
	}

	//LagGoの演出が終了したら、タイマーを更新する
	if (m_LagGo->GetFinishFlag())
	{
		if (!m_LagGo->GetCustomFlag())
		{
			//タイマーを動かす
			m_TimerJudge->SetTimerUp(true);

			//カスタムフラグをtrueにする
			m_LagGo->SetCustomFlag(true);
		}
	}

	Vector3 vel = Vector3::Zero;

	//LagFinishの更新
	switch (this->GetNowState())
	{
	case BACKOUT::ADMISSON:
		//経過時間を更新する
		m_easingTime += elapsedTime;

		//UIのアニメーションを設定する(Easing関数を使用する)
		vel.x += -Easing::OutBack(m_easingTime, CoroKun::EASING_TOTALTIME,6.0f, CoroKun::EASING_MAX, CoroKun::EASING_MIN);

		//時間が経過したらステートを切り替える
		if (m_easingTime > CoroKun::EASING_MAX)
		{
			this->SetNowState(BACKOUT::EXIT);
			m_easingTime = CoroKun::EASING_MIN;
		}
		break;
	case BACKOUT::EXIT:
		//経過時間を更新する
		m_easingTime += elapsedTime;

		//UIのアニメーションを設定する(Easing関数を使用する)
		vel.x += Easing::InBack(m_easingTime, CoroKun::EASING_TOTALTIME,0.0f, CoroKun::EASING_MAX, CoroKun::EASING_MIN) * 100.0f;

		//時間が経過したらステートを切り替える
		if (m_easingTime > CoroKun::EASING_MAX)
		{
			//制限時間が残っているとき
			if (!m_TimerJudge->GetTimerJudge())
			{
				this->SetNowState(BACKOUT::RESULT);
			}
			else {
				//制限時間が残っていないとき
				this->SetNowState(BACKOUT::TITLE);
			}
			m_easingTime = CoroKun::EASING_MIN;
		}
		break;
	case BACKOUT::TITLE:
	case BACKOUT::RESULT:
		//リザルトに遷移する
		state->SetFadeState(SceneState::FADEOUT);

		//ステートを切り替える
		this->SetNowState(BACKOUT::NONE);

		//フェードの加算速度を設定する
		state->SetFadeSpeed(SceneState::ALFA_SPEED);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__23_プレイフェードアウト);
		break;
	}

	m_LagFinish->SetVelocity(vel);
}

