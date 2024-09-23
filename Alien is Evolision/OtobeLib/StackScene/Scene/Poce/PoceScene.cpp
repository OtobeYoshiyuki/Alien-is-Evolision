#include "pch.h"
#include "PoceScene.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//フォントの情報
const std::vector<std::pair<std::string, Vector2>> PoceScene::FONT_INFO =
{
	{"プレイ",Vector2(600.0f,150.0f)},
	{"タイトル",Vector2(550.0f,450.0f)}
};

PoceScene::PoceScene():
	m_TextureManager(nullptr)//画像の管理クラス
{
}



void PoceScene::Initialize()
{
	//現在の添え字を設定する
	this->SetNowState(NULL);

	//生成処理を行う
	this->Create();
}

void PoceScene::Update()
{
	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();
	SceneState* sceneState = sceneManager->GetState();

	//キー入力を取得
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//タイトルへ戻る
	if (sceneState->GetFade() == SceneState::FADERESULT)
	{
		sceneManager->ChangeScene("Title");
	}

	//キー入力待ち以外の時は、何もしない
	if (sceneState->GetFade() != SceneState::WAITKEY)return;

	//TextureManagerの更新
	m_TextureManager->Update();

	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);

		//現在のSceneを消す
		sceneManager->PopScene();
	}


	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//プレイへ戻る場合
		if (this->GetNowState() == NULL)
		{
			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__05_決定音);

			//現在のSceneを消す
			sceneManager->PopScene();
		}
		//タイトルへ戻る場合
		else {
			//フェードアウトする
			sceneState->SetFadeState(SceneState::FADEOUT);

			//フェードの加算速度を設定する
			sceneState->SetFadeSpeed(SceneState::ALFA_SPEED);

			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__23_プレイフェードアウト);
		}
	}

	if (keyTrakker.IsKeyPressed(Keyboard::Up) ||
		keyTrakker.IsKeyPressed(Keyboard::Down))
	{
		//選択項目の更新
		int nowState = this->GetNowState();
		int newState = nowState;

		//デクリメント
		newState--;

		//絶対値に直す
		newState = abs(newState);

		//色を取得する
		Color nowColor = m_sceneTexture[nowState]->GetColor();
		Color newColor = m_sceneTexture[newState]->GetColor();

		//交換する
		std::swap(nowColor, newColor);

		//色を設定する
		m_sceneTexture[nowState]->SetColor(nowColor);
		m_sceneTexture[newState]->SetColor(newColor);

		//添え字を更新する
		this->SetNowState(newState);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__07_上下音);
	}

}

void PoceScene::Render()
{
	//描画系の情報を取得
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//フォントを取得する
	TextureFont* font = TextureFont::GetInstance();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//TextureManagerの描画
	m_TextureManager->Render();

	for (const std::pair<std::string, Vector2>& info : FONT_INFO)
	{
		//フォントを描画する
		font->DrawFormatString(info.first, info.second, 2.0f, Colors::Black);
	}

	//上下キーのフォントを描画
	font->DrawFormatString("上下キ－：移動", Vector2(210.0f, 670.0f), 1.0f, Colors::Black);

	//スペースキーのフォントの描画
	font->DrawFormatString("SPACEキ－：決定", Vector2(550.0f, 670.0f), 1.0f, Colors::Black);

	//ECSキーのフォントの描画
	font->DrawFormatString("ECSキ－：戻る", Vector2(1000.0f, 670.0f), 1.0f, Colors::Black);

	Graph->GetSpriteBatch()->End();

}

void PoceScene::Finalize()
{
}

void PoceScene::Create()
{
	//Managerクラスに登録する
	m_TextureManager = std::make_unique<TextureManager>();

	//必要なリソースを参照する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//使用するテクスチャを取得する
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");
	
	//forループで回す
	for (int i = 0; i < 2; i++)
	{
		//テクスチャを生成する
		std::unique_ptr<ITextureBase> pwhiteBack = std::make_unique<TextureBase>(whiteBack);

		//RECTを設定する
		pwhiteBack->SetRect(RECT{ 0,0,800,150 });

		//座標を設定する
		pwhiteBack->SetPosition(Vector3(250.0f, i * 300 + 100, 0.0f));

		//生ポインタを登録する
		m_sceneTexture.push_back(pwhiteBack.get());

		m_TextureManager->Add(std::move(pwhiteBack));
	}

	//現在の色を青にする
	m_sceneTexture[this->GetNowState()]->SetColor(Colors::Aquamarine);

	//画面下部のUIを描画
	std::unique_ptr<ITextureBase> pBotomBackUI = std::make_unique<TextureBase>(whiteBack);
	pBotomBackUI->SetRect(RECT{ 0,0,1080,141 });
	pBotomBackUI->SetPosition(Vector3(200.0f, 661.0f, 0.0f));

	//TextureManagerに追加する
	m_TextureManager->Add(std::move(pBotomBackUI));

}
