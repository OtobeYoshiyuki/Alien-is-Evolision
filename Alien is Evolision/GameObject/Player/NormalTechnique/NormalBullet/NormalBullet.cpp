#include "pch.h"
#include "NormalBullet.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameObject/Player/NormalTechnique/NormalTechnique.h"
#include "GameObject/Gage/NormalGage/NormalGage.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

NormalBullet::NormalBullet(ITextureBase* texture):Particle(texture),m_Temp(Vector3::Zero),m_PlayerTarget(Vector3::Zero)
{

}

void NormalBullet::Initialize()
{
	this->SetNowState(ENERGY_CHAGE::ENERGYNONE);

	this->SetVelocity(Vector3::Zero);

	this->SetState(STATE::NONE);
}

void NormalBullet::Update()
{
	//CoroKun���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//NormalTechnique���擾����
	NormalTechnique* Ntch = GameContext::Get<NormalTechnique>();

	//�p�[�e�B�N���̍X�V���s��
	Particle::Update();

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseHero(this);

	//�Q�̍��W����x�N�g�����Z�o����

	//PlayerTarget���ݒ肳��Ă��Ȃ��Ƃ��́A�������Ȃ�
	if (m_PlayerTarget == Vector3::Zero)return;

	Vector3 Len = Length(m_PlayerTarget, this->GetPosition());

	if (Len.x <= 0.0f && this->GetNowState() == ENERGY_CHAGE::ENERGYATACK)
	{
		this->SetState(ITextureBase::STATE::NONE);

		this->SetVelocity(Vector3::Zero);

		this->SetNowState(ENERGY_CHAGE::ENERGYNONE);

		//PlayerTarget������������
		m_PlayerTarget = Vector3::Zero;
	}
}

void NormalBullet::Render()
{
	//�p�[�e�B�N���̕`����s��
	Particle::Render();
}

void NormalBullet::Finalize()
{
}

void NormalBullet::OnCollisionEnter(ITextureBase* obj)
{
	//NormalTechnique���擾����
	NormalTechnique* Ntch = GameContext::Get<NormalTechnique>();

	//�e�ɃL���X�g����
	NormalBullet* normalBullet = dynamic_cast<NormalBullet*>(this);

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	Particle::OnCollisionEnter(obj);

	//���ݑł��Ă���e�ƁA�擪�̒e���ꏏ�̎��A����������
	if (Ntch->GetFrontBullet() == normalBullet)
	{
		//�Q�[�W�̏�����
		Ntch->GetNormalGage()->Initialize();

		//�e�̏�����
		Ntch->GetFrontBullet()->Initialize();

		//SE���~�߂�
		soundManager->Stop(CRI_CUESHEET_0__14_�`���[�W);
	}

	//�`���[�W���ɓ�������
	if (this->GetNowState() == ENERGY_CHAGE::ENERGYNONE)
	{
		//�t���O���N����
		Ntch->SetCheckPush(true);

		//�t���O���N����
		Ntch->SetCheckRelease(true);
	}

	//�`�������
	this->SetState(STATE::NONE);

	//�x�N�g��������������
	this->SetVelocity(Vector3::Zero);

	this->SetNowState(ENERGY_CHAGE::ENERGYNONE);

	//PlayerTarget������������
	m_PlayerTarget = Vector3::Zero;
}

void NormalBullet::NonCollisionEnter(ITextureBase* obj)
{
	Particle::NonCollisionEnter(obj);
}
