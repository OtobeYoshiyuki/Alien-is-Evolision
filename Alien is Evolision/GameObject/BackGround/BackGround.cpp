#include "pch.h"
#include "BackGround.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

BackGround::BackGround(ITextureBase* texture):TextureBase(texture)
{
	this->SetDrawOrder(7);
}

void BackGround::Initialize()
{
}

void BackGround::Update()
{
	TextureBase::Update();
}

void BackGround::Render()
{
	TextureBase::Render();
}

void BackGround::Finalize()
{
}
