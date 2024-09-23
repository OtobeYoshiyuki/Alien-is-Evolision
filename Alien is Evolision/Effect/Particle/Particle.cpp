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
	//‰æ‘œ‚ÌXV
	TextureBase::Update();

	//“–‚½‚è”»’è‚ÌÀ•W‚ðÝ’è‚·‚é
	this->SetCenter(this->GetPosition());

	//“–‚½‚è”»’è‚Ì”¼Œa‚ðÝ’è‚·‚é
	this->SetRadius(Vector3(this->AskOrigin()*2));
}

void Particle::Render()
{
	//‰æ‘œ‚Ì•`‰æ
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

