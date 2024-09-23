#include "pch.h"
#include "LevelUp.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

LevelUp::LevelUp(ITextureBase* texture):
	TextureBase(texture)//���N���X�ɏ��𑗐M����
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Level"))
	{
		//�A�j���[�V�����̎g�p����e�N�X�`�����擾����
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//�A�j���[�V��������ݒ肷��
		this->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//���̃A�j���[�V�����̏���ݒ肷��
		this->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		this->SetEndTime(importer->GetAnimation(), importer->GetEnd());
	}

	//�A�j���[�V�����̃t���O��ݒ肷��
	this->SetAniFlag(false);
	this->SetLoopFlag(false);

	//���݂̃A�j���[�V��������ݒ肷��
	this->SetNowInfo("Level1");
	this->SetLoopInfo("Level10");
	this->SetNowAnimation(this->GetNowAnimation());

	//���s�֐����N����
	this->SetActive(true);
}

void LevelUp::Initialize()
{
	//���W��ݒ肷��
	this->SetPosition(Vector3(600, 100, 0));
}

void LevelUp::Update()
{
	//UI�̕\�����I���܂ŉ������Ȃ�
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//���s�֐��������Ă���Ƃ��́A�������Ȃ�
	if (!this->GetActive())return;

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//�X�e�[�g�̍X�V����
	this->UpdateState();

	//�A�j���[�V�����̍X�V����
	Animation::Update();

	//�摜�̍X�V����
	TextureBase::Update();

	//�摜�̐؂�o���ʒu���X�V����
	this->SetRect(this->GetNowAnimation());
}

void LevelUp::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void LevelUp::Finalize()
{
}

void LevelUp::UpdateState()
{
	//Switch���ŏ����𕪂���

	switch (this->GetNowState())
	{
	case LEVELUP::NONE:
		this->State_None();
		break;
	case LEVELUP::LEVELUP:
		this->State_LevelUp();
		break;
	case LEVELUP::ANIMATION:
		this->State_Animation();
		break;
	case LEVELUP::DOWN:
		this->State_Down();
		break;
	case LEVELUP::RESULT:
		this->State_Result();
		break;
	}
}

void LevelUp::State_None()
{
	//State_None�̏���

	//�`���؂�
	this->SetState(STATE::NONE);

	//�F��ݒ肷��
	this->SetColor(Colors::White);
}

void LevelUp::State_LevelUp()
{
	//State_LevelUp�̏���

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�`����N����
	this->SetState(STATE::DRAW);

	//�A�j���[�V�������N��������
	this->SetAniFlag(true);

	//�X�e�[�g��؂�ւ���
	this->SetNowState(LEVELUP::ANIMATION);

	//SE���Đ�����
	soundManager->Play(CRI_CUESHEET_0__12_���x���A�b�v);
}

void LevelUp::State_Animation()
{
	//State_Animation�̏���

	//�A�j���[�V�������I������܂ő҂�
	if (!this->GetAniFlag())
	{
		//�A�j���[�V�������I��������A�X�e�[�g��؂�ւ���
		this->SetNowState(LEVELUP::DOWN);
	}
}

void LevelUp::State_Down()
{
	//State_Down�̏���

	//�F���擾����
	Color color = this->GetColor();

	//�A���t�@�l���擾����
	float alfa = color.A();

	//�A���t�@�l���v�Z����
	alfa -= 0.01f;

	//�A���t�@�l���Đݒ肷��
	color.A(alfa);

	//�A���t�@�l��0�����̏ꍇ
	if (alfa < 0.0f)
	{
		//�A���t�@�l��␳����
		alfa = 0.0f;

		//�X�e�[�g��؂�ւ���
		this->SetNowState(LEVELUP::RESULT);
	}

	//�F���Đݒ肷��
	this->SetColor(color);
}

void LevelUp::State_Result()
{
	//State_Result�̏���

	//�o���l�Q�[�W���擾����
	ExperienceGage* exGage = GameContext::Get<CoroKun>()->GetExperienceGage();

	//�X�e�[�g��؂�ւ���
	this->SetNowState(LEVELUP::NONE);

	//���g�̃X�e�[�g��؂�ւ���
	exGage->SetNowState(CALC::CALCING);
}
