#include "pch.h"
#include "HoleSwitch.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameObject/Player/NormalTechnique/NormalTechnique.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Stage/Maptip/HoleFloor/HoleFloor.h"
#include "Utils.h"
#include "GameObject/TargetScrollCamera/TargetScrollCamera.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

HoleSwitch::HoleSwitch(ITextureBase* texture):
	MapTip(texture),//���N���X�ɏ��𑗐M����
	m_DetonationCount(0.0f),//�N���J�E���g
	m_DetonationFlag(false),//�N���t���O
	m_SelealNumber(NULL),//�V���A���i���o�[
	m_backTexture(nullptr)//�w�i�̃e�N�X�`��
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//GameManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//�g���e�N�X�`�����擾����
	ITextureBase* mapTip = resourceManager->GetTexture("�}�b�v�`�b�v");

	//�w�i�̃e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase>pback = std::make_unique<TextureBase>(mapTip);
	pback->SetRect(RECT{ 40,160,80,200 });
	pback->SetDrawOrder(4);
	m_backTexture = pback.get();

	//gameManager�ɒǉ�����
	gameManager->Add(std::move(pback));
}

void HoleSwitch::Initialize()
{
	//�w�i�̍��W��ݒ肷��
	m_backTexture->SetPosition(this->GetPosition());

	//�X�e�[�g������������
	this->SetNowState(FADE_STATE::DOWN);
}

void HoleSwitch::Update()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�}�b�v�`�b�v�̍X�V����
	MapTip::Update();

	//�w�i�̐F�����擾����
	Color color = m_backTexture->GetColor();
	float alfa = color.A();

	//�X�e�[�g�ɂ���āA������؂�ւ���
	switch (this->GetNowState())
	{
	case FADE_STATE::DOWN:
		if (color.A() <= 0.5f)this->SetNowState(FADE_STATE::FUP);
		else alfa -= 0.01f;
		break;
	case FADE_STATE::FUP:
		if (color.A() >= 1.0f)this->SetNowState(FADE_STATE::DOWN);
		else alfa += 0.01f;
		break;
	}

	//�F�����Đݒ肷��
	color.A(alfa);
	m_backTexture->SetColor(color);

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�X�N���[���J�����ɃL���X�g����
	TargetScrollCamera* scrollCamera = dynamic_cast<TargetScrollCamera*>(nowCamera);

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove() && scrollCamera && 
		scrollCamera->GetNowState() < TARGET_STATE::WAIT)return;

	//�`�悪�؂��Ă�����A�N���X���O�𗧂Ă�
	if (this->GetState() == STATE::NONE)
	{
		m_DetonationFlag = true;
	}

	//�N���t���O��true�̂Ƃ��A�N���J�E���g���X�V����
	if (m_DetonationFlag)
	{
		//�N���J�E���g���X�V����
		m_DetonationCount += staticData->Get_FloatData("elapsedTime") * 10;
		//�N���J�E���g��StaticData�ɓo�^����
		staticData->Add(this->GetName() + std::to_string(this->GetSerealNumber()) + "Count", m_DetonationCount);
		//���g�̘A�Ԃ�o�^����
		staticData->Add(this->GetName() + std::to_string(this->GetSerealNumber()),this->GetSerealNumber());
	}	

	//�r�[�����擾����
	VirusBeam* beam = GameContext::Get<VirusBeam>();

	//�摜�̉��̕����擾����
	float length = (beam->GetRect().right - beam->GetRect().left) / 2;

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);
	GameContext::Get<CollisionManager>()->AddUseOther(this, beam, length + 100.0f);

	//�j�[�h���̒������͈͍U�����擾����
	std::vector<Needle*> needleListOver = GameContext::Get<NeedleOverall>()->GetNeedleList();

	for (Needle* needle : needleListOver)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 100.0f);
	}

	//�j�[�h���̒Z�����͈͍U�����擾����
	std::vector<Needle*> needleListWide = GameContext::Get<Resurrection>()->GetNeedleList();

	for (Needle* needle : needleListWide)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 500.0f);
	}

	//�ʏ�e���擾����
	std::vector<NormalBullet*>bulletList = GameContext::Get<NormalTechnique>()->GetBulletsList();

	for (NormalBullet* bullet : bulletList)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, bullet, 300.0f);
	}
}

void HoleSwitch::Render()
{
	//�}�b�v�`�b�v�̕`�揈��
	MapTip::Render();
}

void HoleSwitch::Finalize()
{
}

void HoleSwitch::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�`�悳��Ă���Ƃ��̂݁A���s
	if (this->GetState() == STATE::DRAW)
	{
		//�l������ő�̒l��ݒ肷��
		float min = FLT_MAX;

		//����ւ��p�̍��W
		Vector3 tempPos = Vector3::Zero;

		//����ւ��p�̕ϐ�
		int tempSerial = NULL;

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__22_�l��);

		//�Ή����闎�Ƃ����̃��X�g���擾����
		for (HoleFloor* hole : HoleFloor::GetHoleGrop(this->GetSerealNumber()))
		{
			//���������߂�
			Vector3 len = Length(player->GetPosition(), hole->GetPosition());

			//����ւ��̏������s��(�����X���W�ōs��)
			//��r�Ώۂ̕����������Ƃ��ɓ���ւ���
			if (min > len.x)
			{
				min = len.x;
				tempPos = hole->GetPosition();
				tempSerial = hole->GetSerealNumber();
			}
		}

		//�J�����̌��E�n���X�N���[���l���傫���Ƃ��́A�␳����
		if(tempPos.x > 3460.0f)tempPos.x = 3460.0f;
		
		//��ԋ߂����Ƃ������ATARGET_TO_LENGTH���傫���Ƃ��A�J������؂�ւ���
		if (min > TargetScrollCamera::TARGET_TO_LENGTH)
		{
			//�J�������^�[�Q�b�g�X�N���[���J�����ɐ؂�ւ���
			cameraManager->PushCamera("�X�N���[���J����");

			//�ÓI�f�[�^�Ɉꎞ�i�[����
			staticData->Add("�^�[�Q�b�g", tempPos);
			staticData->Add("�^�[�Q�b�g", tempSerial);
		}
	}
	
	//�`���؂�
	this->SetState(STATE::NONE);
	m_backTexture->SetState(STATE::NONE);
}

void HoleSwitch::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}
