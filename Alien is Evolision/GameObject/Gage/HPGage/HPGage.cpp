#include "pch.h"
#include "HPGage.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

HPGage::HPGage(ITextureBase* texture)
	:TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_backTexture(nullptr),//���ʂ̃e�N�X�`��
	m_Max(NULL),//�Q�[�W�̍ő�l
	m_easingTime(0.0f)//�C�[�W���O�֐��p�̎���
{
}

void HPGage::Initialize()
{

}

void HPGage::Update()
{
	//�摜�̍X�V����
	TextureBase::Update();
}

void HPGage::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void HPGage::Finalize()
{
}

void HPGage::CreateGage(TextureManager* textureManager)
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* gageTexture = resourceManager->GetTexture("WhiteBack");

	//�w�ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase> pbackTexture = std::make_unique<TextureBase>(gageTexture);

	//���W��ݒ肷��
	pbackTexture->SetPosition(this->GetPosition());

	//�؂�o���ʒu��ݒ肷��
	pbackTexture->SetRect(this->GetRect());

	//�F��ݒ肷��
	pbackTexture->SetColor(Colors::Red);

	//�`�揇��ύX����
	pbackTexture->SetDrawOrder(5);

	//���|�C���^��ǉ�����
	m_backTexture = pbackTexture.get();

	//�摜��ǉ�����
	textureManager->Add(std::move(pbackTexture));
}
