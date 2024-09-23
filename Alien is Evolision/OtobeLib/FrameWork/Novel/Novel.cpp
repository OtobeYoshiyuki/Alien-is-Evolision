#include "pch.h"
#include "Novel.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Novel::Novel():
	m_nameWindow(nullptr),//���O�̃E�B���h�E
	m_textWindow(nullptr),//�e�L�X�g���̃E�B���h�E
	m_story(nullptr)//�X�g�[���[
{
}


Novel::~Novel()
{
}

void Novel::Initialize(const wchar_t* fileName, TextureManager* texMana)
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* textWindow = resourceManager->GetTexture("�E�B���h�E");
	ITextureBase* nameWindow = resourceManager->GetTexture("���O�o�[");

	//�E�B���h�E�𐶐�
	std::unique_ptr<TextWindow>ptextWindow = std::make_unique<TextWindow>(textWindow);
	//std::unique_ptr<NameWindow>pnameWindow = std::make_unique<NameWindow>(nameWindow);

	//���L�҂�ݒ肷��
	ptextWindow->SetOwner(this);
	//pnameWindow->SetOwner(this);

	//�X�g�[���[�𐶐�
	m_story = std::make_unique<Story>();
	m_story->Initialize(fileName);

	//���|�C���^��o�^
	m_textWindow = ptextWindow.get();
	//m_nameWindow = pnameWindow.get();

	//TextureManager�ɒǉ�
	texMana->Add(std::move(ptextWindow));
	//texMana->Add(std::move(pnameWindow));

	//�E�B���h�E�̏�����
	m_textWindow->Initialize();
	//m_nameWindow->Initialize();
}

void Novel::Update()
{
}

void Novel::Finalize()
{
}
