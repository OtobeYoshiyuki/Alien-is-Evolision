#include "pch.h"
#include "Experience.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�G�����ł��Ă���A�o���l�������o���܂ł̑҂�����
const float Experience::WAIT_TIME = 1.0f;

Experience::Experience(ITextureBase* texture) :
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_tracking(nullptr),//�ǔ��p�̃X�e�[�g
	m_homing(nullptr),//�z�[�~���O�p�̃X�e�[�g
	m_timer(0.0f)//�o�ߎ���
{
	//�ǐ՗p�̃X�e�[�g�𐶐�����
	m_tracking = std::make_unique<TrackingState>();

	//�X�e�[�g��؂�
	m_tracking->SetExecute(false);

	//�z�[�~���O�p�̃X�e�[�^�X�𐶐�����
	m_homing = std::make_unique<HomingState>();

	//�X�e�[�g��؂�
	m_homing->SetExecute(false);

	//�p�����[�^�[�̏�����������
	m_homing->SetMaxVel(Vector3(0.0f, -300.0f, 0.0f));
	m_homing->SetMinVel(Vector3::Zero);
	m_homing->SetDirVel(Vector3(0.0f, -5.0f, 0.0f));

	//���X�g�ɒǉ�����
	m_stateList.push_back(m_tracking.get());
	m_stateList.push_back(m_homing.get());
}

void Experience::Initialize()
{
}

void Experience::Update()
{
	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

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
		GameContext::Get<CollisionManager>()->AddUseOther(this,player, 200.0f);
	}
}

void Experience::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void Experience::Finalize()
{
}

void Experience::OnCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::OnCollisionEnter(obj);

	//�v���C���[�Ɠ����������̏���
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);
}

void Experience::NonCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::NonCollisionEnter(obj);
}

void Experience::UpdateState()
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

void Experience::State_Wait()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//elapsedTime���擾����
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//���Ԃ��o�߂�����
	if (m_timer > WAIT_TIME)
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(TARGET_STATE::TRACKING);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__32_�A�C�e���ǐ�);

		//���Ԃ�����������
		m_timer = 0.0f;

		//���Ԃ��X�V���Ȃ�
		return;
	}

	//���Ԃ��X�V����
	m_timer += elapsedTime;
}

void Experience::State_Tracking()
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

void Experience::OnCollision_Player(ITextureBase* player)
{
	//���N���X����A�h���N���X�ɃL���X�g����
	CoroKun* coro = dynamic_cast<CoroKun*>(player);

	//���L�ҁiEnemy�j�̃X�e�[�^�X���擾����
	IStatusBase* enemyStatus = this->GetOwner()->GetStatus();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�o���l���ǔ����̎�
	if (this->GetNowState() == TARGET_STATE::TRACKING)
	{
		//�`���؂�
		this->SetState(STATE::NONE);

		//�x�N�g��������������
		this->SetVelocity(Vector3::Zero);

		//�X�e�[�g��؂�ւ���
		this->SetNowState(TARGET_STATE::CHANGE);

		//�o���l���擾���Ă��Ȃ��ꍇ
		if (!coro->GetExperienceGage()->CheckExp())
		{
			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__33_�Q�[�W�㏸);
		}

		//�ϐ��̏�����
		int Exp, texExp;
		float ratio;

		//�擾�o���l�����߂�
		StatusCalc::CalcExp(enemyStatus->GetLevel(), enemyStatus->GetCompensation(), 50,
			coro->GetExperienceGage()->GetMaxTexLength(),coro->GetMaxNextLevelPoint(),Exp,texExp,ratio);

		//�o���l��ݒ肷��(�摜�p)
		coro->GetExperienceGage()->PushExp(texExp);
		coro->GetExperienceGage()->SetRatio(ratio);

		//�o���l��ݒ肷��(�v���C���[�p)
		coro->SetNextLevelPoint(coro->GetNextLevelPoint() + Exp);

		//�o���l�Q�[�W�̃X�e�[�g��ݒ肷��
		coro->GetExperienceGage()->SetNowState(CALC::CALCING);
	}
}
