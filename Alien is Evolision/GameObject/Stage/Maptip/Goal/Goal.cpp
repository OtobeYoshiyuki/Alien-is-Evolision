#include "pch.h"
#include "Goal.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Goal::Goal(ITextureBase* texture):MapTip(texture)
{
	this->SetDrawOrder(5);
}

void Goal::Initialize()
{
}

void Goal::Update()
{
	//�}�b�v�`�b�v�̍X�V
	MapTip::Update();

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);
}

void Goal::Render()
{
	//�}�b�v�`�b�v�̕`��
	MapTip::Render();
}

void Goal::Finalize()
{
}

void Goal::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);
}

void Goal::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}
