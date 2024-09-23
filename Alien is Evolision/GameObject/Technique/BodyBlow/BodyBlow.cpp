#include "pch.h"
#include "BodyBlow.h"
#include "OtobeLib/Camera/Manager/CameraManager.h" 
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

BodyBlow::BodyBlow(ITextureBase* texture):TextureBase(texture)
{
	//�`���؂�
	this->SetState(STATE::NONE);
	//�摜�̒��S�����ォ�璆���ɂ���
	this->SetOrigin(TextureBase::AskOrigin());
}

void BodyBlow::Initialize()
{
}

void BodyBlow::Update()
{	
	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//�A�j���[�V�����̏���ݒ肷��
	this->SetRect(this->GetNowAnimation());

	if(!this->GetActive())soundManager->Stop(CRI_CUESHEET_0__26_�̓�����);

	//�A�j���[�V�����̍X�V
	Animation::Update();

	//�摜�̍X�V����
	TextureBase::Update();
}

void BodyBlow::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void BodyBlow::Finalize()
{
}

void BodyBlow::WithPlayer(ITextureBase* texture)
{
	//�摜�������o��
	TextureBase::Extrude(texture);
}
