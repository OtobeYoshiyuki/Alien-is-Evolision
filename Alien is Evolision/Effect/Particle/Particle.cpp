#include "pch.h"
#include "Particle.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Particle::Particle(ITextureBase* texture):TextureBase(texture)
{
	this->SetDrawOrder(2);
}

void Particle::Initialize()
{
}

void Particle::Update()
{
	//画像の更新
	TextureBase::Update();

	//当たり判定の座標を設定する
	this->SetCenter(this->GetPosition());

	//当たり判定の半径を設定する
	this->SetRadius(Vector3(this->AskOrigin()*2));
}

void Particle::Render()
{
	//画像の描画
	TextureBase::Render();
}

void Particle::Finalize()
{
}

void Particle::OnCollisionEnter(ITextureBase* obj)
{
	CollisionBase::OnCollisionEnter(obj);
}

void Particle::NonCollisionEnter(ITextureBase* obj)
{
	CollisionBase::NonCollisionEnter(obj);
}

