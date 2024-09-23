#include "pch.h"
#include "TechniqueGage.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TechniqueGage::TechniqueGage(ITextureBase* texture):
	TextureBase(texture),//基底クラスに情報を送信する
	m_Max(NULL)//画像の縦か横の最大値
{
}

void TechniqueGage::Initialize()
{
}

void TechniqueGage::Update()
{
	//画像の更新処理
	TextureBase::Update();
}

void TechniqueGage::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void TechniqueGage::Finalize()
{
}
