#include "pch.h"
#include "TargetScrollCamera.h"
#include "GameObject/GameCamera/GameCamera.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Stage/Maptip/HoleFloor/HoleFloor.h"
#include "GameObject/Stage/Stage.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�Ώە��Ɉړ�����ړ���
const float TargetScrollCamera::TARGET_TO_VECTOL = 0.05f;

//�v���C���[�ƑΏە��Ƃ̋���
const float TargetScrollCamera::TARGET_TO_LENGTH = 450.0f;

TargetScrollCamera::TargetScrollCamera():
	m_target(Vector3::Zero),//�X�N���[��������ʒu
	m_targetNumber(NULL)//�ǐՂ���e�N�X�`��
{
}

void TargetScrollCamera::Initialize()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�^�[�Q�b�g�X�N���[���J������Register�ɓo�^����
	GameContext::Register<TargetScrollCamera>(this);

	//�Q�[���J�����̏����󂯎��
	GameCamera* gameCamera = GameContext::Get<GameCamera>();

	//���W���󂯎��
	this->SetPosition(gameCamera->GetPosition());

	//�s����󂯎��
	this->SetMatrix(gameCamera->GetMatrix());

	//�I�u�W�F�N�g�̈ړ����~�߂�
	this->SetObjectMove(false);

	//�^�[�Q�b�g��ݒ肷��
	this->SetTarget(staticData->Get_Vector3("�^�[�Q�b�g"));

	//�^�[�Q�b�g�i���o�[��ݒ肷��
	this->SetTargetNumber(staticData->Get_IntData("�^�[�Q�b�g"));

	//�X�e�[�g��ݒ肷��
	this->SetNowState(TARGET_STATE::TRACKING);
}

void TargetScrollCamera::Update()
{
	//�X�e�[�g�̍X�V
	this->UpdateState();
}

void TargetScrollCamera::Finalize()
{
}

void TargetScrollCamera::UpdateState()
{
	//�X�e�[�g�̍X�V����

	//Switch���ŏ����𕪂���
	switch (this->GetNowState())
	{
	case TARGET_STATE::TRACKING:
		this->State_Tracking();
		break;
	case TARGET_STATE::WAIT:
		this->State_Wait();
		break;
	case TARGET_STATE::FADEOUT:
		this->State_FadeOut();
		break;
	case TARGET_STATE::FADEIN:
		this->State_FadeIn();
		break;
	case TARGET_STATE::CHANGE:
		this->State_Change();
		break;
	}
}

void TargetScrollCamera::State_Tracking()
{
	//State_Tracking�̏���

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�ړ��ʂ��v�Z����
	Vector3 vel = player->GetPosition() - m_target;

	//���������߂�
	Vector3 length = Length(player->GetPosition(),m_target);

	//�ړ��ʂ�␳����
	vel *= TARGET_TO_VECTOL;

	//���W��ݒ肷��
	this->SetPosition(Vector3(this->GetPosition().x + vel.x, 0.0f, 0.0f));

	//�^�[�Q�b�g���X�V����
	m_target += vel;

	//�s���ݒ肷��
	this->SetMatrix(Matrix::CreateTranslation(this->GetPosition()));

	//�X�N���[��������������A�t���O���N����
	if (length.x < 10.0f)
	{
		this->SetNowState(TARGET_STATE::WAIT);
	}
}

void TargetScrollCamera::State_Wait()
{
	//State_Wait�̏���

	//�J�E���g������������
	int count = NULL;

	//�Ή����闎�Ƃ����̃��X�g���擾����
	std::list<HoleFloor*>holeFloors = HoleFloor::GetHoleGrop(this->GetTargetNumber());

	//�Ή����闎�Ƃ������擾����
	for (HoleFloor* hole : holeFloors)
	{
		//�`�悳��Ă��Ȃ���΁A�J�E���g���X�V����
		if (hole->GetState() == ITextureBase::STATE::NONE)count++;
	}

	//�Ή�����S�Ă̗��Ƃ�����������Ă���΁A�X�e�[�g��؂�ւ���
	if (count >= (int)holeFloors.size())this->SetNowState(TARGET_STATE::FADEOUT);
}

void TargetScrollCamera::State_FadeOut()
{
	//State_FadeOut�̏���

	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneState���擾����
	SceneState* sceneState = sceneManager->GetState();

	//�L�[���͑҂��̎��̂݁A����������
	if (sceneState->GetFade() == SceneState::WAITKEY)
	{
		//�t�F�[�h�A�E�g���s��
		sceneState->SetFadeState(SceneState::FADEOUT);

		//�t�F�[�h�̉��Z���x��ݒ肷��
		sceneState->SetFadeSpeed(0.025f);
	}

	//�t�F�[�h�����̏�ԂɂȂ�����
	if (sceneState->GetFade() == SceneState::FADERESULT)
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(TARGET_STATE::FADEIN);

		//�t�F�[�h�C�����s��
		sceneState->SetFadeState(SceneState::FADEIN);

		//CameraManager���擾����
		CameraManager* cameraManager = GameContext::Get<CameraManager>();

		//�J������؂�ւ���
		cameraManager->PopCamera();
	}
}

void TargetScrollCamera::State_FadeIn()
{
	//State_FadeIn�̏���

	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneState���擾����
	SceneState* sceneState = sceneManager->GetState();

	//�L�[���͑҂��̂Ƃ�
	if (sceneState->GetFade() == SceneState::WAITKEY)
	{
		//�t�F�[�h�̉��Z���x��ݒ肷��
		sceneState->SetFadeSpeed(SceneState::ALFA_SPEED);

		//�X�e�[�g��؂�ւ���
		this->SetNowState(TARGET_STATE::CHANGE);
	}
}

void TargetScrollCamera::State_Change()
{
	//State_Change�̏���

	//�I�u�W�F�N�g�̍X�V��������
	this->SetObjectMove(true);
}
