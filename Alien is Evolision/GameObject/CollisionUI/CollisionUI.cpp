#include "pch.h"
#include "CollisionUI.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "Effect/SoapBabble/SoapBabble.h"
#include "GameObject/FixdTurret/FixedTurret.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CollisionUI::CollisionUI(ITextureBase* texture):TextureBase(texture)
{
}

void CollisionUI::Initialize()
{
	//�摜�̑傫���𓖂��蔻��Ɏg�p����
	this->SetRadius(Vector3(this->AskOrigin() * 2));
}

void CollisionUI::Update()
{
	TextureBase::Update();

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//Stage���擾����
	Stage* stage = GameContext::Get<Stage>();

	//�����蔻��̍��W��ݒ肷��
	this->SetCenter(this->GetPosition() + this->AskOrigin());

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseOther(this, player->GetName());
	GameContext::Get<CollisionManager>()->AddUseOther(this, "shortNeedle");
	GameContext::Get<CollisionManager>()->AddUseOther(this, "�ʏ�e");
	GameContext::Get<CollisionManager>()->AddUseOther(this, "HyperEnemy");
	GameContext::Get<CollisionManager>()->AddUseOther(this, "SuperEnemy");
	GameContext::Get<CollisionManager>()->AddUseOther(this, "Hypersyabondama");
	GameContext::Get<CollisionManager>()->AddUseOther(this, "Supersyabondama");
}

void CollisionUI::Render()
{
	TextureBase::Render();
}

void CollisionUI::Finalize()
{
}

void CollisionUI::OnCollisionEnter(ITextureBase* obj)
{
	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	CollisionBase::OnCollisionEnter(obj);
}

void CollisionUI::NonCollisionEnter(ITextureBase* obj)
{
	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	CollisionBase::NonCollisionEnter(obj);
}

void CollisionUI::WithPlayer(ITextureBase* texture)
{
	//�摜�������o��
	TextureBase::Extrude(texture);
}
