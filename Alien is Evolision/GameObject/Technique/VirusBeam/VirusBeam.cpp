#include "pch.h"
#include "VirusBeam.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Player/CoroKun.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "GameObject/Enemy/Normal/NormalEnemy.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

VirusBeam::VirusBeam(ITextureBase* texture):TextureBase(texture),m_MainParticle(nullptr),m_DrawFinal(NULL),
m_DrawLimit(NULL),m_DrawTimeLimit(NULL),m_FrameCount(NULL),m_BeamSpeed(NULL)
{
	//�`���؂�
	this->SetState(STATE::NONE);

	//�`�揇��ύX����
	this->SetDrawOrder(2);

	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�Q�[���p��TextureManager���擾����
	TextureManager* GameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//���q�̃e�N�X�`�����擾����
	ITextureBase* particle = resourceManager->GetTexture("���q");

	//�v���C���[�̃|�C���^���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//GameContext�ɓo�^����
	GameContext::Register<VirusBeam>(this);

	//���S�̃p�[�e�B�N���𐶐�
	std::unique_ptr<MainParticle> pParticle = std::make_unique<MainParticle>(particle);
	pParticle->SetPosition(player->GetPosition() + Vector2(player->AskOrigin().x * 2));
	pParticle->SetScale(Vector3(0.2f));
	pParticle->SetName("���S�̗��q");
	pParticle->SetOrigin(pParticle->AskOrigin());
	pParticle->SetState(STATE::NONE);
	pParticle->SetActive(false);
	pParticle->SetChageLimit(2);
	pParticle->SetScaleLimitCount(3);
	pParticle->SetScaleLimit(1.0f);
	//���|�C���^��o�^����
	m_MainParticle = pParticle.get();
	m_AllParticleList.push_back(m_MainParticle);

	//�Q�[���p��TextureManager�ɒǉ�����
	GameManager->Add(std::move(pParticle));

	//�p�x������������
	float angle = 0.0f;

	//���͂̃p�[�e�B�N�����쐬����
	for (int i = 0; i < OTHER_PARTICLE_NUM; i++)
	{
		//�w�肵���������A�p�[�e�B�N���𐶐�����
		std::unique_ptr<TrackingParticle> pParticle2 = std::make_unique<TrackingParticle>(particle);

		//�������W�͒��S�̃p�[�e�B�N���̍��W�Ƃ���
		pParticle2->SetPosition(m_MainParticle->GetPosition());

		//�x�N�g��������������
		Vector3 vel = Vector3(0.0f, -50.0f, 0.0f);

		//�x�N�g�����X�V
		this->AskCircleVectol(vel, angle);

		//�X�V�����x�N�g�������W�ɉ��Z����
		pParticle2->SetPosition(pParticle2->GetPosition() + vel);
		 
		//�g��{����ύX����
		pParticle2->SetScale(Vector3(0.1f));

		//���O��ύX����
		pParticle2->SetName("����̗��q");

		//���_��ύX����
		pParticle2->SetOrigin(pParticle2->AskOrigin());

		//�`���ύX����
		pParticle2->SetState(STATE::NONE);

		//�X�V�֐��̐ݒ������
		pParticle2->SetActive(false);

		//���X�g�ɐ��|�C���^��ǉ�����
		m_OtherParticleList.push_back(pParticle2.get());
		m_AllParticleList.push_back(pParticle2.get());

		//�Q�[���p��TextureManager�ɒǉ�����
		GameManager->Add(std::move(pParticle2));
	}

	//�؂�o���ʒu���Đݒ肷��
	this->SetRect(RECT{ 0,0,0,50 });
}

void VirusBeam::Initialize()
{
	//Stage���擾����
	Stage* stage = GameContext::Get<Stage>();

	//�}�b�v�����擾����
	std::vector<std::vector<ITextureBase*>>& mapAll = stage->GetMap();

	//�S�Ẵp�[�e�B�N���̐����}�b�v����ǉ�����
	for (Particle* particle : m_AllParticleList)
	{
		//�}�b�v����ǉ�����
		mapAll[mapAll.size() - 1].push_back(particle);
	}
}

void VirusBeam::Update()
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

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�v���C���[�̃|�C���^���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�����G��vector���擾����
	std::vector<HyperEnemy*> hyenemies = GameContext::Get<Stage>()->GetHyperEnemies();

	//������Ƌ����G��vector���擾����
	std::vector<SuperEnemy*> superenemies = GameContext::Get<Stage>()->GetSuperEnemies();

	//���ʂ̓G��vector���擾����
	std::vector<NormalEnemy*> normalenemies = GameContext::Get<Stage>()->GetNormalEnemies();

	//�r�[���U�����̏���
	if (this->GetNowState() == ENERGY_CHAGE::ENERGYATACK)
	{
		//�`�悷��
		this->SetState(STATE::DRAW);

		for (HyperEnemy* hyene : hyenemies)
		{
			//�G�̃V���{���ʍU�����擾����
			std::vector<SoapBabble*> highbabble = hyene->GetBabbleList();
			//�G��HP��0���傫���Ƃ��ɓ����蔻���ǉ�����
			if (hyene->GetHP() > NULL)
			{
				//�����蔻���ǉ�����
				GameContext::Get<CollisionManager>()->AddUseOther(hyene, this, (float)m_DrawFinal / 2.0f + 100.0f);
				for (SoapBabble* babble : highbabble)
				{
					//�����蔻���ǉ�����
					GameContext::Get<CollisionManager>()->AddUseOther(babble, this, (float)m_DrawFinal / 2.0f + 100.0f);
				}
			}
		}

		for (SuperEnemy* super : superenemies)
		{
			//�Œ�C����擾����
			for (std::unique_ptr<FixedTurret>& turret : super->GetTurretList())
			{
				//�G��HP��0���傫���Ƃ��ɓ����蔻���ǉ�����
				if (super->GetHP() > NULL)
				{
					//�����蔻���ǉ�����
					GameContext::Get<CollisionManager>()->AddUseOther(super, this, (float)m_DrawFinal / 2.0f + 100.0f);
					for (SoapBabble* babble : turret->GetBabbleList())
					{
						//�����蔻���ǉ�����
						GameContext::Get<CollisionManager>()->AddUseOther(babble, this, (float)m_DrawFinal / 2.0f + 100.0f);
					}
				}
			}
		}

		for (NormalEnemy* normal : normalenemies)
		{
			//�G��HP��0���傫���Ƃ��ɓ����蔻���ǉ�����
			if (normal->GetHP() > NULL)
			{
				//�����蔻���ǉ�����
				GameContext::Get<CollisionManager>()->AddUseOther(normal, this, (float)m_DrawFinal / 2.0f + 100.0f);
			}
		}

		//�������Ԃ𒴂�����A�r�[�����~�߂�
		if (m_FrameCount > m_DrawTimeLimit)
		{
			//�`����~�߂�
			this->SetState(STATE::NONE);

			//�r�[��������������
			this->InitBeam();

			//�X�e�[�g��؂�ւ���
			this->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//�ړ���~���Ă���Ƃ��ɁA�ړ����ĊJ������
			if (!player->GetWalkState()->GetExecute())
			{
				player->GetWalkState()->SetExecute(true);
			}
			//�X�V�֐��𗎂Ƃ�
			this->SetActive(false);

			//SE���~�߂�
			soundManager->Stop(CRI_CUESHEET_0__15_�r�[��);
		}

		//���݂̏I�_�����E�l�����̎��͏I�_���X�V����
		if (m_DrawFinal < m_DrawLimit)
		{
			m_DrawFinal += m_BeamSpeed;
		}
		m_FrameCount++;

		//�������Ƀr�[����ł��Ă���Ƃ��́A�摜�̌��_��ύX����
		if (this->GetSpriteEffect() == SpriteEffects::SpriteEffects_FlipHorizontally)
		{
			//���_��ݒ肷��
			this->SetOrigin(Vector2(m_DrawFinal, 0.0f));
		}
		//�E�����Ƀr�[����ł��Ă���Ƃ��́A�ύX���Ȃ�
		else
		{
			//���_������������
			this->SetOrigin(Vector2::Zero);
		}

	}

	//�摜�̐؂�o���ʒu���X�V����
	this->SetRect(RECT{ 0,0,m_DrawFinal,50 });

	//�摜�̌��_��ݒ肷��
	this->SetOrigin(this->GetOrigin());

	//�p�[�e�B�N���̍X�V����
	this->UpdateParticle();
	
	//�摜�̍X�V����
	TextureBase::Update();

	//�����蔻��̍��W��ݒ肷��
	this->SetCenter(this->GetPosition());

	//�����蔻��̔��a��ݒ肷��
	this->SetRadius(Vector3(this->AskOrigin()));

	//�����蔻��̒ǉ�
	GameContext::Get<CollisionManager>()->AddUseHero(this);
}

void VirusBeam::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void VirusBeam::Finalize()
{
}

void VirusBeam::OnCollisionEnter(ITextureBase* obj)
{
	CollisionBase::OnCollisionEnter(obj);
}

void VirusBeam::NonCollisionEnter(ITextureBase* obj)
{
	CollisionBase::NonCollisionEnter(obj);
}

void VirusBeam::WithPlayer(ITextureBase* texture)
{
	//�v���C���[�Ɠ����������s��

	//�摜�������o��
	TextureBase::Extrude(texture);

	//���S�̃p�[�e�B�N���̏���
	m_MainParticle->Extrude(texture);

	//���S�ɏW�܂��Ă���p�[�e�B�N���̏���
	for (TrackingParticle*& particle : m_OtherParticleList)
	{
		particle->Extrude(texture);
	}

}

void VirusBeam::InitBeam()
{
	//�r�[���̏�����������

	//�t���[����������������
	m_FrameCount = NULL;

	//�I�_������������
	m_DrawFinal = NULL;

	//�؂�o���ʒu������������
	this->SetRect(RECT{ 0,0,0,0 });

	
}

void VirusBeam::UpdateParticle()
{
	//�p�[�e�B�N���̍X�V����
	
	//�x�N�g����ݒ肷��
	for (Particle*& particle : m_AllParticleList)
	{
		particle->SetVelocity(this->GetVelocity());
	}
}

void VirusBeam::AskCircleVectol(Vector3& vel,float& angle)
{
	//��]�s����쐬����i�X�N���[�����W�ł́AZ���݂̂̉�]�Ƃ���j
	Matrix rot = Matrix::CreateRotationZ(XMConvertToRadians(angle));

	//�x�N�g������]������
	vel = Vector3::Transform(vel, rot);

	//�p�x���X�V����
	angle += 36.0f;
}

