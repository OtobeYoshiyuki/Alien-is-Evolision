#include "pch.h"
#include "NormalTechnique.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameObject/Player/CoroKun.h"
#include "GameObject/Gage/NormalGage/NormalGage.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

NormalTechnique::NormalTechnique():
	m_BackGage(nullptr),//�Q�[�W�̗���
	m_NormalGage(nullptr),//�Q�[�W�̕\��
	m_FrontBullet(nullptr),//�擪�̒e
	m_Active(false),//���s�\
	m_checkPush(false),//�_���[�W����p�̃t���O
	m_checkRelease(false),//�_���[�W����p�̃t���O
	m_time(0.0f)//����
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//�g���e�N�X�`�����擾����
	ITextureBase* gagetex = resourceManager->GetTexture("WhiteBack");
	ITextureBase* particle = resourceManager->GetTexture("���q");

	//�Z�Q�[�W�𐶐�����
	std::unique_ptr<NormalGage> pNormalGage = std::make_unique<NormalGage>(gagetex);
	pNormalGage->SetState(ITextureBase::STATE::DRAW);
	pNormalGage->SetDrawOrder(0);
	pNormalGage->SetRect(RECT{ 0,0,0,20 });
	pNormalGage->SetColor(Colors::Blue);
	pNormalGage->SetNowState(NORMALSTATE::LEFTUP);
	m_NormalGage = pNormalGage.get();

	//�Z�Q�[�W�̗��ʂ𐶐�����
	std::unique_ptr<ITextureBase> pBackGage = std::make_unique<TextureBase>(gagetex);
	pBackGage->SetState(ITextureBase::STATE::DRAW);
	pBackGage->SetDrawOrder(1);
	pBackGage->SetRect(RECT{ 0,0,100,20 });
	pBackGage->SetColor(Colors::White);
	m_BackGage = pBackGage.get();

	//TextureManager�ɒǉ�����
	gameManager->Add(std::move(pNormalGage));
	gameManager->Add(std::move(pBackGage));

	//�ʏ�e��10�e��������
	for (int i = 0; i < BULLET_NUM; i++)
	{
		std::unique_ptr<NormalBullet>pBullet = std::make_unique<NormalBullet>(particle);
		pBullet->SetDrawOrder(2);
		pBullet->SetState(ITextureBase::STATE::NONE);
		pBullet->SetOrigin(pBullet->AskOrigin());
		pBullet->SetName("�ʏ�e");
		m_NormalBulletsList.push_back(pBullet.get());
		gameManager->Add(std::move(pBullet));
	}

	//GameContext�ɓo�^����
	GameContext::Register<NormalTechnique>(this);

	//m_FrontBullet = this->GetAllBulletFront();
}

void NormalTechnique::Initialize()
{
}

void NormalTechnique::Update()
{
	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�v���C���[�̍��W�����ƂɁA�Q�[�W�̍��W�����肷��
	m_NormalGage->SetPosition(player->GetPosition() - Vector2(50.0f,70.0f));
	m_BackGage->SetPosition(player->GetPosition() - Vector2(50.0f,70.0f));
}

NormalBullet* NormalTechnique::GetAllBulletFront()
{
	//���ˑҋ@���Ă���擪�̃|�C���^���擾����

	for (NormalBullet* nBullet : m_NormalBulletsList)
	{
		if (nBullet->GetState() == ITextureBase::STATE::NONE)
		{
			return nBullet;
		}
	}

	return nullptr;
}
