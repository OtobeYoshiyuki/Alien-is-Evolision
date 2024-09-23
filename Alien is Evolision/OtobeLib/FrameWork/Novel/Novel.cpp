#include "pch.h"
#include "Novel.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Novel::Novel():
	m_nameWindow(nullptr),//名前のウィンドウ
	m_textWindow(nullptr),//テキスト欄のウィンドウ
	m_story(nullptr)//ストーリー
{
}


Novel::~Novel()
{
}

void Novel::Initialize(const wchar_t* fileName, TextureManager* texMana)
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//使用するテクスチャを取得する
	ITextureBase* textWindow = resourceManager->GetTexture("ウィンドウ");
	ITextureBase* nameWindow = resourceManager->GetTexture("名前バー");

	//ウィンドウを生成
	std::unique_ptr<TextWindow>ptextWindow = std::make_unique<TextWindow>(textWindow);
	//std::unique_ptr<NameWindow>pnameWindow = std::make_unique<NameWindow>(nameWindow);

	//所有者を設定する
	ptextWindow->SetOwner(this);
	//pnameWindow->SetOwner(this);

	//ストーリーを生成
	m_story = std::make_unique<Story>();
	m_story->Initialize(fileName);

	//生ポインタを登録
	m_textWindow = ptextWindow.get();
	//m_nameWindow = pnameWindow.get();

	//TextureManagerに追加
	texMana->Add(std::move(ptextWindow));
	//texMana->Add(std::move(pnameWindow));

	//ウィンドウの初期化
	m_textWindow->Initialize();
	//m_nameWindow->Initialize();
}

void Novel::Update()
{
}

void Novel::Finalize()
{
}
