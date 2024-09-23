#include "pch.h"
#include "Star.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Star::Star(ITextureBase* texture):TextureBase(texture),m_AngleSpeed(0.0f),m_AutoMoveState(nullptr),m_JumpState(nullptr)
{
	//AutoMoveState�𐶐�����
	m_AutoMoveState = std::make_unique<AutoMoveState>();
	m_AutoMoveState->SetExecute(false);

	//JumpState�𐶐�����
	m_JumpState = std::make_unique<JumpState>();
	m_JumpState->SetExecute(false);

	//���X�g�ɒǉ�����
	m_MoveStateList.push_back(m_AutoMoveState.get());
	m_MoveStateList.push_back(m_JumpState.get());
}

void Star::Initialize()
{
}

void Star::Update()
{
	//�X�e�[�g�̍X�V���s��
	this->UpdateState();

	//�摜�̍X�V����
	TextureBase::Update();

	//�p�x�̍X�V���s��
	this->UpdateAngle();
}

void Star::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void Star::Finalize()
{
}

void Star::UpdateState()
{
	//�x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//�X�e�[�g�̍X�V���s��
	for (MoveState*& move : m_MoveStateList)
	{
		vel += move->Update();
	}

	//�x�N�g����ݒ肷��
	this->SetVelocity(vel);
}

void Star::UpdateAngle()
{
	//�p�x�̍X�V���s��

	float angle = this->GetAngle();

	angle += m_AngleSpeed;

	this->SetAngle(angle);
}
