#pragma once
#include "pch.h"
#include "LogoScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"

using namespace OtobeLib;
using namespace DirectX;

LogoScene::LogoScene():
	m_TextureManager(nullptr)//画像の管理クラス
{
}

void LogoScene::Initialize()
{
	//生成を行う
	this->Create();

	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//フェードインを行う
	sceneState->SetFadeState(SceneState::FADEIN);
}

void LogoScene::Update()
{
	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//フェードアウトが完了したら、タイトルに移行する
	if (sceneState->GetFade() == SceneState::FADERESULT)
	{
		sceneManager->ChangeScene("Title");
	}

	//キー入力待ち以外の時は、処理を飛ばす
	if (sceneState->GetFade() != SceneState::WAITKEY)return;

	//キー入力待ちの時は、すぐにフェードアウトする
	sceneState->SetFadeState(SceneState::FADEOUT);

	//画像の更新処理
	m_TextureManager->Update();
}

void LogoScene::Render()
{
	//描画系の情報を取得する
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	Graphics* Graph = Graphics::GetInstance();

	//描画の設定を行う
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//テクスチャを描画
	m_TextureManager->Render();

	//描画の設定を終了する
	Graph->GetSpriteBatch()->End();
}

void LogoScene::Finalize()
{
}

void LogoScene::Create()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//使うテクスチャを取得する
	ITextureBase* LogoTexture = resourceManager->GetTexture("Logo");

	//Logoのテクスチャを生成する
	std::unique_ptr<ITextureBase>pTexture = std::make_unique<TextureBase>(LogoTexture);

	//Managerクラスに登録する
	m_TextureManager = std::make_unique<TextureManager>();
	m_TextureManager->Add(std::move(pTexture));
}

