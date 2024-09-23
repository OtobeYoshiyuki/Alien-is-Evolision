#include "pch.h"
#include "Resurrection.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�ҋ@����U���Ɉڂ�܂ł̎���
const float Resurrection::MOVETIME = 60.0f;

Resurrection::Resurrection():m_Active(false),m_Timer(0.0f)
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager���擾����
	TextureManager* texMana = GameContext::Get<PlayScene>()->GetGameManager().get();

	//CollisionManager���擾����
	CollisionManager* colMana = GameContext::Get<CollisionManager>();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* needle = resourceManager->GetTexture("�j�[�h��");

	for (int i = 0; i < NEEDLESHORT_NUM; i++)
	{
		//�j�[�h���𐶐�
		std::unique_ptr<Needle> pNeedleWide = std::make_unique<Needle>(needle);
		//���_��ݒ肷��
		pNeedleWide->SetOrigin(pNeedleWide->AskOrigin());
		//�`���؂�
		pNeedleWide->SetState(ITextureBase::STATE::NONE);
		//�`�揇��ύX����
		pNeedleWide->SetDrawOrder(2);
		//���O��ύX����
		pNeedleWide->SetName("longNeedle");
		//���X�g�ɒǉ�����
		m_NeedleList.push_back(pNeedleWide.get());
		//�e�N�X�`����ǉ�����
		texMana->Add(std::move(pNeedleWide));
	}

	//GameContext�ɓo�^����
	GameContext::Register<Resurrection>(this);
}

void Resurrection::Initialize()
{
}

void Resurrection::Update()
{
	//�X�e�[�g�ɂ���āA������؂�ւ���
	switch (this->GetNowState())
	{
	case RENEEDLE::LITTLE:
		this->State_Little();
		break;
	case RENEEDLE::ATACK:
		this->State_Atack();
		break;
	case RENEEDLE::END:
		this->State_End();
		break;
	}

	//�A�Ɠ����蔻������

}

void Resurrection::State_Little()
{
	//�ҋ@���̃X�e�[�g

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//�p�x��ݒ肷��
	float angle = 0.0f;

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�^�C�}�[���������Ԃ𒴂�����A�U���Ɉڍs����
	if (m_Timer >MOVETIME)
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(RENEEDLE::ATACK);
		//�^�C�}�[������������
		m_Timer = 0.0f;
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__18_�j���o);
		return;
	}

	//�j��1�������U���Ă���
	for (Needle* needle : m_NeedleList)
	{
		//�x�N�g����ݒ肷��
		vel = Vector3(0.0f, -55.0f, 0.0f);

		//��]�s���ݒ肷��
		Matrix rot = Matrix::CreateRotationZ(XMConvertToRadians(needle->GetAngle()));

		//�x�N�g������]������
		vel = Vector3::Transform(vel, rot);

		//�p�x��ݒ肷��
		needle->SetAngle(angle);

		//��]�x��30�x���X�V����
		angle += 30.0f;

		//�j�̊g��{����ݒ肷��
		needle->SetScale(Vector3(0.75f));

		//�v���C���[�̎��͂ɍ��W��ݒ肷��
		needle->SetPosition(player->GetPosition() + vel);

		//�j��`�悷��
		needle->SetState(ITextureBase::STATE::DRAW);
	}

	//�^�C�}�[���X�V����
	m_Timer ++;
}

void Resurrection::State_Atack()
{
	//�U�����̃X�e�[�g

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//�^�C�}�[���������Ԃ𒴂�����A�U���Ɉڍs����
	if (m_Timer > MOVETIME)
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(RENEEDLE::END);
		//�^�C�}�[������������
		m_Timer = 0.0f;
		return;
	}

	//�j����ĂɎ��͂ɏo��
	for (Needle* needle : m_NeedleList)
	{
		//�x�N�g����ݒ肷��
		vel = Vector3(0.0f, -105.0f, 0.0f);

		//��]�s���ݒ肷��
		Matrix rot = Matrix::CreateRotationZ(XMConvertToRadians(needle->GetAngle()));

		//�x�N�g������]������
		vel = Vector3::Transform(vel, rot);

		//�j�̊g��{����ݒ肷��
		needle->SetScale(Vector3(1.5f,3.0f,0.75f));

		//�v���C���[�̎��͂ɍ��W��ݒ肷��
		needle->SetPosition(player->GetPosition() + vel);
	}

	//�^�C�}�[���X�V����
	m_Timer ++;
}

void Resurrection::State_End()
{
	//�I���̃X�e�[�g

	//�j�̃p�����[�^�[�����ɖ߂�
	for (Needle* needle : m_NeedleList)
	{
		//�`���؂�
		needle->SetState(ITextureBase::STATE::NONE);
	}

	//���s�֐��𗎂Ƃ�
	m_Active = false;

	//�^�C�}�[������������
	m_Timer = 0.0f;

	//�X�e�[�g��؂�ւ���
	this->SetNowState(RENEEDLE::RENONE);
}
