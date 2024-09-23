#include "pch.h"
#include "TrackingParticle.h"
#include "GameObject/Technique/VirusBeam/VirusBeam.h"
#include "GameContext.h"
#include "GameObject/Technique/VirusBeam/MainParticle/MainParticle.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TrackingParticle::TrackingParticle(ITextureBase* texture):Particle(texture),m_RandNum(0.0)
{
	//�����}�V�[�����擾����
	Rand* random = Rand::GetInstance();

	//���S�̃p�[�e�B�N�����擾����
	MainParticle* mainParticle = GameContext::Get<VirusBeam>()->GetMainParticle();

	//������ݒ肷��(��x����)
	m_RandNum = random->RandomMachine(1.0, 3.0);
}

void TrackingParticle::Initialize()
{
}

void TrackingParticle::Update()
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

	//���S�̃p�[�e�B�N�����擾����
	MainParticle* mainParticle = virusBeam->GetMainParticle();

	//�^�[�Q�b�g�x�N�g��������������
	Vector3 target = Vector3::Zero;

	//�p�[�e�B�N�����ǔ�����Ƃ�
	if (virusBeam->GetNowState() == ENERGY_CHAGE::TRACKING)
	{
		//���S�̃p�[�e�B�N���Ƃ̋������Z�o����
		target = mainParticle->GetPosition() - this->GetPosition();

		//�����𐳋K������
		target.Normalize();

		//�x�N�g����␳����
		target *= m_RandNum;
	}

	//�x�N�g����ݒ肷��
	this->SetVelocity(this->GetVelocity() + target);

	//�p�[�e�B�N���̍X�V
	Particle::Update();

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseOther(this, "���S�̗��q");
}

void TrackingParticle::Render()
{
	//�p�[�e�B�N���̕`��
	Particle::Render();
}

void TrackingParticle::Finalize()
{
}

void TrackingParticle::OnCollisionEnter(ITextureBase* obj)
{
	Particle::OnCollisionEnter(obj);

	//���S�̃p�[�e�B�N���ɓ��������Ƃ��̏���
	if (obj->GetName() == "���S�̗��q")
	{
		//�E�C���X�r�[�����擾����
		VirusBeam* virusBeam = GameContext::Get<VirusBeam>();

		//���S�̃p�[�e�B�N�����擾����
		MainParticle* mainParticle = virusBeam->GetMainParticle();

		//���͂̃p�[�e�B�N�����ǔ����̎�
		if (virusBeam->GetNowState() == ENERGY_CHAGE::TRACKING)
		{
			//�p�[�e�B�N�����`�悳��Ă���Ƃ�
			if (this->GetState() == STATE::DRAW)
			{
				//�`����~�߂�
				this->SetState(STATE::NONE);
				//�g��{�����擾
				Vector3 scale = mainParticle->GetScale();
				//�{�����グ��
				scale += Vector3(0.08f / (float)mainParticle->GetChageLimit());
				//�g��{����ݒ肷��
				mainParticle->SetScale(scale);
			}
		}
	}
}

void TrackingParticle::NonCollisionEnter(ITextureBase* obj)
{
	Particle::NonCollisionEnter(obj);
}
