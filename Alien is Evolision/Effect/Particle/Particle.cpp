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
	//�摜�̍X�V
	TextureBase::Update();

	//�����蔻��̍��W��ݒ肷��
	this->SetCenter(this->GetPosition());

	//�����蔻��̔��a��ݒ肷��
	this->SetRadius(Vector3(this->AskOrigin()*2));
}

void Particle::Render()
{
	//�摜�̕`��
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

