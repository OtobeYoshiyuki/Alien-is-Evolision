#include "pch.h"
#include "CutInScene.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CutInScene::CutInScene():
	m_CutInManager(nullptr),//カットインの管理クラス
	m_TextureManager(nullptr)//テクスチャの管理クラス
{
}

void CutInScene::Initialize()
{
	//GameContextに登録する
	GameContext::Register<CutInScene>(this);

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//TextureManagerを生成する
	m_TextureManager = std::make_unique<TextureManager>();

	//CutInManagerを生成する
	m_CutInManager = std::make_unique<CutInManager>();
	//初期化を行う
	m_CutInManager->Initialize();
	//SEを鳴らす
	soundManager->Play(CRI_CUESHEET_0__20_カットイン);
}

void CutInScene::Update()
{
	//TextureManagerの更新処理
	m_TextureManager->Update();

	//CutInManagerの更新処理
	m_CutInManager->Update();
}

void CutInScene::Render()
{
	//描画系の状態を取得する
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//描画の開始
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//画像の描画処理
	m_TextureManager->Render();

	//描画の終了
	Graph->GetSpriteBatch()->End();
}

void CutInScene::Finalize()
{
}

void CutInScene::Create()
{
}
