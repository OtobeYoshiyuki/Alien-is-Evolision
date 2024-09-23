#include "pch.h"
#include "AnimeTexture.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

AnimeTexture::AnimeTexture(ITextureBase* texture):
	TextureBase(texture)//���N���X�ɏ��𑗐M����
{
}

void AnimeTexture::Initialize()
{
}

void AnimeTexture::InitAnimation(
	const std::string& grop,//�A�j���[�V�����O���[�v�̖��O
	const bool& animeflag,//�A�j���[�V�����J�E���g�X�V�p�̃t���O
	const bool& loopfrag,//�A�j���[�V�����̃��[�v�Đ��p�̃t���O
	const std::string& nowinfo,//���݂̃A�j���[�V�����̏��
	const std::string& loopinfo//�A�j���[�V�����̃��[�v���
)
{
	//ResourcesManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList(grop))
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
	this->SetAniFlag(animeflag);
	this->SetLoopFlag(loopfrag);

	//���݂̃A�j���[�V��������ݒ肷��
	this->SetNowInfo(nowinfo);
	this->SetLoopInfo(loopinfo);
	this->SetNowAnimation(this->GetNowAnimation());

	//���s�֐����N����
	this->SetActive(true);
}

void AnimeTexture::Update()
{
	//UI�̕\�����I���܂ŉ������Ȃ�
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//���s�֐��������Ă���Ƃ��́A�������Ȃ�
	if (!this->GetActive())return;

	//�A�j���[�V�����̍X�V����
	Animation::Update();

	//�摜�̍X�V����
	TextureBase::Update();

	//�摜�̐؂�o���ʒu���X�V����
	this->SetRect(this->GetNowAnimation());
}

void AnimeTexture::Render()
{
	//�摜�̕`�揈�����s��
	TextureBase::Render();
}

void AnimeTexture::Finalize()
{
}
