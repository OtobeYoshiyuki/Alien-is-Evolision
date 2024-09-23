#include "pch.h"
#include "OpeningScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "GameObject/Player/CoroKun.h"
#include "ExternalLib/Easing/Easing.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/Array/Array.h"

using namespace OtobeLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace ExternalLib;

//スクロール前の座標
const Vector3 OpeningScene::BEFORE_POS = Vector3(1550.0f, 75.0f, 0.0f);

//スクロール後の座標
const Vector3 OpeningScene::AFTER_POS = Vector3(-1300.0f, 75.0f, 0.0f);

//コンストラクタ
OpeningScene::OpeningScene():
	m_textureManager(nullptr),//画像の管理クラス
	m_time(0.0f),//イージング関数の時間
	m_direct(NULL),//ベクトルの向き
	m_ifState(nullptr),//関数内で実行する関数
	m_novel(nullptr)//ノベルの管理クラス
{
}

//初期化処理
void OpeningScene::Initialize()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//生成処理
	this->Create();

	//添え字を設定する
	this->AddData1(NULL);

	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//フェードインを行う
	state->SetFadeState(SceneState::FADEIN);

	//フェードの加算速度を設定する
	state->SetFadeSpeed(SceneState::ALFA_SPEED);

	//ノベルの管理クラスを生成する
	m_novel = std::make_unique<Novel>();
	m_novel->Initialize(L"FileData/OpeningStory.csv", m_textureManager.get());

	//選択肢の初期値を設定する
	staticData->Add("選択肢", (std::string)"デフォルト");

	//テキストデータのパスを設定する
	staticData->Add("ストーリーパス", (std::string)"FileData/SkipRequest.csv");

	//BGMを再生する
	soundManager->Play(CRI_CUESHEET_0__48_オープニングBGM);
}

//更新処理
void OpeningScene::Update()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//ファイルを取得する
	Stream* file = Stream::GetInstance();

	//フェードアウトが終わった時
	if (state->GetFade() == SceneState::FADERESULT)
	{
		//オープニングが未視聴の時
		if (staticData->Get_StringData("オープニング") == "未視聴")
		{
			//ステージセレクトに切り替える
			Manager->ChangeScene("StageSelect");

			//オープニングデータを変更する
			staticData->Add("オープニング", (std::string)"視聴済み");

			//ファイルを開く
			if(file->Open(L"OpeningInfo.csv", Stream::FILE::STREAM_OFSTREAM))
			{
				//ファイルの上書き用の配列を宣言する
				std::vector<std::vector<std::string>> outputData(2, std::vector<std::string>(1, ""));

				//ファイルに上書きするパラメーターを代入する
				outputData[0][0] = "オープニング";
				outputData[1][0] = staticData->Get_StringData("オープニング");

				//ファイルに上書きする
				file->Data_Input(outputData);

				//ファイルを閉じる
				file->Close();
			}
		}
		//オープニングが視聴済みの時
		else
		{
			//タイトルに戻る
			Manager->ChangeScene("Title");
		}
	}

	//キー入力待ちの時
	if (state->GetFade() != SceneState::WAITKEY)return;

	//画像の更新処理
	m_textureManager->Update();

	//キーの更新
	this->UpdateInput();

	//イージング関数の更新処理
	this->UpdateEasing();

	//ノベルの更新処理
	m_novel->Update();
}

//描画処理
void OpeningScene::Render()
{
	//描画系の情報を取得
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//フォントを取得する
	TextureFont* font = TextureFont::GetInstance();

	//描画の設定
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//画像の描画処理
	m_textureManager->Render();

	//フォントを描画する

	//SPACEキーを描画
	font->DrawFormatString("SPACEキ－：会話を進める", Vector2(20.0f, 675.0f), 1.0f, Colors::Black);

	//ESCキーを描画
	font->DrawFormatString("ESCキ－：会話をスキップする", Vector2(700.0f, 675.0f), 1.0f, Colors::Black);

	//描画の終了
	Graph->GetSpriteBatch()->End();
}

//終了処理
void OpeningScene::Finalize()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//BGMを停止する
	soundManager->Stop(CRI_CUESHEET_0__48_オープニングBGM);
}

//生成処理
void OpeningScene::Create()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを生成する
	m_textureManager = std::make_unique<TextureManager>();

	//使用する画像を取得する
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");
	ITextureBase* arrow = resourceManager->GetTexture("矢印");
	ITextureBase* stageBack = resourceManager->GetTexture("ステージセレクト背景");

	//背景を生成する
	std::unique_ptr<ITextureBase> pback = std::make_unique<TextureBase>(stageBack);

	//TextureManagerに追加する
	m_textureManager->Add(std::move(pback));

	for (int i = 1; i <= SCROLL_NUM; i++)
	{
		//使用するテクスチャを取得する
		ITextureBase* scroll = resourceManager->GetTexture("プロローグ" + std::to_string(i));

		//スクロール用のテクスチャを生成
		std::unique_ptr<ITextureBase> pscroll = std::make_unique<TextureBase>(scroll);
		pscroll->SetPosition(BEFORE_POS);
		m_scrollTexture.push_back(pscroll.get());

		//TextureManagerに追加する
		m_textureManager->Add(std::move(pscroll));
	}

	//画面下部のUIを作成する
	std::unique_ptr<ITextureBase> pbotomUI = std::make_unique<TextureBase>(whiteBack);
	pbotomUI->SetPosition(Vector3(0.0f, 665.0f, 0.0f));
	pbotomUI->SetRect(RECT{ 0,0,1280,60 });

	//TextureManagerに追加する
	m_textureManager->Add(std::move(pbotomUI));

	//１枚目の画像のみ補正する
	m_scrollTexture[0]->SetPosition(Vector3(150.0f, 75.0f, 0.0f));
}

void OpeningScene::UpdateEasing()
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

void OpeningScene::State_None()
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

	//Storyを取得する
	Story* story = m_novel->GetStory();

	//TextWindowを取得する
	TextWindow* textWindow = m_novel->GetTextWindow();

	//現在読まれているテキストを取得する
	std::string text = story->GetStoryText(textWindow->GetDrawCount());

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//読まれているテキストがFFのとき
	if (text == "FF")
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
			if (nextState >= (int)m_scrollTexture.size() - 1)
			{
				nextState = (int)m_scrollTexture.size() - 1;
			}
		};

		//スクロール対象以外を初期位置に戻す
		this->OtherInitPos(this->GetNowData1(), BEFORE_POS);

		if (this->GetNowData1() != (int)m_scrollTexture.size() - 1)
		{
			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__08_スライド);
		}

	}

	//テキストの開始の記号の時
	if (text == "S" + std::to_string(this->GetNowData1() + 1))
	{
		//Noneの時だけ、ノベルを進める
		textWindow->SetDrawCheck(true);
	}

	//文字が終了の時
	if (text == "END")
	{
		//フェードアウトする
		sceneState->SetFadeState(SceneState::FADEOUT);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__24_ステージフェードアウト);
	}

	//スキップするとき
	if (staticData->Get_StringData("選択肢") == "はい")
	{
		//フェードアウトする
		sceneState->SetFadeState(SceneState::FADEOUT);

		//選択肢を初期化する
		staticData->Add("選択肢", (std::string)"デフォルト");

		//SEを止める
		soundManager->Stop(CRI_CUESHEET_0__47_メッセージ表示);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__24_ステージフェードアウト);
	}
	//キャンセルしたとき
	else if (staticData->Get_StringData("選択肢") == "いいえ")
	{
		//選択肢を初期化する
		staticData->Add("選択肢", (std::string)"デフォルト");

		//SEを止める
		soundManager->Play(CRI_CUESHEET_0__47_メッセージ表示);
	}

}

void OpeningScene::State_Admisson()
{
	//State_Admissonの処理

	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//TextWindowを取得する
	TextWindow* textWindow = m_novel->GetTextWindow();

	//スクロールしているときは、ノベルの更新を止める
	textWindow->SetDrawCheck(false);
	textWindow->SetActive(false);

	//前回のフレームからの経過時間を取得する
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//現在の添え字を取得する
	int nowState = this->GetNowData1();

	//次の添え字を取得する
	int nextState = nowState + m_direct;

	//設定されたif文の処理を行う
	m_ifState(nextState);

	//ステージの画像を取得する
	ITextureBase* nowStageTexture = m_scrollTexture[nowState];

	//次のステージの画像を取得する
	ITextureBase* nextStageTexture = m_scrollTexture[nextState];

	//スクロールしない場合
	if (nowStageTexture == nextStageTexture)
	{
		//ステートを切り替える
		this->AddData2(BACKOUT::RESULT);

		//以降の処理は行わない
		return;
	}

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

void OpeningScene::State_Exit()
{
	//State_Exitの処理
}

void OpeningScene::State_Result()
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

	Story* story = m_novel->GetStory();

	//TextWindowを取得する
	TextWindow* textWindow = m_novel->GetTextWindow();

	//フレーム数を初期化する
	textWindow->SetFrameCount(5);
	textWindow->SetActive(true);
}

void OpeningScene::OtherInitPos(const int& target, const Vector3& pos)
{
	for (int i = 0; i < (int)m_scrollTexture.size(); i++)
	{
		//ターゲット以外のものであるとき
		if (i != target)
		{
			//ステージ画像を取得
			ITextureBase* stage = m_scrollTexture[i];

			//座標を設定する
			stage->SetPosition(pos);
		}
	}
}

//キーの更新処理
void OpeningScene::UpdateInput()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//キーボードの状態を取得
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//Storyを取得する
	Story* story = m_novel->GetStory();

	//TextWindowを取得する
	TextWindow* textWindow = m_novel->GetTextWindow();

	//ESCキーが押されたら
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//選択肢のSceneを追加生成する
		sceneManager->PushScene("Choices");

		//SEを停止する
		soundManager->Stop(CRI_CUESHEET_0__47_メッセージ表示);
	}
}
