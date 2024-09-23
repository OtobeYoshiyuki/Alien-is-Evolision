#include "pch.h"
#include "StatusScene.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/StackScene/Scene/Menu/MenuScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//ステータス関連の連想配列
std::vector<std::pair<std::string, Vector2>> StatusScene::s_statusInfo =
{
	{"なまえ：コロくん",Vector2(450.0f,25.0f)},
	{"LV",Vector2(425.0f,65.0f)},
	{"HP",Vector2(425.0f,105.0f)},
	{"MP",Vector2(425.0f,145.0f)},
	{"AT",Vector2(425.0f,185.0f)},
	{"DF",Vector2(425.0f,225.0f)},
	{"SP",Vector2(425.0f,265.0f)},
	{"NT",Vector2(425.0f,305.0f)},
	{"TL",Vector2(425.0f,345.0f)}
};

//コマンド関連の連想配列
const std::vector<std::pair<std::string, Vector2>> StatusScene::COMMAND_INFO =
{
	{"コマンドわざ",Vector2(475.0f,400.0f)},
	{"たいあたり：ニ－ドル",Vector2(425.0f,440)},
	{"ビ－ム：リザレクション",Vector2(425.0f,480.0f)},
	{"ひっさつわざ",Vector2(475.0f,550.0f)},
	{"ダ－ク",Vector2(425.0f,590.0f)},
	{"エクスプロ－ジョン",Vector2(475.0f,622.0f)},
	{"リバ－スアビリティ",Vector2(425.0f,662.0f)}
};

StatusScene::StatusScene():
	m_TextureManager(nullptr),//画像の管理クラス
	m_backSpaceTexture(nullptr)//スペースキーの裏面のテクスチャ
{
}

void StatusScene::Initialize()
{
	//生成処理
	this->Create();

	//PlaySceneを取得する
	PlayScene* playScene = GameContext::Get<PlayScene>();

	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//次のレベルまでの経験値
	int nextLevel = NULL;

	//スタティック変数を上書きする
	s_statusInfo[(int)STATUS_INFO::LEVEL].first = "LV：" + std::to_string(player->GetLevel());
	s_statusInfo[(int)STATUS_INFO::HP].first = "HP：" + std::to_string(player->GetHP()) + "/" + std::to_string(player->GetMaxHP());
	s_statusInfo[(int)STATUS_INFO::MP].first = "MP：" + std::to_string(player->GetMP()) + "/" + std::to_string(player->GetMaxMP());
	s_statusInfo[(int)STATUS_INFO::AT].first = "AK：" + std::to_string(player->GetAtack());
	s_statusInfo[(int)STATUS_INFO::DF].first = "DF：" + std::to_string(player->GetDefence());

	s_statusInfo[(int)STATUS_INFO::SP].first = "SP：" + std::to_string(player->GetSpecial()) +
													"/" + std::to_string(player->GetMaxSpecialPoint());
	s_statusInfo[(int)STATUS_INFO::TOTAL].first = "TL：" + std::to_string(player->GetNextLevelPoint());

	//次のレベルまでの経験値は、レベル1から現在のレベルまでの経験値の最大を足して、
	//獲得済みの経験値を引いたものとする
	for (int i = CoroKun::MIN_LEVEL; i <= player->GetLevel(); i++)
	{
		//そのレベルのステータスを取得する
		StatusImporter* importer = StatusBase::FindStatus(player->GetName(), i);

		//経験値の最大を加算していく
		nextLevel += importer->GetNextLevelPoint();
	}

	//1つ前のレベルの経験値の最大を引く
	if (player->GetLevel() - 1 > NULL)
	{
		nextLevel -= player->GetNextLevelPoint();
		nextLevel = player->GetMaxNextLevelPoint() - nextLevel;

		//if (nextLevel < NULL)
		//{
		//	nextLevel += player->GetNextLevelPoint();
		//}
	}
	else
		//レベルが1の時
	{
		nextLevel = player->GetNextLevelPoint();
	}

	//レベルが最大の時は、強制的に０にする
	if (player->GetLevel() == CoroKun::MAX_LEVEL)
	{
		nextLevel = player->GetMaxNextLevelPoint();
	}

	//次のレベルまでの経験値
	s_statusInfo[(int)STATUS_INFO::NEXT].first = "NT：" + std::to_string(nextLevel) +
		"/" + std::to_string(player->GetMaxNextLevelPoint());

	//経験値を計算中
	if (player->GetExperienceGage()->CheckExp())
	{
		s_statusInfo[(int)STATUS_INFO::NEXT].first = "NT：けいさんちゅう";
	}
}

void StatusScene::Update()
{
	//キーボードの状態
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();
	const auto& Mouse_Trakker = Input::GetInstance()->GetMouse_Trakker();

	//SceneManagerを取得する
	SceneManager* manager = SceneManager::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//MenuSceneを取得する
	MenuScene* menu = GameContext::Get<MenuScene>();

	//Xキーが押されたら、メニューに戻る
	if (keyTrakker.IsKeyPressed(Keyboard::X))
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);

		manager->PopScene();
	}

	//ESCキーが押されたら、Playに戻る
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);

		manager->PopScene(2);
	}

	//TextureManagerの更新
	m_TextureManager->Update();

	//MenuのTextureManagerの更新をかける(UIを動かすため)
	menu->GetTextureManager()->Update();
}

void StatusScene::Render()
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

	for (const std::pair<std::string, Vector2>& status : s_statusInfo)
	{
		//フォントを描画する
		font->DrawFormatString(status.first, status.second, 1.0f, Colors::Black);
	}

	for (const std::pair<std::string, Vector2>& tech : COMMAND_INFO)
	{
		//フォントを描画する
		font->DrawFormatString(tech.first, tech.second, 1.0f, Colors::Black);
	}

	//Xキーのフォントの描画
	font->DrawFormatString("Xキ－：メニュ－へ", Vector2(50.0f, 60.0f), 1.0f, Colors::Black);

	//ESCキーのフォントの描画
	font->DrawFormatString("ESCキ－：プレイへ", Vector2(925.0f, 60.0f), 1.0f, Colors::Black);

	Graph->GetSpriteBatch()->End();
}

void StatusScene::Finalize()
{
}

void StatusScene::Create()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを生成する
	m_TextureManager = std::make_unique<TextureManager>();

	//使用するテクスチャを取得する
	ITextureBase* back = resourceManager->GetTexture("ステータス画面");
	ITextureBase* player = resourceManager->GetTexture("ステータスプレイヤー");
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");

	//背景を生成する
	std::unique_ptr<ITextureBase> pback = std::make_unique<TextureBase>(back);
	std::unique_ptr<ITextureBase> pPlayer = std::make_unique<TextureBase>(player);
	std::unique_ptr<ITextureBase> pwhite = std::make_unique<TextureBase>(whiteBack);

	//座標を設定する
	pwhite->SetPosition(Vector3(400.0f, 0.0f, 0.0f));

	//色を設定する
	pwhite->SetColor(Color{ 1.0f,1.0f,1.0f,0.5f });

	//切り取り位置を設定する
	pwhite->SetRect(RECT{ 0,0,480,720 });

	//Xキーの裏面のテクスチャを生成する
	std::unique_ptr<ITextureBase> pbackXkey = std::make_unique<TextureBase>(whiteBack);
	pbackXkey->SetPosition(Vector3(30.0f, 50.0f, 0.0f));
	pbackXkey->SetRect(RECT{ 0,0,330,50 });

	//ESCキーの裏面のテクスチャを生成する
	std::unique_ptr<ITextureBase> pbackEsckey = std::make_unique<TextureBase>(whiteBack);
	pbackEsckey->SetPosition(Vector3(900.0f, 50.0f, 0.0f));
	pbackEsckey->SetRect(RECT{ 0,0,370,50 });

	//TextureManagerに追加する
	m_TextureManager->Add(std::move(pback));
	m_TextureManager->Add(std::move(pPlayer));
	m_TextureManager->Add(std::move(pwhite));
	m_TextureManager->Add(std::move(pbackXkey));
	m_TextureManager->Add(std::move(pbackEsckey));
}
