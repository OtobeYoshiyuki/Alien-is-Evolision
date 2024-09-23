#include "pch.h"
#include "ResultScene.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "GameObject/Player/CoroKun.h"
#include "ExternalLib/Easing/Easing.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;
using namespace ExternalLib;

//最初に表示されるUIとスクロールするUIの距離
const float ResultScene::UI_INTERVAL = 1230.0f;

ResultScene::ResultScene():
	m_TextureManager(nullptr),//画像の管理クラス
	m_timerJudge(nullptr),//クリアタイム
	m_score(nullptr),//獲得スコア
	m_totalScore(nullptr),//トータルスコア
	m_time(0.0f),//経過時間
	m_pushCount(NULL),//キーを押した回数
	m_func(nullptr),//関数オブジェクト
	m_fontX(0.0f),//フォントのX座標
	m_spacekeyBlink(nullptr)//スペースキーの点滅の制御
{
}


ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//フェードインを行う
	state->SetFadeState(SceneState::FADEIN);

	//フェードの加算速度を設定する
	state->SetFadeSpeed(SceneState::ALFA_SPEED);

	//BGMを再生する
	soundManager->Play(CRI_CUESHEET_0__04_リザルト用BGM);

	//生成処理
	this->Create();

}

void ResultScene::Update()
{
	//SceneManagerの状態を取得
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//キーボードの状態
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//フェードアウトが終了
	if (state->GetFade() == SceneState::FADERESULT)
	{
		Manager->ChangeScene("Title");
	}

	//画像の管理クラスの更新
	m_TextureManager->Update();

	//スコアの更新
	m_score->Update();
	m_totalScore->Update();

	//クリアタイムの更新
	m_timerJudge->Update();

	for (std::unique_ptr<Ranking>& ranking : m_rankings)
	{
		//ランキングの更新
		ranking->Update();
	}

	//点滅の更新
	m_spacekeyBlink->Update();

	//ステートの更新処理
	this->UpdateState();
}

void ResultScene::Render()
{
	//描画系の情報を取得
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//フォントを取得
	TextureFont* font = TextureFont::GetInstance();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	m_TextureManager->Render();

	//フォントを描画

	//クリアタイム
	font->DrawFormatString("のこりタイム：" + m_timerJudge->AskTimeStringMinute() + "：" + m_timerJudge->AskTimeStringSecound(),
		Vector2(m_fontX, 200.0f), 1.5f, Colors::Black);

	//スコアの値を編集する
	int score = (int)m_score->GetScore();
	std::string scoreStr1 = std::to_string(score);
	scoreStr1 = font->Convert_NumberOfDigits(scoreStr1, 7, "0", 1);

	int totalScore = (int)m_totalScore->GetScore();
	std::string scoreStr2 = std::to_string(totalScore);
	scoreStr2 = font->Convert_NumberOfDigits(scoreStr2, 7, "0", 1);

	//獲得スコア
	font->DrawFormatString("かくとくスコア：" + scoreStr1, Vector2(m_fontX, 300.0f), 1.5f, Colors::Black);

	//トータルスコア
	font->DrawFormatString("ト－タルスコア：" + scoreStr2, Vector2(m_fontX, 400.0f), 1.5f, Colors::Black);

	//ランキングの描画
	for (int i = 0; i < (int)m_rankings.size(); i++)
	{
		if (m_rankings[i]->GetBlinking()->NowBlinking())
		{
			//スコアの値を編集する
			int score = (int)m_rankings[i]->GetScore()->GetScore();
			std::string scoreStr = std::to_string(score);
			scoreStr = font->Convert_NumberOfDigits(scoreStr, 7, "0", 1);

			font->DrawFormatString("No." + std::to_string(i + 1), Vector2(m_fontX + UI_INTERVAL, 100.0f + (200.0f * i)), 1.0f, m_rankings[i]->GetColor());
			font->DrawFormatString("レべル：" + m_rankings[i]->GetLevel(), Vector2(m_fontX + UI_INTERVAL, 150.0f + (200.0f * i)), 1.0f, m_rankings[i]->GetColor());
			font->DrawFormatString("スコア：" + scoreStr, Vector2(m_fontX + UI_INTERVAL, 200.0f + (200.0f * i)), 1.0f, m_rankings[i]->GetColor());
			font->DrawFormatString("クリアタイム：" +
				m_rankings[i]->GetTimer()->AskTimeStringMinute() + "：" + m_rankings[i]->GetTimer()->AskTimeStringSecound(),
				Vector2(m_fontX + UI_INTERVAL, 250.0f + (200.0f * i)), 1.0f, m_rankings[i]->GetColor());
		}
	}

	//スペースキーを描画
	if (!m_spacekeyBlink->NowBlinking())
	{
		font->DrawFormatString("PUSH SPACE", Vector2(350.0f, 520.0f), 1.5f, Colors::Black);
	}

	Graph->GetSpriteBatch()->End();
}

void ResultScene::Finalize()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//BGMを止める
	soundManager->Stop(CRI_CUESHEET_0__04_リザルト用BGM);
}

void ResultScene::Create()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//ファイルを取得する
	Stream* file = Stream::GetInstance();

	//必要なリソースを参照する
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* ResultTexture = resourceManager->GetTexture("Result");
	std::unique_ptr<ITextureBase>pTexture = std::make_unique<TextureBase>(ResultTexture);

	//Managerクラスに登録する
	m_TextureManager = std::make_unique<TextureManager>();
	m_TextureManager->Add(std::move(pTexture));

	//タイマーを生成する
	m_timerJudge = std::make_unique<TimerJudge>();
	m_timerJudge->SetLimitTimer(staticData->Get_IntData("のこりタイム"));
	m_timerJudge->SetCorrection(60.0f);
	m_timerJudge->SetTimerUp(false);

	//獲得スコアを生成する
	m_score = std::make_unique<Score>();
	m_score->SetSaving(NULL);
	m_score->SetScore(staticData->Get_IntData("スコア"));
	m_score->SetAdd(-1);
	m_score->SetSoundCheck(true);

	//トータルスコアを生成する
	m_totalScore = std::make_unique<Score>();
	m_totalScore->SetSaving(NULL);
	m_totalScore->SetScore(0);
	m_totalScore->SetSoundCheck(true);

	//スペースキーの点滅クラスを生成する
	m_spacekeyBlink = std::make_unique<Blinking>();
	m_spacekeyBlink->SetTarget(0.5f);
	m_spacekeyBlink->SetBlink(false);

	//ランキングデータを読み込む
	//ファイルが開けたら、開く
	if (file->Open(L"RankingData.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//データをロードする
		m_rankingData = file->Load_Data();

		//ファイルを閉じる
		file->Close();
	}

	//フォントのX座標を設定する
	m_fontX = 100.0f;

	for (int i = 0; i < (int)RESULT_INFO::RANKING_NUM; i++)
	{
		//ランキングを生成する
		std::unique_ptr<Ranking> pranking = std::make_unique<Ranking>();

		//Moveする
		m_rankings.push_back(std::move(pranking));
	}

	//ステージ1から4の中から、抜き出すデータを決定する
	int stageSelect = 10 * (staticData->Get_IntData("ステージ") - 1) + (int)STAGE1;

	//ロードしたデータから、一部だけデータを抜き出す
	std::vector<std::vector<std::string>> convertArray =
		this->GetSaveDataDoubleArray(stageSelect + 1,
			CREAR_RESULT_RANGE, stageSelect + 3, STAGE_INTERVAL);

	for (int i = 0; i < (int)m_rankings.size(); i++)
	{
		//パラメーターを設定する

		//レベルを設定する
		m_rankings[i]->SetLevel(convertArray[i][0]);

		//スコアを設定する
		m_rankings[i]->GetScore()->SetScore(std::stoi(convertArray[i][1]));

		//クリアタイムを設定する
		m_rankings[i]->GetTimer()->SetLimitTimer(std::stoi(convertArray[i][2]));
	}
}

void ResultScene::UpdateState()
{
	//ステートの更新処理

	//Switch文で処理を分ける
	switch (this->GetNowState())
	{
	case CLEARSTATE::NONE:
		this->State_None();
		break;
	case CLEARSTATE::WAITKEY:
		this->State_WaitKey();
		break;
	case CLEARSTATE::NEXT:
		this->State_Next();
		break;
	case CLEARSTATE::ADMISSON:
		this->State_Admisson();
		break;
	case CLEARSTATE::RANKING:
		this->State_Ranking();
		break;
	}
}

void ResultScene::State_None()
{
	//State_Noneの処理

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//前回のフレームからの経過時間を取得する
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//点滅しない
	m_spacekeyBlink->SetTargetCount(NULL);
	m_spacekeyBlink->SetBlink(false);

	//1回目の時
	if (m_pushCount == NULL)
	{
		//関数オブジェクトを設定する
		m_func = [&]()
		{
			//スコアの設定をする
			m_score->SetSaving(staticData->Get_IntData("スコア"));
			m_score->SetFrameSave(staticData->Get_IntData("スコア"), 60.0f);
			m_totalScore->SetSaving(staticData->Get_IntData("スコア"));
			m_totalScore->SetFrameSave(staticData->Get_IntData("スコア"), 60.0f);

			//点滅開始
			m_spacekeyBlink->SetBlink(true);
		};
	}
	//２回目以降の時
	else
	{
		//関数オブジェクトを設定する
		m_func = [&]()
		{
			//スコアの設定をする
			m_totalScore->SetSaving(m_timerJudge->GetLimitTimer() * 10);
			m_totalScore->SetFrameSave(m_timerJudge->GetLimitTimer() * 10, 60.0f);

			//クリアタイムの設定をする
			m_timerJudge->SetTimerUp(true);

			//点滅開始
			m_spacekeyBlink->SetBlink(true);
		};

	}


	//1秒経過
	if (m_time > 1.0f)
	{
		//ステートを切り替える
		this->SetNowState(CLEARSTATE::WAITKEY);

		//経過時間を初期化する
		m_time = 0.0f;

		//保存した関数を実行
		m_func();

		//経過時間の更新は行わない
		return;
	}

	//経過時間を更新
	m_time += elapsedTime;
}

void ResultScene::State_WaitKey()
{
	//State_WaitKeyの処理
	
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//キーボードの状態
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//現在のステートを設定する
	CLEARSTATE state = this->GetNowState();

	//1回目の時
	if (m_pushCount == NULL)
	{
		//ステートを設定する
		state = CLEARSTATE::NONE;

		//関数オブジェクトを設定する
		m_func = [&]()
		{
			//スコアの設定をする
			m_score->SetScore(0);
			m_score->SetSaving(0);
			m_totalScore->SetScore(staticData->Get_IntData("スコア"));
			m_totalScore->SetSaving(0);
		};
	}
	//２回目以降の時
	else
	{
		//ステートを設定する
		state = CLEARSTATE::NEXT;

		//関数オブジェクトを設定する
		m_func = [&]() 
		{
			//タイマーの設定をする
			m_timerJudge->SetLimitTimer(0);
			m_timerJudge->SetTimerUp(false);

			//スコアの設定をする
			m_totalScore->SetScore(m_totalScore->GetScore() + m_totalScore->GetSaving());
			m_totalScore->SetSaving(0); 
		};
	}

	//スペースキーが押されたら
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//ステートを切り替える
		this->SetNowState(state);

		//pushCountを更新する
		m_pushCount++;

		//保存した関数を実行
		m_func();

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__05_決定音);
	}
}

void ResultScene::State_Next()
{
	//State_Nextの処理

	//ファイルを取得する
	Stream* file = Stream::GetInstance();

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//キーボードの状態
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//ステージ1から4の中から、抜き出すデータを決定する
	int stageSelect = 10 * (staticData->Get_IntData("ステージ") - 1) + (int)STAGE1;

	//スペースキーが押されたら
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//ランキングの入れ替えが発生した場合、入れ替える
		for (int i = 0;i < (int)m_rankings.size(); i++)
		{
			//スコアがランキングのものより、大きければ、
			if (m_totalScore->GetScore() > m_rankings[i]->GetScore()->GetScore())
			{
				//スコアの序列を１個ずつ下げる
				for (int j = (int)m_rankings.size() - 1; j > i; j--)
				{
					m_rankings[j]->GetScore()->SetScore(m_rankings[j - 1]->GetScore()->GetScore());
					m_rankings[j]->GetTimer()->SetLimitTimer((float)m_rankings[j - 1]->GetTimer()->GetLimitTimer());
					m_rankings[j]->SetLevel(m_rankings[j - 1]->GetLevel());
				}

				//入れ替えを行う
				m_rankings[i]->GetScore()->SetScore(m_totalScore->GetScore());
				m_rankings[i]->GetTimer()->SetLimitTimer(staticData->Get_IntData("クリアタイム"));
				m_rankings[i]->SetLevel(staticData->Get_StringData("レベル"));
				m_rankings[i]->GetBlinking()->SetBlink(true);
				m_rankings[i]->SetColor(Colors::Red);

				//データを上書きする
				for (int z = 1; z <= 3; z++)
				{
					m_rankingData[stageSelect + (z * 3)][(int)RESULT_INFO::LEVEL] = m_rankings[z - 1]->GetLevel();
					m_rankingData[stageSelect + (z * 3)][(int)RESULT_INFO::SCORE] = std::to_string(m_rankings[z - 1]->GetScore()->GetScore());
					m_rankingData[stageSelect + (z * 3)][(int)RESULT_INFO::CLEATIME] = std::to_string(m_rankings[z - 1]->GetTimer()->GetLimitTimer());
				}

				//ファイルを開く
				if (file->Open(L"RankingData.csv", Stream::FILE::STREAM_OFSTREAM))
				{
					//Csv形式に変換する
					file->Data_Input(m_rankingData);

					//ファイルを閉じる
					file->Close();
				}

				//入れ替えを行ったら、ループを抜ける
				break;
			}
		}

		//ステートを切り替える
		this->SetNowState(CLEARSTATE::ADMISSON);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__05_決定音);
	}
}

void ResultScene::State_Admisson()
{
	//State_Admissonの処理

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//前回のフレームからの経過時間を取得する
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//時間が経過
	if (m_time > CoroKun::EASING_TOTALTIME)
	{
		//時間を初期する
		m_time = 0.0f;

		//ステートを切り替える
		this->SetNowState(CLEARSTATE::RANKING);

		//経過時間の更新は行わない
		return;
	}

	//座標を更新する
	m_fontX -= Easing::InExp(m_time, CoroKun::EASING_TOTALTIME) * 120.0f;

	//経過時間の更新を行う
	m_time += elapsedTime;
}

void ResultScene::State_Ranking()
{
	//State_Rankingの処理

	//SceneManagerの状態を取得
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* State = Manager->GetState();

	//キーボードの状態
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//スペースキーが押されたら
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//フェードアウトする
		State->SetFadeState(SceneState::FADEOUT);

		//ステートを切り替える
		this->SetNowState(CLEARSTATE::RESULT);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__25_リザルトフェードアウト);

		//点滅を止める
		m_spacekeyBlink->SetTargetCount(NULL);
		m_spacekeyBlink->SetBlink(false);
	}
}

//指定したデータを配列にして返す(std::vector<>)
const std::vector<std::string> ResultScene::GetSaveDataArray(const int& data)
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

//指定した範囲のデータを2次元配列にして返す(std::vector<std::vector<>>)
const std::vector<std::vector<std::string>> ResultScene::GetSaveDataDoubleArray
(const int& first, const int& finishi,const int& target, const int interval)
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
