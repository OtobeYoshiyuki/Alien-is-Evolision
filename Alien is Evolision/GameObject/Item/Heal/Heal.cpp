#include "pch.h"
#include "Heal.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//1�t���[���ɉ��Z���鑬�x
const float	Heal::JUMP_SPEED = 5.0f;

//�W�����v�̌��E�l(�ŏ�)
const float	Heal::JUMP_LIMIT_FIRST = 50.0f;

//�����ʒu
const Vector3 Heal::INIT_POS = Vector3(0.0f, -200.0f, 0.0f);

//�����{��
const Vector3 Heal::INIT_SCALE = Vector3(0.1f);

//�㏸����A�����܂ł̕␳�l
const float	Heal::CORRECTION = 2.0f;

//�o�E���h�W��
const float	Heal::BOUND_POWER = 5.0f;

Heal::Heal(ITextureBase* texture):
	Item(texture),//���N���X�ɏ��𑗐M����
	m_jumpState(nullptr),//JumpState
	m_healPower(0.0f)//�񕜗�
{
	//JumpState�𐶐�����
	m_jumpState = std::make_unique<JumpState>();
	m_jumpState->SetExecute(false);
	m_jumpState->SetJumpSpeed(JUMP_SPEED);
	m_jumpState->SetJumpLimit(JUMP_LIMIT_FIRST);
	m_jumpState->SetJumpLimitCount(JUMP_COUNT);
	m_jumpState->SetCorrection(CORRECTION);
	m_jumpState->SetJumpOption(false);

	//MoveState�ɒǉ�����
	m_stateList.push_back(m_jumpState.get());
}

void Heal::Initialize()
{
}

void Heal::Update()
{
	//UI�̕\�����I���܂ŉ������Ȃ�
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//CollisionManager���擾����
	CollisionManager* collsionManager = GameContext::Get<CollisionManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//�X�e�[�g�̍X�V����
	this->UpdateState();

	//�摜�̍X�V����
	Item::Update();

	//�����蔻���ǉ�����
	collsionManager->AddUseHero(this);

	//�X�e�[�g�����s���ŁA���n�����Ƃ�
	//�o�E���h���̎�
	if (m_jumpState->GetExecute() && this->GetNowState() == BOUND::BOUND)
	{
		//���n
		if (m_jumpState->GetNowState() == JUMP::LANDING)
		{
			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__34_�o�E���h);
		}
	}
}

void Heal::Render()
{
	//�摜�̕`�揈��
	Item::Render();
}

void Heal::Finalize()
{
}

void Heal::OnCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	Item::OnCollisionEnter(obj);

	//Player�ɓ��������Ƃ�
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);

	//Floor�ɓ��������Ƃ�
	if (obj->GetName() == "floor")this->OnCollision_Floor(obj);

	//Wall�ɓ��������Ƃ�
	if (obj->GetName() == "wall")this->OnCollision_Wall(obj);

}

void Heal::NonCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	Item::NonCollisionEnter(obj);
}

void Heal::OnFall(const Vector3& pos)
{
	//�X�e�[�g��؂�ւ���
	this->SetNowState(BOUND::FALL);

	//���W��ݒ肷��
	this->SetPosition(pos);

	//���s�֐����N����
	m_jumpState->SetExecute(true);

	//�X�e�[�g��؂�ւ���
	m_jumpState->SetNowState(JUMP::FALL);

	//�`����N����
	this->SetState(STATE::DRAW);

	//���X�g�ɒl��ǉ�����
	m_jumpState->AddFall();
}

void Heal::UpdateState()
{
	//�x�N�g���̏�����
	Vector3 vel = Vector3::Zero;

	//���s���̃X�e�[�g����x�N�g�����擾����
	for (MoveState* state : m_stateList)
	{
		vel += state->Update();
	}

	//�x�N�g����ݒ肷��
	this->SetVelocity(vel);

	//Switch���ŏ����𕪂���
	switch (this->GetNowState())
	{
	case BOUND::BOUND:
		this->State_Bound();
		break;
	case BOUND::RESULT:
		this->State_Result();
		break;
	}

}

void Heal::State_Bound()
{
	//�w�肵���񐔃W�����v����
	if (m_jumpState->GetJumpmDesignation())
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(BOUND::RESULT);

		//���s�֐��𗎂Ƃ�
		m_jumpState->SetExecute(false);
	}
}

void Heal::State_Result()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SE���~����
	soundManager->Stop(CRI_CUESHEET_0__35_����);

	//���s�֐��𗎂Ƃ�
	m_jumpState->SetExecute(false);

	//�X�e�[�g��؂�ւ���
	this->SetNowState(BOUND::NONE);
}

void Heal::OnCollision_Player(ITextureBase* obj)
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SE���Đ�����
	soundManager->Play(CRI_CUESHEET_0__22_�l��);

	//�摜�������o��
	this->Extrude(this);

	//�擾�����A�C�e����ǉ�����
	GameContext::Get<PlayScene>()->GetHealManager()->AddItem(this);

	//�`���؂�
	this->SetState(STATE::NONE);

	//�X�e�[�g��؂�ւ���
	this->SetNowState(BOUND::RESULT);
}

void Heal::OnCollision_Floor(ITextureBase* obj)
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�摜�������o��
	this->Extrude(this);
	
	//�������̎��́A�W�����v������
	if (this->GetNowState() == BOUND::FALL)
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(BOUND::BOUND);

		//�X�e�[�g��؂�ւ���
		m_jumpState->SetNowState(JUMP::LANDING);

		//���X�g����ɂ���
		m_jumpState->RemoveFall();

		//�W�����v�I�v�V������ݒ肷��
		m_jumpState->SetJumpOption(true);

		//�o�E���h����l��ݒ肷��
		m_jumpState->SetBoundPower(BOUND_POWER);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__34_�o�E���h);
	}
}

void Heal::OnCollision_Wall(ITextureBase* obj)
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�摜�������o��
	this->Extrude(this);

	//�������̎��́A�W�����v������
	if (this->GetNowState() == BOUND::FALL)
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(BOUND::BOUND);

		//�X�e�[�g��؂�ւ���
		m_jumpState->SetNowState(JUMP::LANDING);

		//���X�g����ɂ���
		m_jumpState->RemoveFall();

		//�W�����v�I�v�V������ݒ肷��
		m_jumpState->SetJumpOption(true);

		//�o�E���h����l��ݒ肷��
		m_jumpState->SetBoundPower(BOUND_POWER);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__34_�o�E���h);
	}
}
