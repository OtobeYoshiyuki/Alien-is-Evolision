#include "pch.h"
#include "MenuScene.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//ステータスの座標
const Vector3 MenuScene::STATUS_POS = Vector3(50.0f,150.0f,0.0f);

//アイテムの座標
const Vector3 MenuScene::ITEM_POS = Vector3(50.0f, 350.0f, 0.0f);

//キャラブックの座標
const Vector3 MenuScene::CHARBOOK_POS = Vector3(50.0f, 550.0f, 0.0f);

//画像の座標
const std::vector<DirectX::SimpleMath::Vector3> MenuScene::TEXPOS =
{
	STATUS_POS,//ステータスの座標
	ITEM_POS,//アイテムの座標
	CHARBOOK_POS,//キャラブックの座標
};

//各Sceneの情報
const std::vector<std::string> MenuScene::SCENES_INFO =
{
	"Status",//ステータスシーン
	"Item",//アイテムシーン
	"CharBook",//キャラブックシーン
};

MenuScene::MenuScene():
	m_TextureManager(nullptr),//画像の管理クラス
	m_backSpaceTexture(nullptr)//スペースキーの裏面のテクスチャ
{
}

void MenuScene::Initialize()
{
	//生成の処理
	this->Create();

	//ステートを切り替える
	this->SetNowState((int)SELECT_INFO::STATUS);

	//選択している色を変更する
	m_selectTextures[this->GetNowState()]->SetColor(Colors::Red);

	//GameContextに登録する
	GameContext::Register<MenuScene>(this);
}

void MenuScene::Update()
{
	//キーボードの状態
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();
	const auto& Mouse_Trakker = Input::GetInstance()->GetMouse_Trakker();

	//SceneManagerを取得する
	SceneManager* manager = SceneManager::GetInstance();

	//CollisionManagerを取得する
	CollisionManager* collisionManager = GameContext::Get<CollisionManager>();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//選択中の画像を取得する
	ITextureBase* nowTexture = m_selectTextures[this->GetNowState()];

	//Xキーが押されたら、メニューを閉じる
	if (keyTrakker.IsKeyPressed(Keyboard::X))
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);

		manager->PopScene();
		collisionManager->ResetUseHero();
		return;
	}

	//Spaceキーが押されたら、選択したSceneへ移行する
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		this->InputEntry();
	}
	//色が赤の時
	else if (nowTexture->GetName() == "白")
	{
		//色を白に変更する
		nowTexture->SetColor(Colors::Red);

		//タグを変更する
		nowTexture->SetName("赤");

		//Sceneを追加生成する
		if (SCENES_INFO[this->GetNowState()] != "CharBook")
		{
			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__05_決定音);

			manager->PushScene(SCENES_INFO[this->GetNowState()]);
		}
	}
		
	//下キーが押されたとき
	if (keyTrakker.IsKeyPressed(Keyboard::Down))
	{
		this->InputSelect(1, [&](int& num)
			{
				//配列の参照領域を超えたら、補正する
				if (num >= (int)m_selectTextures.size())num = NULL;});

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__07_上下音);
	}

	//上キーが押されたとき
	if (keyTrakker.IsKeyPressed(Keyboard::Up))
	{
		this->InputSelect(-1, [&](int& num)
			{
				//配列の参照領域を超えたら、補正する
				if (num <= -1)num = (int)m_selectTextures.size() - 1;});

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__07_上下音);
	}

	//画像の管理クラスの更新処理
	m_TextureManager->Update();
}

void MenuScene::Render()
{
	//デバイスコンテキストを取得する
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//描画系の情報を取得する
	Graphics* Graph = Graphics::GetInstance();

	//フォントを取得する
	TextureFont* font = TextureFont::GetInstance();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//画像の管理クラスの描画処理
	m_TextureManager->Render();

	//フォントを描画する
	font->DrawFormatString("ステ－タス", Vector2(STATUS_POS) + Vector2(10.0f,20.0f),1.8f,Colors::Black);
	font->DrawFormatString("アイテム", Vector2(ITEM_POS) + Vector2(30.0f, 20.0f), 1.8f, Colors::Black);
	font->DrawFormatString("キャラブック", Vector2(CHARBOOK_POS) + Vector2(0.0f, 20.0f), 1.5f, Colors::Black);
	font->DrawFormatString("Xキ－：プレイへ", Vector2(50.0f, 60.0f), 1.0f, Colors::Black);

	//点滅中じゃないとき
	if (m_backSpaceTexture->NowBlinking())
	{
		font->DrawFormatString("PUSH SPACE", Vector2(500.0f, 350.0f), 2.0f, Colors::Black);
	}

	Graph->GetSpriteBatch()->End();
}

void MenuScene::Finalize()
{
}

void MenuScene::Create()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを生成する
	m_TextureManager = std::make_unique<TextureManager>();

	//使用するテクスチャを取得する
	ITextureBase* menuTexture = resourceManager->GetTexture("Menu");
	ITextureBase* whiteback = resourceManager->GetTexture("WhiteBack");

	//メニューの画像を生成する
	std::unique_ptr<ITextureBase> pmenuTexture = std::make_unique<TextureBase>(menuTexture);

	//TextureManagerに追加する
	m_TextureManager->Add(std::move(pmenuTexture));

	for (const Vector3& pos : TEXPOS)
	{
		//画像を生成する
		std::unique_ptr<FadeTexture> ptexture = std::make_unique<FadeTexture>(whiteback);

		//座標を設定する
		ptexture->SetPosition(pos);

		//RECTを設定する
		ptexture->SetRect(RECT{ 0,0,300,100 });

		//フェードのアルファ値の最小値を設定する
		ptexture->SetMinAlfa(0.25f);

		//フェードのアルファ値の最大値を設定する
		ptexture->SetMaxAlfa(1.0f);

		//生ポインタを登録する
		m_selectTextures.push_back(ptexture.get());

		//TextureManagerに追加する
		m_TextureManager->Add(std::move(ptexture));
	}

	//選択中の画像のフェードを開始する
	m_selectTextures[0]->SetNowState(FADE_STATE::DOWN);

	//SPACEキーの裏面のテクスチャを生成する
	std::unique_ptr<TextureBase> pbackSpace = std::make_unique<TextureBase>(whiteback);
	pbackSpace->SetPosition(Vector3(450.0f, 320.0f, 0.0f));
	pbackSpace->SetRect(RECT{ 0,0,750,120 });
	pbackSpace->SetBlink(true);
	pbackSpace->SetTarget(0.5f);
	m_backSpaceTexture = pbackSpace.get();

	//Xキーの裏面のテクスチャを生成する
	std::unique_ptr<ITextureBase> pbackXkey = std::make_unique<TextureBase>(whiteback);
	pbackXkey->SetPosition(Vector3(30.0f, 50.0f, 0.0f));
	pbackXkey->SetRect(RECT{ 0,0,300,50 });

	//TextureManagerに追加する
	m_TextureManager->Add(std::move(pbackSpace));
	m_TextureManager->Add(std::move(pbackXkey));
}

void MenuScene::InputSelect(const int& add, std::function<void(int&)> func)
{
	//現在の状態を取得する
	int nowSelect = this->GetNowState();

	//現在の色を取得する
	Color nowColor = m_selectTextures[nowSelect]->GetColor();

	//新しい状態を取得する
	int newSelect = nowSelect + add;

	//ラムダ式から関数を実行
	func(newSelect);

	//新しい色を取得する
	Color newColor = m_selectTextures[newSelect]->GetColor();

	//色を入れ替える
	std::swap(nowColor, newColor);

	//ステートを更新する
	this->SetNowState(newSelect);

	//色を設定する
	m_selectTextures[nowSelect]->SetColor(nowColor);
	m_selectTextures[newSelect]->SetColor(newColor);

	//フェードのステートを設定する
	m_selectTextures[nowSelect]->SetNowState(FADE_STATE::NONE);
	m_selectTextures[newSelect]->SetNowState(FADE_STATE::DOWN);
}

void MenuScene::InputEntry()
{
	//選択中の画像を取得する
	ITextureBase* nowTexture = m_selectTextures[this->GetNowState()];

	//色を変更する
	nowTexture->SetColor(Colors::White);

	//タグを変更する
	nowTexture->SetName("白");
}

