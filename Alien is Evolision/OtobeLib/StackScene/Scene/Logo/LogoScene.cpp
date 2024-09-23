#pragma once
#include "pch.h"
#include "LogoScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"

using namespace OtobeLib;
using namespace DirectX;

LogoScene::LogoScene():
	m_TextureManager(nullptr)//�摜�̊Ǘ��N���X
{
}

void LogoScene::Initialize()
{
	//�������s��
	this->Create();

	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneState���擾����
	SceneState* sceneState = sceneManager->GetState();

	//�t�F�[�h�C�����s��
	sceneState->SetFadeState(SceneState::FADEIN);
}

void LogoScene::Update()
{
	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneState���擾����
	SceneState* sceneState = sceneManager->GetState();

	//�t�F�[�h�A�E�g������������A�^�C�g���Ɉڍs����
	if (sceneState->GetFade() == SceneState::FADERESULT)
	{
		sceneManager->ChangeScene("Title");
	}

	//�L�[���͑҂��ȊO�̎��́A�������΂�
	if (sceneState->GetFade() != SceneState::WAITKEY)return;

	//�L�[���͑҂��̎��́A�����Ƀt�F�[�h�A�E�g����
	sceneState->SetFadeState(SceneState::FADEOUT);

	//�摜�̍X�V����
	m_TextureManager->Update();
}

void LogoScene::Render()
{
	//�`��n�̏����擾����
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	Graphics* Graph = Graphics::GetInstance();

	//�`��̐ݒ���s��
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//�e�N�X�`����`��
	m_TextureManager->Render();

	//�`��̐ݒ���I������
	Graph->GetSpriteBatch()->End();
}

void LogoScene::Finalize()
{
}

void LogoScene::Create()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�g���e�N�X�`�����擾����
	ITextureBase* LogoTexture = resourceManager->GetTexture("Logo");

	//Logo�̃e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase>pTexture = std::make_unique<TextureBase>(LogoTexture);

	//Manager�N���X�ɓo�^����
	m_TextureManager = std::make_unique<TextureManager>();
	m_TextureManager->Add(std::move(pTexture));
}

