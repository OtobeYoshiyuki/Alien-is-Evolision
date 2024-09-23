#include "pch.h"
#include "FixedTurret.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "OtobeLib/FrameWork/Collision/Collision.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//��������
const float FixedTurret::SURVIVAL_TIME = 10.0f;

FixedTurret::FixedTurret():
	m_parentBabble(nullptr),//�e�̖A
	m_JumpState(nullptr),//�W�����v�X�e�[�g
	m_AutoMoveState(nullptr),//�����ړ��̃X�e�[�g
	m_SurvivalTime(0.0f),//��������
	m_bulletFiring(false)//�e�����˂��ꂽ�t���O
{
}

void FixedTurret::Initialize()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* shabondama1 = resourceManager->GetTexture("�V���{����1");

	//GameManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//Stage���擾����
	Stage* stage = GameContext::Get<Stage>();

	//�}�b�v�����擾����
	std::vector<std::vector<ITextureBase*>>& mapAll = stage->GetMap();

	//�e�̖A�𐶐�����
	std::unique_ptr<SoapBabble> pbabble = std::make_unique<SoapBabble>(shabondama1);

	//���L�҂̃|�C���^��ݒ肷��
	pbabble->SetOwner(this->GetOwner());

	//�e�̖A�̊g��{����ύX����
	pbabble->SetScale(Vector3(3.0f,3.0f,0.0f));

	//�����蔻��̑傫���͉摜��2�{�̑傫���Ƃ���
	pbabble->SetRadius(Vector3(pbabble->AskOrigin()));

	//���W��ݒ肷��
	pbabble->SetPosition(this->GetOwner()->GetPosition());

	//�x�N�g��������������
	pbabble->SetVelocity(Vector3::Zero);
	pbabble->SetNowState(Vector3::Zero);

	//���O��ύX����
	pbabble->SetName("Supersyabondama");

	//���_��ݒ肷��
	pbabble->SetOrigin(pbabble->AskOrigin());

	//�ϐ��ɐ��|�C���^��o�^����
	m_parentBabble = pbabble.get();

	//���X�g�ɐ��|�C���^��ǉ�����
	m_allBabble.push_back(pbabble.get());

	//�}�b�v����ǉ�����
	mapAll[mapAll.size() - 1].push_back(pbabble.get());

	//GameManager�ɒǉ�����
	gameManager->Add(std::move(pbabble));

	for (int i = 0; i < CHILD_NUM; i++)
	{
		//�q���̖A�𐶐�����
		std::unique_ptr<SoapBabble> pchild = std::make_unique<SoapBabble>(shabondama1);

		//���L�҂̃|�C���^��ݒ肷��
		pchild->SetOwner(this->GetOwner());

		//���W��ݒ肷��
		pchild->SetPosition(this->GetOwner()->GetPosition());

		//���O��ύX����
		pchild->SetName("Supersyabondama");

		//�x�N�g��������������
		pchild->SetVelocity(Vector3::Zero);
		pchild->SetNowState(Vector3::Zero);

		//���_��ݒ肷��
		pchild->SetOrigin(pchild->AskOrigin());

		//���X�g�ɐ��|�C���^��ǉ�����
		m_allBabble.push_back(pchild.get());
		m_childBabbles.push_back(pchild.get());
		//�}�b�v����ǉ�����
		mapAll[mapAll.size() - 1].push_back(pchild.get());

		//GameManager�ɒǉ�����
		gameManager->Add(std::move(pchild));
	}
	
	//AutoMoveState�𐶐�
	m_AutoMoveState = std::make_unique<AutoMoveState>();
	m_AutoMoveState->SetExecute(false);
	m_AutoMoveState->SetSpeed(Vector3(4.0f, 0.0f, 0.0f));
	m_AutoMoveState->SetNowState(DIRECT::LEFT);

	//JumpState�𐶐�
	m_JumpState = std::make_unique<JumpState>();
	m_JumpState->SetExecute(false);
	m_JumpState->SetJumpLimit(200.0f);
	m_JumpState->SetJumpSpeed(4.0f);
	m_JumpState->SetCorrection(0.4f);
	m_JumpState->SetJumpLimitCount(1);
	m_JumpState->SetJumpOption(true);

	//���X�g�ɃX�e�[�g��ǉ�����
	m_StateList.push_back(m_AutoMoveState.get());
	m_StateList.push_back(m_JumpState.get());
}

void FixedTurret::Update()
{
	//�X�e�[�g�̍X�V����
	this->UpdateState();

	//�A�̍X�V����
	this->UpdateBabble();
}

void FixedTurret::RandShake(SoapBabble* child)
{
	//RandMachine���擾����
	Rand* random = Rand::GetInstance();

	//X�̗�����U��
	double x = random->RandomMachine(-3.0, 3.0);

	//Y�̗�����U��
	double y = random->RandomMachine(-3.0, 3.0);

	//�x�N�g����ݒ肷��
	child->SetNowState(Vector3((float)x, (float)y, 0.0f));
}

void FixedTurret::InitTurret()
{
	//�������Ԃ�����������
	m_SurvivalTime = 0.0f;

	//�t���O�𗎂Ƃ�
	m_bulletFiring = false;

	for (SoapBabble* babble : m_allBabble)
	{
		//�A������
		babble->SetState(ITextureBase::STATE::NONE);

		//����ւ��p�̃x�N�g��������������
		babble->SetNowState(Vector3::Zero);
	}
}

void FixedTurret::UpdateState()
{
	//�x�N�g���̏�����
	Vector3 vel = Vector3::Zero;

	for (MoveState* state : m_StateList)
	{
		//���s���̃x�N�g�����擾����
		vel += state->Update();
	}

	//�e����q�܂őS�ē�����
	for (SoapBabble* babble : m_allBabble)
	{
		//����ւ��p�̃x�N�g�����擾����
		Vector3 temp = babble->GetNowState();

		//�x�N�g����ݒ肷��
		babble->SetVelocity(vel + temp);
	}
}

void FixedTurret::UpdateBabble()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�`�悳��Ă��鐔
	int drawNum = NULL;

	//�A��S�����˂��Ă���Ƃ�
	for (SoapBabble* child : m_childBabbles)
	{
		//�`�悳��Ă���
		if (child->GetState() == ITextureBase::STATE::DRAW)drawNum++;
	}

	//�S�e���˂��Ă�����A�t���O���N��������(�t���O���N�����Ă��Ȃ��Ƃ��̂�)
	if (drawNum == (int)m_childBabbles.size() && !m_bulletFiring)m_bulletFiring = true;

	//�e��S�����˂��Ă�����A
	if (m_bulletFiring)
	{
		//�v�l���Ԃ��o�߂�����A���ɖ߂�
		if (m_SurvivalTime > SURVIVAL_TIME)
		{
			//��������������
			this->InitTurret();

			//�v�l���Ԃ̍X�V���s��Ȃ�
			return;
		}

		//�v�l���Ԃ��X�V����
		m_SurvivalTime += staticData->Get_FloatData("elapsedTime");
	}
}
