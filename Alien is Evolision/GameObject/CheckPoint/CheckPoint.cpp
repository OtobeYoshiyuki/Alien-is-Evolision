#include "pch.h"
#include "CheckPoint.h"
#include "GameContext.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Enemy/Normal/NormalEnemy.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CheckPoint::CheckPoint(ITextureBase* texture):TextureBase(texture)
{
	//�摜�̑傫���𓖂��蔻��Ɏg�p����
	this->SetRadius(Vector3(this->AskOrigin()*2));
}

void CheckPoint::Initialize()
{
}

void CheckPoint::Update()
{
	//�摜�̍X�V����
	TextureBase::Update();

	//�����蔻��̍��W��ݒ肷��
	this->SetCenter(this->GetPosition() + this->AskOrigin());

	//HyperEnemy��vector���擾����
	std::vector<HyperEnemy*>HyperEnemyList = GameContext::Get<Stage>()->GetHyperEnemies();

	for (HyperEnemy* Hyper : HyperEnemyList)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, Hyper, 200.0f);
	}

	//NormalEnemy��vector���擾����
	std::vector<NormalEnemy*>NormalEnemyList = GameContext::Get<Stage>()->GetNormalEnemies();

	//NormalEnemy��vector���擾����
	for (NormalEnemy* Normal : NormalEnemyList)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, Normal, 200.0f);
	}
}

void CheckPoint::Render()
{
	////�摜�̕`�揈��
	//TextureBase::Render();
}

void CheckPoint::Finalize()
{
}

void CheckPoint::OnCollisionEnter(ITextureBase* obj)
{
	CollisionBase::OnCollisionEnter(obj);
}

void CheckPoint::NonCollisionEnter(ITextureBase* obj)
{
	CollisionBase::NonCollisionEnter(obj);
}
