#include "pch.h"
#include "MainParticle.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Technique/VirusBeam/VirusBeam.h"
#include "GameObject/Technique/VirusBeam/TrackingParticle/TrackingParticle.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//1�t���[���ɃX�P�[���ɉ��Z���鑬�x
const Vector3 MainParticle::SCALESPEED = Vector3(0.1f);

MainParticle::MainParticle(ITextureBase* texture):Particle(texture),m_ChageCount(NULL),m_LimitChage(NULL),
m_ChageFinish(false),m_ScaleCount(NULL),m_ScaleLimitCount(NULL),m_ScaleLimit(0.0f),m_ScaleVel(Vector3::Zero)
{
}

void MainParticle::Initialize()
{
	//�I���t���O������������
	m_ChageFinish = false;

	//�`���[�W�J�E���g������������
	m_ChageCount = NULL;

	//�g��{��������������
	this->SetScale(Vector3(0.2f));
}

void MainParticle::Update()
{
	//UI�̕\�����I���܂ŉ������Ȃ�
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//�E�C���X�r�[�����擾����
	VirusBeam* virusBeam = GameContext::Get<VirusBeam>();

	//�ǔ��p�̃p�[�e�B�N���̃��X�g���擾����
	std::vector<TrackingParticle*> TrackingList = virusBeam->GetParticleList();

	//�p�[�e�B�N���̍X�V
	Particle::Update();

	//�ϐ�������������
	int NoDrawNum = NULL;

	//�`�悪�؂��Ă��鐔�𐔂���
	for (TrackingParticle*& particle : TrackingList)
	{
		if (particle->GetState() == STATE::NONE)
		{
			NoDrawNum++;
		}
	}

	//�X�e�[�g�ɂ���āA������؂�ւ���
	switch (virusBeam->GetNowState())
	{
	case ENERGY_CHAGE::TRACKING:
		this->State_Tracking(NoDrawNum);
		break;
	case ENERGY_CHAGE::ENERGYUP:
		this->State_PowerUp(NoDrawNum);
		break;
	case ENERGY_CHAGE::ENERGYMAX:
		this->State_Max();
		break;
	}

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseHero(this);
}

void MainParticle::Render()
{
	//�p�[�e�B�N���̕`��
	Particle::Render();
}

void MainParticle::Finalize()
{
}

void MainParticle::OnCollisionEnter(ITextureBase* obj)
{
	Particle::OnCollisionEnter(obj);
}

void MainParticle::NonCollisionEnter(ITextureBase* obj)
{
	Particle::NonCollisionEnter(obj);
}

void MainParticle::State_Tracking(const int& NoDrawNum)
{
	//���͂̃p�[�e�B�N�����ǔ����Ă���Ƃ��̏���

	//�E�C���X�r�[�����擾����
	VirusBeam* virusBeam = GameContext::Get<VirusBeam>();

	//�ǔ��p�̃p�[�e�B�N���̃��X�g���擾����
	std::vector<TrackingParticle*> TrackingList = virusBeam->GetParticleList();

	//�S�Ă̕`�悪�؂��Ă���
	if (NoDrawNum >= (int)TrackingList.size())
	{
		//�X�e�[�g��ύX����
		virusBeam->SetNowState(ENERGY_CHAGE::ENERGYUP);

		//���E�l�ɒB���Ă��Ȃ���΁A�`���[�W�����X�V����
		if (m_ChageCount < m_LimitChage)
		{
			m_ChageCount++;
		}
	}
}

void MainParticle::State_PowerUp(const int& NoDrawNum)
{
	//���S�̃p�[�e�B�N�����傫���Ȃ鏈��

	//�E�C���X�r�[�����擾����
	VirusBeam* virusBeam = GameContext::Get<VirusBeam>();

	//�ǔ��p�̃p�[�e�B�N���̃��X�g���擾����
	std::vector<TrackingParticle*> TrackingList = virusBeam->GetParticleList();

	//�S�Ă̕`�悪�؂��Ă�����A�`���[�W�����X�V����
	if (NoDrawNum >= (int)TrackingList.size())
	{
		//���łɌ��E�l�ɒB���Ă�����A�������Ȃ�
		if (m_ChageCount >= m_LimitChage)
		{
			//�`���[�W�����t���O���I���ɂ���
			m_ChageFinish = true;
			//�J�E���g������������
			m_ChageCount = NULL;
			//�X�e�[�g��؂�ւ���
			virusBeam->SetNowState(ENERGY_CHAGE::ENERGYMAX);
			//�`�������
			for (TrackingParticle* particle : TrackingList)
			{
				//�`����I�t�ɂ���
				particle->SetState(STATE::NONE);
			}
		}
		//���E�l�ɒB���Ă��Ȃ���΁A�X�V����
		else
		{
			//�ĂсA�`�悷��
			for (TrackingParticle* particle : TrackingList)
			{
				//�`����I���ɂ���
				particle->SetState(STATE::DRAW);
			}
			//�X�e�[�g��؂�ւ���
			virusBeam->SetNowState(ENERGY_CHAGE::TRACKING);
			//���W���Đݒ肷��
			virusBeam->SetParticlesPos(this->GetPosition());
		}
	}
}

void MainParticle::State_Max()
{
	//�G�l���M�[��MAX�̎��̏���

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�E�C���X�r�[�����擾����
	VirusBeam* virusBeam = GameContext::Get<VirusBeam>();

	//���݂̔{�����擾����
	Vector3 scale = this->GetScale();

	//�����Switch���ŏ����𕪂���
	switch (this->GetNowState())
	{
	case SCALE::SNONE:
		//�J�E���g���X�V����
		m_ScaleCount++;
		//�X�e�[�g��؂�ւ���
		this->SetNowState(SCALE::EXPANSION);
		break;
	case SCALE::EXPANSION:
		//�g��̏���

		//�g��p�����[�^�[�̍X�V
		if (m_ScaleVel.x < m_ScaleLimit)
		{
			scale += SCALESPEED;
			m_ScaleVel += SCALESPEED;
		}
		//�X�e�[�g��؂�ւ���
		else
		{
			//�X�P�[���x�N�g��������������
			m_ScaleVel = Vector3::Zero;
			//�X�e�[�g��؂�ւ���
			this->SetNowState(SCALE::SHRINK);
		}
		break;
	case SCALE::SHRINK:
		//�k���̏���

		//�k���p�����[�^�[�̍X�V
		if (m_ScaleVel.x < m_ScaleLimit)
		{
			scale -= SCALESPEED;
			m_ScaleVel += SCALESPEED;
		}
		else
		{
			//�w��񐔈ȏ�g��E�k�����s������A���̃X�e�[�g�Ɉڍs����
			if (m_ScaleCount >= m_ScaleLimitCount)
			{
				//�X�e�[�g�̐؂�ւ�
				virusBeam->SetNowState(ENERGY_CHAGE::ENERGYATACK);
				//�X�e�[�g�̐؂�ւ�
				this->SetNowState(SCALE::SNONE);
				//�`�������
				this->SetState(STATE::NONE);
				//�X�P�[���x�N�g��������������
				m_ScaleVel = Vector3::Zero;
				//�g��{��������������
				scale = Vector3(0.2f);
				//�X�P�[���J�E���g������������
				m_ScaleCount = NULL;
				//SE���Đ�����
				soundManager->Play(CRI_CUESHEET_0__15_�r�[��);
			}
			//���[�v
			else
			{
				//�X�e�[�g�̐؂�ւ�
				this->SetNowState(SCALE::SNONE);
				//�X�P�[���x�N�g��������������
				m_ScaleVel = Vector3::Zero;
			}
		}
		break;
	}

	//�{�����Đݒ肷��
	this->SetScale(scale);
}
