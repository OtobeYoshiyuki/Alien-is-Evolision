#include "pch.h"
#include "Rebound.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Rebound::Rebound(OtobeLib::ITextureBase* texture):
	MapTip(texture),//���N���X�ɏ��𑗐M����
	m_frontTexture(nullptr)//�O�ʂ̃e�N�X�`��
{
}

void Rebound::Initialize()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//GameManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//�g���e�N�X�`�����擾����
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");

	//�F����ݒ肷��
	Color color = Colors::Red;
	color.A(0.5f);

	//�e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase>pfront = std::make_unique<TextureBase>(whiteBack);
	pfront->SetColor(color);
	pfront->SetRect(RECT{ 0,0,40,40 });
	pfront->SetDrawOrder(0);
	pfront->SetPosition(this->GetPosition());
	m_frontTexture = pfront.get();

	//GameManager�ɒǉ�����
	gameManager->Add(std::move(pfront));

	//�X�e�[�g������������
	this->SetNowState(FADE_STATE::DOWN);
}

void Rebound::Update()
{
	MapTip::Update();

	//�X�e�[�g�̍X�V����
	this->UpdateState();

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�v���C���[���������Ă���Ƃ�
	if (player->GetHP() > NULL)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);
	}

	//�j�[�h���̒������͈͍U�����擾����
	std::vector<Needle*> needleList = GameContext::Get<NeedleOverall>()->GetNeedleList();

	for (Needle* needle : needleList)
	{
		//�����蔻���ǉ�����
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 100.0f);
	}
}

void Rebound::Render()
{
	MapTip::Render();
}

void Rebound::Finalize()
{
}

void Rebound::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);
}

void Rebound::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}

void Rebound::UpdateState()
{
	//�w�i�̐F�����擾����
	Color color = m_frontTexture->GetColor();
	float alfa = color.A();

	//Switch���ɂ���āA�����𕪂���
	switch (this->GetNowState())
	{
	case FADE_STATE::DOWN:
		if (color.A() <= 0.0f)this->SetNowState(FADE_STATE::FUP);
		else alfa -= 0.01f;
		break;
	case FADE_STATE::FUP:
		if (color.A() >= 0.5f)this->SetNowState(FADE_STATE::DOWN);
		else alfa += 0.01f;
		break;
	}

	//�F�����Đݒ肷��
	color.A(alfa);
	m_frontTexture->SetColor(color);
}
