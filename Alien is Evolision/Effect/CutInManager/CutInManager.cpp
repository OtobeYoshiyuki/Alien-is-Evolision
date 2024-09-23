#include "pch.h"
#include "CutInManager.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/CutIn/CutInScene.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CutInManager::CutInManager():
	m_cutinTexture(nullptr),//カットインのテクスチャ
	m_backTexture(nullptr),//カットインの背景のテクスチャ
	m_timer(0.0f),//タイマー
	m_totaltimer(0.0f)//シーンが生成されてからの経過時間
{
}

void CutInManager::Initialize()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを取得する
	TextureManager* textureManager = GameContext::Get<CutInScene>()->GetTextureManager();

	//使うテクスチャを取得する
	ITextureBase* cutinTexture = resourceManager->GetTexture("カットイン");
	ITextureBase* cutinBack = resourceManager->GetTexture("WhiteBack");

	//カットインを生成する
	std::unique_ptr<TextureBase> pCutin = std::make_unique<TextureBase>(cutinTexture);
	//生ポインタを登録する
	m_cutinTexture = pCutin.get();

	//カットインの背景を生成する
	std::unique_ptr<TextureBase> pCutinBack = std::make_unique<TextureBase>(cutinBack);
	//背景の色を指定する
	pCutinBack->SetColor(Colors::White);
	//座標を設定する
	pCutinBack->SetPosition(Vector3(0.0f, 200.0f, 0.0f));
	//切り取り位置を指定する
	pCutinBack->SetRect(RECT{ 0,0,1280,400 });
	//描画順を変更する
	pCutinBack->SetDrawOrder(1);
	//生ポインタを登録する
	m_backTexture = pCutinBack.get();

	//TextureManagerに追加する
	textureManager->Add(std::move(pCutin));
	textureManager->Add(std::move(pCutinBack));
}

void CutInManager::Update()
{
	//SceneManagerを取得する
	SceneManager* manager = SceneManager::GetInstance();

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//シーンの遷移から1秒後に、再び遷移する
	if (m_totaltimer > 1.0f)
	{
		//1つ前のSceneに戻る
		manager->PopScene();

		//現在のフレームの処理は実行しない
		return;
	}

	//時間によって、色を変更する
	if (m_timer > 0.05f)
	{
		//色を取得する
		Color color = m_backTexture->GetColor();
		//色の透明度が100％の時は、透明度を50％にする
		if (color.A() == 1.0f)color.A(0.5f);
		//透明度が50％の時は、100％にする
		else color.A(1.0f);
		//色を再設定する
		m_backTexture->SetColor(color);
		//タイマーを初期化する
		m_timer = 0.0f;
	}

	//タイマーを更新する
	m_timer += staticData->Get_FloatData("elapsedTime");
	m_totaltimer += staticData->Get_FloatData("elapsedTime");
}
