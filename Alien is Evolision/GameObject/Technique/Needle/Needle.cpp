#include "pch.h"
#include "Needle.h"
#include "GameObject/Technique/NeedleOverall/NeedleOverall.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Needle::Needle(ITextureBase* texture):TextureBase(texture)
{
	//�����蔻��̔��a��ݒ肷��
	this->SetRadius(Vector3(this->AskOrigin()*2));
}

void Needle::Initialize()
{
}

void Needle::Update()
{
	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//�摜�̍X�V����
	TextureBase::Update();

	//�����蔻��̍��W��ݒ肷��
	this->SetCenter(this->GetPosition());

	//�����蔻��̒ǉ�
	GameContext::Get<CollisionManager>()->AddUseHero(this);
}

void Needle::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void Needle::Finalize()
{
}

void Needle::OnCollisionEnter(ITextureBase* obj)
{
	CollisionBase::OnCollisionEnter(obj);
}

void Needle::NonCollisionEnter(ITextureBase* obj)
{
	CollisionBase::NonCollisionEnter(obj);
}
