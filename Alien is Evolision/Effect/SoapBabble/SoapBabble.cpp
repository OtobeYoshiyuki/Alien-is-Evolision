#include "pch.h"
#include "SoapBabble.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameObject/TimeLag/TimeLag.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Player/NormalTechnique/NormalTechnique.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

SoapBabble::SoapBabble(ITextureBase* texture):TextureBase(texture)
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("�V���{����"))
	{
		//�A�j���[�V�����̎g�p����e�N�X�`�����擾����
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//�A�j���[�V��������ݒ肷��
		this->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//���̃A�j���[�V�����̏���ݒ肷��
		this->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		this->SetEndTime(importer->GetAnimation(),importer->GetEnd());
	}

	this->SetLoopFlag(true);
	this->SetAniFlag(true);

	this->SetNowInfo("�V���{����1");
	this->SetLoopInfo("�V���{����6");

	this->SetNowAnimation(this->GetRect());

	//�X�V�֐����N����
	this->SetActive(true);
}

void SoapBabble::Initialize()
{
}

void SoapBabble::Update()
{
	//UI�̕\�����I���܂ŉ������Ȃ�
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//�`�悳��Ă��Ȃ���΁A�������Ȃ�
	if (this->GetState() == STATE::NONE)return;

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//���s�֐��������Ă���Ƃ��́A�������Ȃ�
	if (!this->GetActive())return;
	
	//�A�j���[�V�����̍X�V�̏���
	Animation::Update();

	//�摜�̍X�V����
	TextureBase::Update();

	//�؂�o���ʒu�̐ݒ�
	this->SetRect(this->GetNowAnimation());

	//�����蔻��̍��W��ݒ肷��
	this->SetCenter(this->GetPosition());

	//�摜�̑傫���𓖂��蔻��Ɏg�p����
	this->SetRadius(Vector3(this->AskOrigin() * this->GetScale()));

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseHero(this);

	//NormalTequnique���擾����
	NormalTechnique* nTequ = GameContext::Get<NormalTechnique>();

	//NeedleOverAll���擾����
	NeedleOverall* needleOverAll = GameContext::Get<NeedleOverall>();

	//Resurrection���擾����
	Resurrection* resurrection = GameContext::Get<Resurrection>();

	for (NormalBullet* bullet : nTequ->GetBulletsList())
	{
		//�G��HP��0���傫���Ƃ��ɓ����蔻���ǉ�����
		if (this->GetOwner()->GetStatus()->GetHP() > NULL)
		{
			//�����蔻���ǉ�����
			GameContext::Get<CollisionManager>()->AddUseOther(this, bullet, 200.0f);
		}
	}

	for (Needle* needle : needleOverAll->GetNeedleList())
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 300.0f);
	}

	for (Needle* needle : resurrection->GetNeedleList())
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 400.0f);
	}
}

void SoapBabble::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void SoapBabble::Finalize()
{
}

void SoapBabble::OnCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::OnCollisionEnter(obj);

	//�v���C���[�Ɠ����������̏���
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);

	//�E�C���X�r�[���Ɠ��������Ƃ��̏���
	if (obj->GetName() == "�E�C���X�r�[��")this->OnCollision_VirusBeam(obj);

	//NormalBullet�Ɠ����������̏���
	if (obj->GetName() == "�ʏ�e")this->OnCollision_NormalBullet(obj);

	//WhiteBack�Ɠ����������̏���
	if (obj->GetName() == "WhiteBack")this->OnCollision_WhiteBack(obj);

	//Needle�Ɠ��������Ƃ��̏���
	if (obj->GetName() == "shortNeedle" || obj->GetName() == "longNeedle")
	{
		this->OnCollision_Needle(obj);
	}
}

void SoapBabble::NonCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::NonCollisionEnter(obj);
}

void SoapBabble::OnCollision_Player(ITextureBase* player)
{
	//�v���C���[�Ɠ����������̏���
	
	//�`�������
	this->SetState(STATE::NONE);
}

void SoapBabble::OnCollision_VirusBeam(ITextureBase* beam)
{
	//�E�C���X�r�[���Ɠ����������̏���

	//�`�������
	this->SetState(STATE::NONE);

}

void SoapBabble::OnCollision_NormalBullet(ITextureBase* bullet)
{
	//�ʏ�e�Ɠ����������̏���

	//�`�������
	this->SetState(STATE::NONE);
}

void SoapBabble::OnCollision_WhiteBack(ITextureBase* whiteback)
{
	//�w�i�Ɠ����������̏���
	int i = 0;

	//�`�������
	this->SetState(STATE::NONE);
}

void SoapBabble::OnCollision_Needle(ITextureBase* needle)
{
	//�j�Ɠ����������̏���

	//�`�������
	this->SetState(STATE::NONE);

}
