#include "pch.h"
#include "Babul.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameObject/Technique/NeedleOverall/NeedleOverall.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Babul::Babul(ITextureBase* texture):MapTip(texture)
{
}

void Babul::Initialize()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Babble"))
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
	this->SetNowInfo("Babble1");

	//���[�v����A�j���[�V��������ݒ肷��
	this->SetLoopInfo("Babble9");

	//�A�j���[�V������ݒ肷��
	this->SetNowAnimation(this->GetRect());
}

void Babul::Update()
{
	MapTip::Update();

	Animation::Update();

	//���݂̃A�j���[�V��������ݒ肷��
	this->SetRect(Animation::GetNowAnimation());

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�v���C���[���������Ă���Ƃ�
	if (player->GetHP() > NULL)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);
	}

	//�j�[�h���̒������͈͍U�����擾����
	std::vector<Needle*> overNeedleList = GameContext::Get<NeedleOverall>()->GetNeedleList();

	for (Needle* needle : overNeedleList)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 100.0f);
	}

	//�j�[�h���̒Z�����͈͍U�����擾����
	std::vector<Needle*> reNeedleList = GameContext::Get<Resurrection>()->GetNeedleList();

	for (Needle* needle : reNeedleList)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 300.0f);
	}
}

void Babul::Render()
{
	MapTip::Render();
}

void Babul::Finalize()
{
}

void Babul::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);
}

void Babul::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}
