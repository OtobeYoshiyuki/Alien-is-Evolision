#include "pch.h"
#include "TechniqueRecovery.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "Effect/MasicEffectManager/MasicEffectManager.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TechniqueRecovery::TechniqueRecovery(ITextureBase* texture):
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_tracking(nullptr),//�ǐ՗p�̃X�e�[�g
	m_homing(nullptr),//�z�[�~���O�p�̃X�e�[�g
	m_timer(0.0f),//����
	m_waitTimer(0.0f),//�҂�����
	m_masicPower(NULL)//�񕜂�����l
{
	//�ǐ՗p�̃X�e�[�g�𐶐�����
	m_tracking = std::make_unique<TrackingState>();

	//�X�e�[�g��؂�
	m_tracking->SetExecute(false);

	//�z�[�~���O�p�̃X�e�[�g�𐶐�����
	m_homing = std::make_unique<HomingState>();

	//�X�e�[�g��؂�
	m_homing->SetExecute(false);

	//�p�����[�^�[�̏�����������
	m_homing->SetMaxVel(Vector3(0.0f, -300.0f, 0.0f));
	m_homing->SetMinVel(Vector3::Zero);
	m_homing->SetScalar(Vector3(6.0f));

	//���X�g�ɒǉ�����
	m_stateList.push_back(m_tracking.get());
	m_stateList.push_back(m_homing.get());
}

void TechniqueRecovery::Initialize()
{
}

void TechniqueRecovery::Update()
{
	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�X�e�[�g�̍X�V����
	this->UpdateState();

	//�摜�̍X�V����
	TextureBase::Update();

	//�v���C���[�ƌo���l�̓����蔻���ݒ肷��
	if (this->GetNowState() == TARGET_STATE::TRACKING)
	{
		//�����蔻���ݒ肷��
		GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);
	}
}

void TechniqueRecovery::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void TechniqueRecovery::Finalize()
{
}

void TechniqueRecovery::OnCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::OnCollisionEnter(obj);

	//�v���C���[�Ɠ����������̏���
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);
}

void TechniqueRecovery::NonCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::NonCollisionEnter(obj);
}

void TechniqueRecovery::UpdateState()
{
	//switch���ŏ����𕪂���
	switch (this->GetNowState())
	{
	case TARGET_STATE::WAIT:
		this->State_Wait();
		break;
	case TARGET_STATE::TRACKING:
		this->State_Tracking();
		break;
	}
}

void TechniqueRecovery::State_Wait()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//elapsedTime���擾����
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//���Ԃ��o�߂�����
	if (m_timer > m_waitTimer)
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(TARGET_STATE::TRACKING);

		//���Ԃ�����������
		m_timer = 0.0f;

		//���Ԃ��X�V���Ȃ�
		return;
	}

	//���Ԃ��X�V����
	m_timer += elapsedTime;
}

void TechniqueRecovery::State_Tracking()
{
	//�x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//���s���̃x�N�g�������Z���Ă���
	for (MoveState* state : m_stateList)
	{
		vel += state->Update();
	}

	//�x�N�g����ݒ肷��
	this->SetVelocity(vel);
}

void TechniqueRecovery::OnCollision_Player(ITextureBase* player)
{
	//���N���X����A�h���N���X�ɃL���X�g����
	CoroKun* coro = dynamic_cast<CoroKun*>(player);

	//�o���l���ǔ����̎�
	if (this->GetNowState() == TARGET_STATE::TRACKING)
	{
		//�`���؂�
		this->SetState(STATE::NONE);

		//�x�N�g��������������
		this->SetVelocity(Vector3::Zero);

		//�X�e�[�g��؂�ւ���
		this->SetNowState(TARGET_STATE::CHANGE);

		//�񕜂�����Ώۂ̃X�e�[�^�X��錾
		int status = NULL;

		//CommandRecovery�̎�
		if (this->GetName() == MasicEffectManager::EFFECT_NAME_COMMAND)
		{
			//�Ώۂ̃X�e�[�^�X���擾����
			status = coro->GetMP();

			//�ΏۃQ�[�W��RECT���擾����
			RECT rect = coro->GetTechniqueGage()->GetRect();

			//�Q�[�W���񕜂�����
			StatusCalc::CalcHeal(status, m_masicPower, coro->GetTechniqueGage()->GetMaxTexLength(),coro->GetMaxMP(), rect.right);

			//MP��ݒ肷��
			coro->SetMP(status);

			//RECT��ݒ肷��
			coro->GetTechniqueGage()->SetRect(rect);
		}
		//SpecialRecovery�̎�(�F����)
		if (this->GetName() == MasicEffectManager::EFFECT_NAME_SPECIAL)
		{
			//�Ώۂ̃X�e�[�^�X���擾����
			status = coro->GetSpecial();

			//�ΏۃQ�[�W��RECT���擾����
			RECT rect = coro->GetDeathblowGage()->GetRect();

			//�Q�[�W���񕜂�����
			StatusCalc::CalcHeal(status, m_masicPower, coro->GetDeathblowGage()->GetMaxTexLength(), coro->GetMaxSpecialPoint(), rect.right);

			//MP��ݒ肷��
			coro->SetSpecial(status);

			//RECT��ݒ肷��
			coro->GetDeathblowGage()->SetRect(rect);
		}
	}
}
