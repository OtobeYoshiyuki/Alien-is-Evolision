#include "pch.h"
#include "HPGage.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

HPGage::HPGage(ITextureBase* texture)
	:TextureBase(texture),//基底クラスに情報を送信する
	m_backTexture(nullptr),//裏面のテクスチャ
	m_Max(NULL),//ゲージの最大値
	m_easingTime(0.0f)//イージング関数用の時間
{
}

void HPGage::Initialize()
{

}

void HPGage::Update()
{
	//画像の更新処理
	TextureBase::Update();
}

void HPGage::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void HPGage::Finalize()
{
}

void HPGage::CreateGage(TextureManager* textureManager)
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//使用するテクスチャを取得する
	ITextureBase* gageTexture = resourceManager->GetTexture("WhiteBack");

	//背面のテクスチャを生成する
	std::unique_ptr<ITextureBase> pbackTexture = std::make_unique<TextureBase>(gageTexture);

	//座標を設定する
	pbackTexture->SetPosition(this->GetPosition());

	//切り出し位置を設定する
	pbackTexture->SetRect(this->GetRect());

	//色を設定する
	pbackTexture->SetColor(Colors::Red);

	//描画順を変更する
	pbackTexture->SetDrawOrder(5);

	//生ポインタを追加する
	m_backTexture = pbackTexture.get();

	//画像を追加する
	textureManager->Add(std::move(pbackTexture));
}
