#include "pch.h"
#include "RankingScene.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/StackScene/Scene/Result/ResultScene.h"
#include "OtobeLib/StackScene/Scene/StageSelect/StageSelectScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Player/CoroKun.h"
#include "ExternalLib/Easing/Easing.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;
using namespace ExternalLib;

//スクロール前の座標
const Vector3 RankingScene::BEFORE_POS = Vector3(1550.0f, 75.0f, 0.0f);

//スクロール後の座標
const Vector3 RankingScene::AFTER_POS = Vector3(-1300.0f, 75.0f, 0.0f);

RankingScene::RankingScene():
	m_TextureManager(nullptr),//画像の管理クラス
	m_rightArrow(nullptr),//右矢印の画像
	m_leftArrow(nullptr),//左矢印の画像
	m_escapeBack(nullptr),//Escapeキーの背面の画像
	m_direct(NULL),//ベクトルの向き
	m_time(0.0f),//イージング関数用の経過時間
	m_ifState(nullptr)//関数オブジェクト
{
	//容量を確保する
	m_rankingDatas.resize((int)ResultScene::STAGENUM);

	for (int i = 0; i < (int)ResultScene::STAGENUM; i++)
	{
		for (int j = 0; j < (int)ResultScene::RESULT_INFO::RANKING_NUM; j++)
		{
			//ランキングを生成する
			std::unique_ptr<Ranking> pranking = std::make_unique<Ranking>();

			//Moveする
			m_rankingDatas[i].push_back(std::move(pranking));
		}
	}

	//ファイルを取得する
	Stream* file = Stream::GetInstance();

	//ファイルを開く
	if (file->Open(L"RankingData.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		m_rankingData = file->Load_Data();

		for (int i = 0; i < (int)ResultScene::STAGENUM; i++)
		{
			//指定した範囲のデータを二次元配列で取得する
			//ロードしたデータから、一部だけデータを抜き出す
			std::vector<std::vector<std::string>> convertArray =
				this->GetSaveDataDoubleArray((int)ResultScene::RANKING_INFO::STAGE1 + 1 + (i * 10),
					ResultScene::CREAR_RESULT_RANGE, (int)ResultScene::RANKING_INFO::STAGE1 + 3 + (i * 10),
					ResultScene::STAGE_INTERVAL);
			
			for (int j = 0; j < (int)ResultScene::RESULT_INFO::RANKING_NUM; j++)
			{
				//パラメーターを設定する

				//レベルを設定する
				m_rankingDatas[i][j]->SetLevel(convertArray[j][0]);

				//スコアを設定する
				m_rankingDatas[i][j]->GetScore()->SetScore(std::stoi(convertArray[j][1]));

				//クリアタイムを設定する
				m_rankingDatas[i][j]->GetTimer()->SetLimitTimer(std::stoi(convertArray[j][2]));
			}
		}

		//ファイルを閉じる
		file->Close();
	}

	//ステージのクリア状況を読み込む
	//ファイルが開けたら、開く
	if (file->Open(L"FileData/StageClear.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//データを取得する
		m_clearInfo = file->Load_Data();

		//ファイルを閉じる
		file->Close();
	}
}

void RankingScene::Initialize()
{
	//生成処理
	this->Create();

	//添え字を設定する
	this->AddData1(NULL);
}

void RankingScene::Update()
{
	//SceneManagerの状態を取得
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//キーボードの状態
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//画像の更新処理
	m_TextureManager->Update();

	//Escapeキーが押されたら
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//現在のSceneを消す
		Manager->PopScene();

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
	}

	//イージング関数の更新処理
	this->UpdateEasing();
}

void RankingScene::Render()
{
	//描画系の情報を取得
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//フォントを取得
	TextureFont* font = TextureFont::GetInstance();

	//描画の設定
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//画像の描画処理
	m_TextureManager->Render();

	//フォントを描画する
	for (int i = 0; i < (int)ResultScene::STAGENUM; i++)
	{
		font->DrawFormatString(m_clearInfo[(int)StageSelectScene::STAGEINFO::STAGENAME][i],
			Vector2(m_rankingBack[i]->GetPosition().x + 300.0f, 100.0f), 2.0f, Colors::Red);

		for (int j = 0; j < (int)ResultScene::RESULT_INFO::RANKING_NUM; j++)
		{
			font->DrawFormatString("No." + std::to_string(j + 1) + "  レべル" + m_rankingDatas[i][j]->GetLevel(),
				Vector2(m_rankingBack[i]->GetPosition().x + 50.0f, 180.0f + float(j * 150)), 1.3f, Colors::Black);

			//スコアの値を編集する
			int score = (int)m_rankingDatas[i][j]->GetScore()->GetScore();
			std::string scoreStr = std::to_string(score);
			scoreStr = font->Convert_NumberOfDigits(scoreStr, 7, "0", 1);

			font->DrawFormatString("スコア " + scoreStr + " "
				"タイム " + m_rankingDatas[i][j]->GetTimer()->AskTimeStringMinute() + "：" + m_rankingDatas[i][j]->GetTimer()->AskTimeStringSecound(),
				Vector2(m_rankingBack[i]->GetPosition().x + 50.0f, 230.0f + float(j * 150)), 1.3f, Colors::Black);
		}
	}
	
	//Escapeキーの描画
	if (m_escapeBack->NowBlinking())
	{
		font->DrawFormatString("PUSH ESCAPE", Vector2(300.0f, 610.0f), 2.0f, Colors::Black);
	}
	
	//描画の終了
	Graph->GetSpriteBatch()->End();
}

void RankingScene::Finalize()
{
}

void RankingScene::Create()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを生成する
	m_TextureManager = std::make_unique<TextureManager>();

	//使用する画像を取得する
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");
	ITextureBase* arrow = resourceManager->GetTexture("矢印");

	//左の矢印を生成する
	std::unique_ptr<Arrow> pArrowLeft = std::make_unique<Arrow>(arrow);
	pArrowLeft->InitArrow(Vector3(0.5f), Vector3(0.75f), Vector3(0.5f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowLeft->SetPosition(Vector3(100.0f, 360.0f, 0.0f));
	pArrowLeft->SetOrigin(pArrowLeft->AskOrigin());
	pArrowLeft->SetAngle(180.0f);
	pArrowLeft->SetColor(Colors::Aqua);
	m_leftArrow = pArrowLeft.get();

	//右の矢印を生成する
	std::unique_ptr<Arrow> pArrowRight = std::make_unique<Arrow>(arrow);
	pArrowRight->InitArrow(Vector3(0.5f), Vector3(0.75f), Vector3(0.5f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowRight->SetPosition(Vector3(1150.0f, 360.0f, 0.0f));
	pArrowRight->SetOrigin(pArrowRight->AskOrigin());
	pArrowRight->SetColor(Colors::Aqua);
	m_rightArrow = pArrowRight.get();

	//Escapeキーの背面のテクスチャを生成する
	std::unique_ptr<TextureBase> pEscapeBack = std::make_unique<TextureBase>(whiteBack);
	pEscapeBack->SetPosition(Vector3(250.0f, 580.0f, 0.0f));
	pEscapeBack->SetRect(RECT{ 0,0,800,120 });
	pEscapeBack->SetBlink(true);
	pEscapeBack->SetTarget(0.5f);
	m_escapeBack = pEscapeBack.get();

	//ランキングの背面を生成する
	for (int i = 0; i < (int)ResultScene::STAGENUM; i++)
	{
		std::unique_ptr<ITextureBase> pRankingBack = std::make_unique<TextureBase>(whiteBack);
		pRankingBack->SetRect(RECT{ 0,0,960,600 });
		pRankingBack->SetPosition(Vector3(1500.0f, 75.0f, 0.0f));
		pRankingBack->SetColor(Colors::Orange);
		m_rankingBack.push_back(pRankingBack.get());

		//TextureManagerに追加する
		m_TextureManager->Add(std::move(pRankingBack));
	}

	//ステージ1のみ補正する
	m_rankingBack[0]->SetPosition(Vector3(150.0f, 75.0f, 0.0f));

	//TextureManagerに追加する
	m_TextureManager->Add(std::move(pArrowLeft));
	m_TextureManager->Add(std::move(pArrowRight));
	m_TextureManager->Add(std::move(pEscapeBack));
}

const std::vector<std::string> RankingScene::GetSaveDataArray(const int& data)
{
	//変数を初期化する
	std::vector<std::string> dataArray;

	//指定したデータを取得する
	for (std::string& info : m_rankingData[data])
	{
		//データが空の時は、処理を飛ばす
		if (info.empty())continue;

		//データを追加する
		dataArray.push_back(info);
	}

	//指定したデータを返す
	return dataArray;
}

const std::vector<std::vector<std::string>> RankingScene::GetSaveDataDoubleArray(const int& first, const int& finishi, const int& target, const int interval)
{
	//変数を初期化する
	std::vector<std::vector<std::string>> dataArray;

	//ループの末尾を設定する
	int fs = first + finishi;

	//指定した範囲のデータを取得する
	for (int i = target; i <= fs; i += interval)
	{
		//データを追加する
		dataArray.push_back(this->GetSaveDataArray(i));
	}

	//指定した範囲のデータを返す
	return dataArray;
}

void RankingScene::UpdateEasing()
{
	//ステートの処理

	//switch文で処理を分ける
	switch (this->GetNowData2())
	{
	case BACKOUT::NONE:
		this->State_None();
		break;
	case BACKOUT::ADMISSON:
		this->State_Admisson();
		break;
	case BACKOUT::EXIT:
		this->State_Exit();
		break;
	case BACKOUT::RESULT:
		this->State_Result();
		break;
	}
}

void RankingScene::State_None()
{
	//State_Noneの処理

//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//キーボードの状態を取得
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//右キーが押されたら
	if (keyState.Right)
	{
		//ベクトルの向きを設定する
		m_direct = 1;

		//経過時間を設定する
		m_time = CoroKun::EASING_MAX;

		//ステートを切り替える
		this->AddData2(BACKOUT::ADMISSON);

		//関数オブジェクトを設定する
		m_ifState = [&](int& nextState)
		{
			//領域外を参照しようとした
			if (nextState >= (int)m_rankingBack.size())
			{
				nextState = NULL;
			}
		};

		//スクロール対象以外を初期位置に戻す
		this->OtherInitPos(this->GetNowData1(), BEFORE_POS);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__08_スライド);
	}

	//左キーが押されたら
	if (keyState.Left)
	{
		//ベクトルの向きを設定する
		m_direct = -1;

		//経過時間を設定する
		m_time = CoroKun::EASING_MAX;

		//ステートを切り替える
		this->AddData2(BACKOUT::ADMISSON);

		//関数オブジェクトを設定する
		m_ifState = [&](int& nextState)
		{
			//領域外を参照しようとした
			if (nextState < NULL)
			{
				nextState = (int)m_rankingBack.size() - 1;
			}
		};

		//スクロール対象以外を初期位置に戻す
		this->OtherInitPos(this->GetNowData1(), AFTER_POS);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__08_スライド);
	}
}

void RankingScene::State_Admisson()
{
	//State_Admissonの処理

	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//前回のフレームからの経過時間を取得する
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//現在の添え字を取得する
	int nowState = this->GetNowData1();

	//次の添え字を取得する
	int nextState = nowState + m_direct;

	//設定されたif文の処理を行う
	m_ifState(nextState);

	//ステージの画像を取得する
	ITextureBase* nowStageTexture = m_rankingBack[nowState];

	//次のステージの画像を取得する
	ITextureBase* nextStageTexture = m_rankingBack[nextState];

	//ベクトルを設定する
	vel.x = Easing::InExp(m_time, CoroKun::EASING_TOTALTIME, CoroKun::EASING_MAX) * 250.0f * (m_direct * -1);
	nowStageTexture->SetVelocity(vel);
	nextStageTexture->SetVelocity(vel);

	//経過時間を更新する
	m_time -= elapsedTime * 2.0f;

	//時間が経過したら、ステートを切り替える
	if (m_time <= CoroKun::EASING_MIN)
	{
		m_time = CoroKun::EASING_MIN;
		nowStageTexture->SetVelocity(Vector3::Zero);
		nextStageTexture->SetVelocity(Vector3::Zero);
		this->AddData2(BACKOUT::RESULT);
	}
}

void RankingScene::State_Exit()
{
	//State_Exitの処理
}

void RankingScene::State_Result()
{
	//State_Resultの処理

	//現在の添え字を取得する
	int nowState = this->GetNowData1();

	//次の添え字を取得する
	int nextState = nowState + m_direct;

	//if文の処理
	m_ifState(nextState);

	//添え字を再設定する
	this->AddData1(nextState);

	//ステートを切り替える
	this->AddData2(BACKOUT::NONE);
}

void RankingScene::OtherInitPos(const int& target, const Vector3& pos)
{
	for (int i = 0; i < (int)m_rankingBack.size(); i++)
	{
		//ターゲット以外のものであるとき
		if (i != target)
		{
			//ステージ画像を取得
			ITextureBase* stage = m_rankingBack[i];

			//座標を設定する
			stage->SetPosition(pos);
		}
	}
}
