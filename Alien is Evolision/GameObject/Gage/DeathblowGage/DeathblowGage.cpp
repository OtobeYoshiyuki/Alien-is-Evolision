#include "pch.h"
#include "DeathblowGage.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

DeathblowGage::DeathblowGage(ITextureBase* texture):
	TextureBase(texture),//基底クラスに情報を送信する
	m_Max(NULL)//画像の縦か横の幅の最大値
{
}

void DeathblowGage::Initialize()
{
}

void DeathblowGage::Update()
{
	//画像の更新処理
	TextureBase::Update();
}

void DeathblowGage::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void DeathblowGage::Finalize()
{
}
