#include "pch.h"
#include "StarManager.h"
#include "GameContext.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

StarManager::StarManager():m_MainStar(nullptr)
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	
	//���̃e�N�X�`�����擾����
	ITextureBase* star = resourceManager->GetTexture("��");

	//TextureManager���擾
	TextureManager* texMana = GameContext::Get<PlayScene>()->GetSubInfoManager().get();

	//MainStar�𐶐�����
	std::unique_ptr<Star> pManiStar = std::make_unique<Star>(star);
	pManiStar->SetPosition(Vector3(Game::MAX_WIDTH / 2, Game::MAX_HEIGHT / 2, 0.0f));
	pManiStar->SetOrigin(pManiStar->AskOrigin());
	pManiStar->SetColor(Colors::Yellow);
	pManiStar->SetState(ITextureBase::STATE::NONE);
	pManiStar->SetAngleSpeed(1.0f);
	pManiStar->SetDrawOrder(0);
	m_MainStar = pManiStar.get();

	//AutoMoveState�̐ݒ������
	m_MainStar->GetAutoMoveState()->SetSpeed(Vector3(5.0f, 5.0f, 0.0f));

	//PopStarRed�𐶐�����
	std::unique_ptr<Star> pPopRedStar = std::make_unique<Star>(star);
	pPopRedStar->SetOrigin(pPopRedStar->AskOrigin());
	pPopRedStar->SetColor(Colors::Red);
	pPopRedStar->SetState(ITextureBase::STATE::NONE);
	pPopRedStar->SetScale(Vector3(0.5f));
	pPopRedStar->SetAngleSpeed(1.0f);
	m_PopStarRed = pPopRedStar.get();

	//AutoMoveState�̐ݒ������
	m_PopStarRed->GetAutoMoveState()->SetSpeed(Vector3(5.0f, 5.0f, 0.0f));
	//JumpState�̐ݒ������
	m_PopStarRed->GetJumpState()->SetJumpLimit(100.0f);
	m_PopStarRed->GetJumpState()->SetJumpSpeed(10.0f);
	m_PopStarRed->GetJumpState()->SetJumpOption(true);
	m_PopStarRed->GetJumpState()->SetJumpLimitCount(1);

	//PopStarBlue�𐶐�����
	std::unique_ptr<Star> pPopBlueStar = std::make_unique<Star>(star);
	pPopBlueStar->SetOrigin(pPopBlueStar->AskOrigin());
	pPopBlueStar->SetColor(Colors::Blue);
	pPopBlueStar->SetState(ITextureBase::STATE::NONE);
	pPopBlueStar->SetScale(Vector3(0.5f));
	pPopBlueStar->SetAngleSpeed(1.0f);
	m_PopStarBlue = pPopBlueStar.get();

	//AutoMoveState�̐ݒ������
	m_PopStarBlue->GetAutoMoveState()->SetSpeed(Vector3(5.0f, 5.0f, 0.0f));
	//JumpState�̐ݒ������
	m_PopStarBlue->GetJumpState()->SetJumpLimit(100.0f);
	m_PopStarBlue->GetJumpState()->SetJumpSpeed(10.0f);
	m_PopStarBlue->GetJumpState()->SetJumpOption(true);
	m_PopStarBlue->GetJumpState()->SetJumpLimitCount(1);

	//���X�g�ɒǉ�����
	m_PopStars.push_back(m_PopStarRed);
	m_PopStars.push_back(m_PopStarBlue);

	//TextureManager�ɓo�^����
	texMana->Add(std::move(pManiStar));
	texMana->Add(std::move(pPopRedStar));
	texMana->Add(std::move(pPopBlueStar));
}

void StarManager::Initialize(const Vector3& pos)
{
	//���W��ݒ肷��

	//���S�̐��̍��W��ݒ肷��
	m_MainStar->SetPosition(pos + Vector3(m_MainStar->AskOrigin().x,0.0f,0.0f));

	//�`����I���ɂ���
	m_MainStar->SetState(ITextureBase::STATE::DRAW);

	//���Ɉړ�������
	m_MainStar->GetAutoMoveState()->SetExecute(true);
	m_MainStar->GetAutoMoveState()->SetNowState(DIRECT::DDOWN);
}

void StarManager::Update()
{
	//Ready�G�t�F�N�g���擾����
	TimeLag* Ready = GameContext::Get<PlayScene>()->GetLagStart().get();

	//���S�̐��̍X�V����
	this->UpdateMainStar();

	//�͂�����Ԃ����̍X�V����
	this->UpdatePopStarRed();

	//�͂���������̍X�V����
	this->UpdatePopStarBlue();
}

void StarManager::UpdateMainStar()
{
	//���S�̐��̍X�V����

	//�摜�̉��̕����Ŕ�������
	float StarBotom = m_MainStar->GetPosition().y + m_MainStar->AskOrigin().y;

	//�摜����ʂ̈�ԉ��܂ŗ�����A�ړ����~���āA�`����~�߂�
	if (StarBotom >= Game::MAX_HEIGHT + 80)
	{
		//�`�悪�I���̎��ɁA�`���؂�
		if (m_MainStar->GetState() == ITextureBase::STATE::DRAW)
		{
			m_MainStar->GetAutoMoveState()->SetNowState(DIRECT::NNONE);
			m_MainStar->SetState(ITextureBase::STATE::NONE);

			//���S�̐��̍��W�����ƂɁA�Ԃ����Ɛ����̍��W��ݒ肷��
			m_PopStarRed->SetPosition(m_MainStar->GetPosition());
			m_PopStarBlue->SetPosition(m_MainStar->GetPosition());

			//�Ԃ����̃X�e�[�g�̐ݒ���s��
			m_PopStarRed->GetAutoMoveState()->SetExecute(true);
			m_PopStarRed->GetJumpState()->SetExecute(true);
			m_PopStarRed->GetAutoMoveState()->SetNowState(DIRECT::RIGHT);
			m_PopStarRed->GetJumpState()->SetNowState(JUMP::START);
			m_PopStarRed->SetState(ITextureBase::STATE::DRAW);

			//�����̃X�e�[�g�̐ݒ���s��
			m_PopStarBlue->GetAutoMoveState()->SetExecute(true);
			m_PopStarBlue->GetJumpState()->SetExecute(true);
			m_PopStarBlue->GetAutoMoveState()->SetNowState(DIRECT::LEFT);
			m_PopStarBlue->GetJumpState()->SetNowState(JUMP::START);
			m_PopStarBlue->SetState(ITextureBase::STATE::DRAW);
		}

	}
}

void StarManager::UpdatePopStarRed()
{
	//�͂�����Ԃ����̍X�V����

	//�W�����v���āA���n�����^�C�~���O�ŁA�`���؂�
	if (m_PopStarRed->GetJumpState()->GetJumpmDesignation())
	{
		m_PopStarRed->SetState(ITextureBase::STATE::NONE);
	}
}

void StarManager::UpdatePopStarBlue()
{
	//�͂���������̍X�V����

	//�W�����v���āA���n�����^�C�~���O�ŁA�`���؂�
	if (m_PopStarBlue->GetJumpState()->GetJumpmDesignation())
	{
		m_PopStarBlue->SetState(ITextureBase::STATE::NONE);
	}
}

