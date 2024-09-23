#pragma once
#include "pch.h"
#include "ChoicesScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Input/Input.h"

using namespace OtobeLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;



//コンストラクタ
ChoicesScene::ChoicesScene():
	m_textureManager(nullptr),//画像の管理クラス
	m_novel(nullptr),//ノベルの管理クラス
	m_yesTexture(nullptr),//はいのテクスチャ
	m_noTexture(nullptr)//いいえのテクスチャ
{
}

//初期化処理
void ChoicesScene::Initialize()
{
	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//生成処理
	this->Create();

	//ステートの初期化
	this->AddData1(CHOICESSTATE::STORYWAIT);

	//選択項目の初期化
	this->AddData2(NULL);
}

//更新処理
void ChoicesScene::Update()
{
	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//キー入力を取得する
	Input* input = Input::GetInstance();

	//トラッカーを取得する
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//Xキーが押されたら
	if (keyTrakker.IsKeyPressed(Keyboard::X))
	{
		//現在のSceneを消す
		sceneManager->PopScene();

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);

		//選択項目を設定する
		staticData->Add("選択肢", (std::string)"いいえ");

		//以降の処理は行わない
		return;
	}

	//画像の管理クラスの更新
	m_textureManager->Update();

	//ノベルの管理クラスの更新
	m_novel->Update();

	//ステートの更新処理
	this->UpdateState();
}

//描画処理
void ChoicesScene::Render()
{
	//コンテキストを取得する
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//描画系クラスを取得する
	Graphics* graph = Graphics::GetInstance();

	//フォントを取得する
	TextureFont* font = TextureFont::GetInstance();

	//描画の初期設定をする
	graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		graph->GetCommonStates()->NonPremultiplied(), graph->GetCommonStates()->PointClamp(),
		graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//画像を描画
	m_textureManager->Render();

	//はいのテクスチャが表示されている
	if (m_yesTexture->GetState() == ITextureBase::STATE::DRAW)
	{
		//フォントを描画
		font->DrawFormatString("はい", Vector2(250.0f, 180.0f), 3.0f, Colors::Black,64);
	}

	//いいえのテクスチャが表示されている
	if (m_noTexture->GetState() == ITextureBase::STATE::DRAW)
	{
		//フォントを描画
		font->DrawFormatString("いいえ", Vector2(720.0f, 180.0f), 3.0f, Colors::Black,20);
	}

	//フォントを描画する

	//矢印キーの描画
	font->DrawFormatString("左右キ－：移動", Vector2(20.0f, 675.0f), 1.0f, Colors::Black);

	//SPACEキーの描画
	font->DrawFormatString("SPACEキ－：決定", Vector2(500.0f, 675.0f), 1.0f, Colors::Black);

	//メニューへ戻る
	font->DrawFormatString("Xキ－：キャンセル", Vector2(950.0f, 675.0f), 1.0f, Colors::Black);

	//描画の終了を適応する
	graph->GetSpriteBatch()->End();
}

//終了処理
void ChoicesScene::Finalize()
{
}

//生成処理
void ChoicesScene::Create()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//使用するテクスチャを取得する
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");

	//画像の管理クラスを生成する
	m_textureManager = std::make_unique<TextureManager>();

	//Sceneの区切り用のテクスチャを生成
	std::unique_ptr<ITextureBase> pseparation = std::make_unique<TextureBase>(whiteBack);
	pseparation->SetRect(RECT{ 0,0,1280,720 });
	pseparation->SetColor(Color(0.0f, 0.0f, 0.0f, 0.5f));

	//選択肢を生成する
	//はいのテクスチャの四角い枠を生成
	std::unique_ptr<ITextureBase> pyes = std::make_unique<TextureBase>(whiteBack);
	pyes->SetPosition(Vector3(200.0f, 150.0f, 0.0f));
	pyes->SetRect(RECT{ 0,0,350,150 });
	pyes->SetColor(Colors::Red);
	pyes->SetState(ITextureBase::STATE::NONE);
	m_yesTexture = pyes.get();

	//いいえのテクスチャの四角い枠を生成
	std::unique_ptr<ITextureBase> pno = std::make_unique<TextureBase>(whiteBack);
	pno->SetPosition(Vector3(700.0f, 150.0f, 0.0f));
	pno->SetRect(RECT{ 0,0,350,150 });
	pno->SetState(ITextureBase::STATE::NONE);
	m_noTexture = pno.get();

	//TextureManagerに追加する
	m_textureManager->Add(std::move(pseparation));
	m_textureManager->Add(std::move(pyes));
	m_textureManager->Add(std::move(pno));

	//画面下部のUIを作成する
	std::unique_ptr<ITextureBase> pbotomUI = std::make_unique<TextureBase>(whiteBack);
	pbotomUI->SetPosition(Vector3(0.0f, 665.0f, 0.0f));
	pbotomUI->SetRect(RECT{ 0,0,1280,60 });

	//TextureManagerに追加する
	m_textureManager->Add(std::move(pbotomUI));

	//ファイルのパスを取得する
	std::wstring path = TextureBase::Convert(staticData->Get_StringData("ストーリーパス"));

	//ノベルの管理クラスを生成する
	m_novel = std::make_unique<Novel>();
	m_novel->Initialize(path.c_str(), m_textureManager.get());
}

//ステートの更新処理
void ChoicesScene::UpdateState()
{
	//Switch文で処理を分ける
	switch (this->GetNowData1())
	{
	case CHOICESSTATE::STORYWAIT:
		this->StateStoryWait();
		break;
	case CHOICESSTATE::INPUT:
		this->StateInput();
		break;
	}
}

//State_StoryWaitの処理
void ChoicesScene::StateStoryWait()
{
	//ストーリーを取得する
	Story* story = m_novel->GetStory();

	//テキストウィンドウを取得する
	TextWindow* textWindow = m_novel->GetTextWindow();

	//ストーリーのテキストを取得する
	std::string storytext = story->GetStoryText(textWindow->GetDrawCount());

	//現在のテキストが＃
	//最後まで読まれた時
	if (storytext == TextWindow::NEWLINE_AND_STOP &&
		textWindow->GetDrawCount() >= (int)story->GetStoryArray().size() - 2)
	{
		//選択肢を表示する
		m_yesTexture->SetState(ITextureBase::STATE::DRAW);
		m_noTexture->SetState(ITextureBase::STATE::DRAW);

		//ステートを切り替える
		this->AddData1(CHOICESSTATE::INPUT);
	}
}

//StateInputの処理
void ChoicesScene::StateInput()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//キー入力を取得する
	Input* input = Input::GetInstance();

	//トラッカーを取得する
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//スペースキーが押されたら
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//はいを選択中の時
		if (this->GetNowData2() == YES)
		{
			//選択項目を設定する
			staticData->Add("選択肢", (std::string)"はい");
		}
		//いいえを選択中の時
		else if (this->GetNowData2() == NO)
		{
			//選択項目を設定する
			staticData->Add("選択肢", (std::string)"いいえ");

			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
		}

		//現在のSceneを消す
		sceneManager->PopScene();

		//以降の処理は行わない
		return;
	}

	//上下のキーが押されたら、選択を変更する
	if (keyTrakker.IsKeyPressed(Keyboard::Left) ||
		keyTrakker.IsKeyPressed(Keyboard::Right))
	{
		//選択対象を取得する
		int target = this->GetNowData2();

		//選択対象をデクリメントする
		target--;

		//選択対象を絶対値に直す
		target = abs(target);

		//色を取得する
		Color preColor = m_yesTexture->GetColor();
		Color nowColor = m_noTexture->GetColor();

		//入れ替える
		std::swap(preColor, nowColor);

		//色を再設定する
		m_yesTexture->SetColor(preColor);
		m_noTexture->SetColor(nowColor);

		//選択対象を設定する
		this->AddData2(target);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__07_上下音);
	}
}