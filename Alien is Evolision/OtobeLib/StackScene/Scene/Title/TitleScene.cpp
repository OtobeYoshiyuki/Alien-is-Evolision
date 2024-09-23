#include "pch.h"
#include "TitleScene.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//ステージセレクトへ
const std::pair<std::string, Vector2> TitleScene::NEXT_STAGESELECT = { "ステ－ジセレクトへ" ,Vector2(200.0f, 450.0f) };

//ランキングへ
const std::pair<std::string, Vector2> TitleScene::NEXT_RANKING = { "ランキングへ" ,Vector2(350.0f, 450.0f) };

//オープニングへ
const std::pair<std::string, DirectX::SimpleMath::Vector2> TitleScene::NEXT_OPENING = {"オ－プニングへ",Vector2(300.0f,450.0f)};

TitleScene::TitleScene():
	m_TextureManager(nullptr),//画像の管理クラス
	m_blink(nullptr),//文字の点滅クラス
	m_leftArrowTexture(nullptr),//矢印のテクスチャ
	m_rightArrowTexture(nullptr),//矢印のテクスチャ
	m_targetStrings(std::pair<std::string,Vector3>("",Vector3::Zero))//表示する文字
{
}

void TitleScene::Initialize()
{
	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//ファイルの取得
	Stream* file = Stream::GetInstance();

	//ファイルを開けたら、開く
	if (file->Open(L"OpeningInfo.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		std::vector<std::vector<std::string>> LoadData = file->Load_Data();

		//静的データに情報を追加する
		staticData->Add("オープニング", LoadData[1][0]);

		//ファイルを閉じる
		file->Close();
	}

	//生成を行う
	this->Create();

	//BGMを再生する
	soundManager->Play(CRI_CUESHEET_0__01_タイトル用BGM);

	//フェードインを行う
	sceneState->SetFadeState(SceneState::FADEIN);

	//オープニングが視聴済みの場合
	if (staticData->Get_StringData("オープニング") == "視聴済み")
	{
		//表示ターゲットを設定する
		m_targetStrings = NEXT_STAGESELECT;
	}
	//オープニングが未視聴の場合
	else
	{
		//表示ターゲットを設定する
		m_targetStrings = NEXT_OPENING;
	}
}

void TitleScene::Update()
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

	//フェードアウトが完了したら、タイトルに移行する
	if (sceneState->GetFade() == SceneState::FADERESULT)
	{
		//ステージセレクトの時
		if (m_targetStrings == NEXT_STAGESELECT)
		{
			//ステージセレクトSceneに切り替える
			sceneManager->ChangeScene("StageSelect");
		}
		//オープニングの時
		else if (m_targetStrings == NEXT_OPENING)
		{
			//ステージセレクトSceneに切り替える
			sceneManager->ChangeScene("Opening");
		}
	}

	//キー入力待ち以外の時は、処理を飛ばす
	if (sceneState->GetFade() != SceneState::WAITKEY)return;

	//Spaceキーを入力
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//ステージセレクトの時
		if (m_targetStrings == NEXT_STAGESELECT)
		{
			//フェードアウトを行う
			sceneState->SetFadeState(SceneState::FADEOUT);
		}
		//ランキングの時
		else if (m_targetStrings == NEXT_RANKING)
		{
			//RankingSceneを追加生成する
			sceneManager->PushScene("Ranking");
		}
		//オープニングの時
		else if (m_targetStrings == NEXT_OPENING)
		{
			//フェードアウトを行う
			sceneState->SetFadeState(SceneState::FADEOUT);
		}

		//決定の音を鳴らす
		soundManager->Play(CRI_CUESHEET_0__05_決定音);
	}

	//オープニングが視聴済みの場合
	if (staticData->Get_StringData("オープニング") == "視聴済み")
	{
		//右キーを入力
		if (keyTrakker.IsKeyPressed(Keyboard::Right))
		{
			//ステージセレクトの時
			if (m_targetStrings == NEXT_STAGESELECT)m_targetStrings = NEXT_RANKING;

			//ランキングの時
			else if (m_targetStrings == NEXT_RANKING)m_targetStrings = NEXT_OPENING;

			//オープニングの時
			else if (m_targetStrings == NEXT_OPENING)m_targetStrings = NEXT_STAGESELECT;

			//選択音を鳴らす
			soundManager->Play(CRI_CUESHEET_0__08_スライド);
		}

		//左キーを入力
		if (keyTrakker.IsKeyPressed(Keyboard::Left))
		{
			//ステージセレクトの時
			if (m_targetStrings == NEXT_STAGESELECT)m_targetStrings = NEXT_OPENING;

			//ランキングの時
			else if (m_targetStrings == NEXT_RANKING)m_targetStrings = NEXT_STAGESELECT;

			//オープニングの時
			else if (m_targetStrings == NEXT_OPENING)m_targetStrings = NEXT_RANKING;

			//選択音を鳴らす
			soundManager->Play(CRI_CUESHEET_0__08_スライド);

		}
	}

	//TextureManagerの更新
	m_TextureManager->Update();

	//点滅の更新
	m_blink->Update();
}

void TitleScene::Render()
{
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	Graphics* Graph = Graphics::GetInstance();

	TextureFont* Font = TextureFont::GetInstance();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	m_TextureManager->Render();

	//フォントを描画する
	if (m_blink->NowBlinking())
	{
		//モードの描画
		Font->DrawFormatString(m_targetStrings.first, m_targetStrings.second, 3.0f, Colors::Red);

		//スペースキーの描画
		Font->DrawFormatString("PUSH SPACE", Vector2(350.0f, 600.0f), 2.0f, Colors::Black);
	}


	Graph->GetSpriteBatch()->End();
}

void TitleScene::Finalize()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//BGMを消す
	soundManager->Stop(CRI_CUESHEET_0__01_タイトル用BGM);
}

void TitleScene::Create()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//画像の管理クラスを生成する
	m_TextureManager = std::make_unique<TextureManager>();

	//使うテクスチャを取得する
	ITextureBase* TitleTexture = resourceManager->GetTexture("Title");
	ITextureBase* arrowTexture = resourceManager->GetTexture("矢印");

	//TitleのTextureを生成する
	std::unique_ptr<ITextureBase>pTexture = std::make_unique<TextureBase>(TitleTexture);

	//TextureManagerに追加する
	m_TextureManager->Add(std::move(pTexture));

	//矢印のTextureを生成する
	//オープニングを視聴済みの場合
	if (staticData->Get_StringData("オープニング") == "視聴済み")
	{
		//左の矢印を生成する
		std::unique_ptr<Arrow> pleftArrow = std::make_unique<Arrow>(arrowTexture);
		pleftArrow->InitArrow(Vector3(0.5f), Vector3(0.75f), Vector3(0.5f), 60.0f, ARROWSCALE::UPSCALE);
		pleftArrow->SetOrigin(pleftArrow->AskOrigin());
		pleftArrow->SetPosition(Vector3(100.0f, 500.0f, 0.0f));
		pleftArrow->SetAngle(180.0f);
		pleftArrow->SetColor(Colors::Blue);
		m_leftArrowTexture = pleftArrow.get();

		//TextureManagerに追加する
		m_TextureManager->Add(std::move(pleftArrow));


		//右の矢印を生成する
		std::unique_ptr<Arrow> prightArrow = std::make_unique<Arrow>(arrowTexture);
		prightArrow->InitArrow(Vector3(0.5f), Vector3(0.75f), Vector3(0.5f), 60.0f, ARROWSCALE::UPSCALE);
		prightArrow->SetOrigin(prightArrow->AskOrigin());
		prightArrow->SetPosition(Vector3(1150.0f, 500.0f, 0.0f));
		prightArrow->SetColor(Colors::Blue);
		m_rightArrowTexture = prightArrow.get();

		//TextureManagerに追加する
		m_TextureManager->Add(std::move(prightArrow));
	}

	//文字の点滅クラスを生成する
	m_blink = std::make_unique<Blinking>();
	m_blink->SetBlink(true);
	m_blink->SetTarget(0.5f);
}

