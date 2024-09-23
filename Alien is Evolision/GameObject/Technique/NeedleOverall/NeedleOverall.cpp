#include "pch.h"
#include "NeedleOverall.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

NeedleOverall::NeedleOverall():m_Active(false)
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager���擾����
	TextureManager* texMana = GameContext::Get<PlayScene>()->GetGameManager().get();

	//CollisionManager���擾����
	CollisionManager* colMana = GameContext::Get<CollisionManager>();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* needle = resourceManager->GetTexture("�j�[�h��");

	for (int i = 0; i < NEEDLERANGE_NUM; i++)
	{
		//�j�[�h���𐶐�
		std::unique_ptr<Needle> pNeedleOver = std::make_unique<Needle>(needle);
		//���_��ݒ肷��
		pNeedleOver->SetOrigin(pNeedleOver->AskOrigin());
		//�`���؂�
		pNeedleOver->SetState(ITextureBase::STATE::NONE);
		//�`�揇��ύX����
		pNeedleOver->SetDrawOrder(2);
		//����ւ��p�̃x�N�g���̏�����������
		pNeedleOver->SetTempVel(Vector3::Zero);
		//���O��ύX����
		pNeedleOver->SetName("shortNeedle");
		//�����蔻���o�^����
		colMana->Add(pNeedleOver.get());
		//���X�g�ɒǉ�����
		m_NeedleList.push_back(pNeedleOver.get());
		//�e�N�X�`����ǉ�����
		texMana->Add(std::move(pNeedleOver));
	}

	//GameContext�ɓo�^����
	GameContext::Register<NeedleOverall>(this);
}

void NeedleOverall::Initialize()
{
}

void NeedleOverall::Update()
{
	//Switch���ŏ�����؂�ւ���
	switch (this->GetNowState())
	{
	case NEEDLEACTION::MOPE:
		this->State_Mope();
		break;
	case NEEDLEACTION::RELEASE:
		this->State_Release();
		break;
	case NEEDLEACTION::RELOAD:
		this->State_Reload();
		break;
	}
}

void NeedleOverall::State_Mope()
{
	//���ˏ����̃X�e�[�g
	
	//�c�����̃x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//�p�x��ݒ肷��
	float angle = 0.0f;

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�j��1�������U���Ă���
	for (Needle* needle : m_NeedleList)
	{
		//�x�N�g����ݒ肷��
		vel = Vector3(0.0f, -70.0f, 0.0f);

		//��]�s���ݒ肷��i�X�N���[�����W�̂���Z����]�̂݁j
		Matrix rot = Matrix::CreateRotationZ(XMConvertToRadians(angle));

		//�p�x��ݒ肷��
		needle->SetAngle(angle);

		//��]�x��30�x���X�V����
		angle += 30.0f;

		//�x�N�g������]������
		vel = Vector3::Transform(vel, rot);

		//�v���C���[�̎��͂ɍ��W��ݒ肷��
		needle->SetPosition(player->GetPosition() + vel);

		//�j��`�悷��
		needle->SetState(ITextureBase::STATE::DRAW);

		//�x�N�g���𐳋K������
		vel.Normalize();

		//�x�N�g����␳����
		vel *= CoroKun::SPEED * 2;

		//�x�N�g����ݒ肷��
		needle->SetTempVel(vel);
	}
}

void NeedleOverall::State_Release()
{
	//�Ƃ��L���m�����˂̃X�e�[�g

	//���s�֐��𗎂Ƃ�
	this->SetActive(false);

	//�`�悳��Ă��Ȃ����𐔂���
	int drawNum = NULL;

	//�j����Ăɔ��˂���
	for (Needle* needle:m_NeedleList)
	{
		//�x�N�g����ݒ肷��
		needle->SetVelocity(needle->GetTempVel());

		if (needle->GetCheck() && needle->GetState() == ITextureBase::STATE::DRAW)
		{
			//�j�Ɠ���ւ��p�̃x�N�g���̏�����������
			needle->SetVelocity(Vector3::Zero);
			needle->SetTempVel(Vector3::Zero);
			//�`���؂�
			needle->SetState(ITextureBase::STATE::NONE);
			needle->SetCheck(false);
		}

		if (needle->GetState() == ITextureBase::STATE::NONE)
		{
			//�����X�V����
			drawNum++;
		}
	}

	//�S���������Ă�����A�����[�h�Ɉڍs����
	if (drawNum >= (int)m_NeedleList.size())
	{
		this->SetNowState(NEEDLEACTION::RELOAD);
	}
}

void NeedleOverall::State_Reload()
{
	//�Ƃ��L���m���̃����[�h�̃X�e�[�g

	this->SetNowState(NEEDLEACTION::NEEDLENONE);
}

