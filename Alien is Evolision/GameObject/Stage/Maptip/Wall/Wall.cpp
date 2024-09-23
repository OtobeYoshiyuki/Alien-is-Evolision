#include "pch.h"
#include "Wall.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameObject/Player/NormalTechnique/NormalTechnique.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/CheckPoint/CheckPoint.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Wall::Wall(ITextureBase* texture):MapTip(texture)
{
}

void Wall::Initialize()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Wall"))
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

	//�A�j���[�V�����̏���ݒ肷��

	//�X�V�t���O��ݒ肷��
	this->SetAniFlag(true);

	//���[�v�t���O��ݒ肷��
	this->SetLoopFlag(true);

	//���݂̃A�j���[�V��������ݒ肷��
	this->SetNowInfo("Wall1");

	//���[�v����A�j���[�V��������ݒ肷��
	this->SetLoopInfo("Wall12");

	//�A�j���[�V������ݒ肷��
	this->SetNowAnimation(this->GetRect());
}

void Wall::Update()
{
	MapTip::Update();

	Animation::Update();

	//���݂̃A�j���[�V��������ݒ肷��
	this->SetRect(Animation::GetNowAnimation());

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);

	//�j�[�h���̒������͈͍U�����擾����
	std::vector<Needle*> needleList = GameContext::Get<NeedleOverall>()->GetNeedleList();

	for (Needle* needle : needleList)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 100.0f);
	}

	//�ʏ�e���擾����
	std::vector<NormalBullet*>bulletList = GameContext::Get<NormalTechnique>()->GetBulletsList();

	for (NormalBullet* bullet : bulletList)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, bullet, 300.0f);
	}

	//�ƂĂ������G��vector���擾����
	std::vector<HyperEnemy*>HyperEnemyList = GameContext::Get<Stage>()->GetHyperEnemies();

	for (HyperEnemy* Hyper : HyperEnemyList)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, Hyper, 100.0f);
	}

	//�A�C�e���̃��X�g���擾����
	std::vector<Item*> itemList = GameContext::Get<PlayScene>()->GetHealManager()->GetAllItem();

	for (Item* item : itemList)
	{
		//�����蔻���ݒ肷��
		GameContext::Get<CollisionManager>()->AddUseOther(this, item, 100.0f);
	}
}

void Wall::Render()
{
	MapTip::Render();
}

void Wall::Finalize()
{
}

void Wall::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);
}

void Wall::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}
