#pragma once
#include "pch.h"
#include "ChoicesScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Input/Input.h"

using namespace OtobeLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;



//�R���X�g���N�^
ChoicesScene::ChoicesScene():
	m_textureManager(nullptr),//�摜�̊Ǘ��N���X
	m_novel(nullptr),//�m�x���̊Ǘ��N���X
	m_yesTexture(nullptr),//�͂��̃e�N�X�`��
	m_noTexture(nullptr)//�������̃e�N�X�`��
{
}

//����������
void ChoicesScene::Initialize()
{
	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneState���擾����
	SceneState* sceneState = sceneManager->GetState();

	//��������
	this->Create();

	//�X�e�[�g�̏�����
	this->AddData1(CHOICESSTATE::STORYWAIT);

	//�I�����ڂ̏�����
	this->AddData2(NULL);
}

//�X�V����
void ChoicesScene::Update()
{
	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//�L�[���͂��擾����
	Input* input = Input::GetInstance();

	//�g���b�J�[���擾����
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//X�L�[�������ꂽ��
	if (keyTrakker.IsKeyPressed(Keyboard::X))
	{
		//���݂�Scene������
		sceneManager->PopScene();

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);

		//�I�����ڂ�ݒ肷��
		staticData->Add("�I����", (std::string)"������");

		//�ȍ~�̏����͍s��Ȃ�
		return;
	}

	//�摜�̊Ǘ��N���X�̍X�V
	m_textureManager->Update();

	//�m�x���̊Ǘ��N���X�̍X�V
	m_novel->Update();

	//�X�e�[�g�̍X�V����
	this->UpdateState();
}

//�`�揈��
void ChoicesScene::Render()
{
	//�R���e�L�X�g���擾����
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//�`��n�N���X���擾����
	Graphics* graph = Graphics::GetInstance();

	//�t�H���g���擾����
	TextureFont* font = TextureFont::GetInstance();

	//�`��̏����ݒ������
	graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		graph->GetCommonStates()->NonPremultiplied(), graph->GetCommonStates()->PointClamp(),
		graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//�摜��`��
	m_textureManager->Render();

	//�͂��̃e�N�X�`�����\������Ă���
	if (m_yesTexture->GetState() == ITextureBase::STATE::DRAW)
	{
		//�t�H���g��`��
		font->DrawFormatString("�͂�", Vector2(250.0f, 180.0f), 3.0f, Colors::Black,64);
	}

	//�������̃e�N�X�`�����\������Ă���
	if (m_noTexture->GetState() == ITextureBase::STATE::DRAW)
	{
		//�t�H���g��`��
		font->DrawFormatString("������", Vector2(720.0f, 180.0f), 3.0f, Colors::Black,20);
	}

	//�t�H���g��`�悷��

	//���L�[�̕`��
	font->DrawFormatString("���E�L�|�F�ړ�", Vector2(20.0f, 675.0f), 1.0f, Colors::Black);

	//SPACE�L�[�̕`��
	font->DrawFormatString("SPACE�L�|�F����", Vector2(500.0f, 675.0f), 1.0f, Colors::Black);

	//���j���[�֖߂�
	font->DrawFormatString("X�L�|�F�L�����Z��", Vector2(950.0f, 675.0f), 1.0f, Colors::Black);

	//�`��̏I����K������
	graph->GetSpriteBatch()->End();
}

//�I������
void ChoicesScene::Finalize()
{
}

//��������
void ChoicesScene::Create()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");

	//�摜�̊Ǘ��N���X�𐶐�����
	m_textureManager = std::make_unique<TextureManager>();

	//Scene�̋�؂�p�̃e�N�X�`���𐶐�
	std::unique_ptr<ITextureBase> pseparation = std::make_unique<TextureBase>(whiteBack);
	pseparation->SetRect(RECT{ 0,0,1280,720 });
	pseparation->SetColor(Color(0.0f, 0.0f, 0.0f, 0.5f));

	//�I�����𐶐�����
	//�͂��̃e�N�X�`���̎l�p���g�𐶐�
	std::unique_ptr<ITextureBase> pyes = std::make_unique<TextureBase>(whiteBack);
	pyes->SetPosition(Vector3(200.0f, 150.0f, 0.0f));
	pyes->SetRect(RECT{ 0,0,350,150 });
	pyes->SetColor(Colors::Red);
	pyes->SetState(ITextureBase::STATE::NONE);
	m_yesTexture = pyes.get();

	//�������̃e�N�X�`���̎l�p���g�𐶐�
	std::unique_ptr<ITextureBase> pno = std::make_unique<TextureBase>(whiteBack);
	pno->SetPosition(Vector3(700.0f, 150.0f, 0.0f));
	pno->SetRect(RECT{ 0,0,350,150 });
	pno->SetState(ITextureBase::STATE::NONE);
	m_noTexture = pno.get();

	//TextureManager�ɒǉ�����
	m_textureManager->Add(std::move(pseparation));
	m_textureManager->Add(std::move(pyes));
	m_textureManager->Add(std::move(pno));

	//��ʉ�����UI���쐬����
	std::unique_ptr<ITextureBase> pbotomUI = std::make_unique<TextureBase>(whiteBack);
	pbotomUI->SetPosition(Vector3(0.0f, 665.0f, 0.0f));
	pbotomUI->SetRect(RECT{ 0,0,1280,60 });

	//TextureManager�ɒǉ�����
	m_textureManager->Add(std::move(pbotomUI));

	//�t�@�C���̃p�X���擾����
	std::wstring path = TextureBase::Convert(staticData->Get_StringData("�X�g�[���[�p�X"));

	//�m�x���̊Ǘ��N���X�𐶐�����
	m_novel = std::make_unique<Novel>();
	m_novel->Initialize(path.c_str(), m_textureManager.get());
}

//�X�e�[�g�̍X�V����
void ChoicesScene::UpdateState()
{
	//Switch���ŏ����𕪂���
	switch (this->GetNowData1())
	{
	case CHOICESSTATE::STORYWAIT:
		this->StateStoryWait();
		break;
	case CHOICESSTATE::INPUT:
		this->StateInput();
		break;
	}
}

//State_StoryWait�̏���
void ChoicesScene::StateStoryWait()
{
	//�X�g�[���[���擾����
	Story* story = m_novel->GetStory();

	//�e�L�X�g�E�B���h�E���擾����
	TextWindow* textWindow = m_novel->GetTextWindow();

	//�X�g�[���[�̃e�L�X�g���擾����
	std::string storytext = story->GetStoryText(textWindow->GetDrawCount());

	//���݂̃e�L�X�g����
	//�Ō�܂œǂ܂ꂽ��
	if (storytext == TextWindow::NEWLINE_AND_STOP &&
		textWindow->GetDrawCount() >= (int)story->GetStoryArray().size() - 2)
	{
		//�I������\������
		m_yesTexture->SetState(ITextureBase::STATE::DRAW);
		m_noTexture->SetState(ITextureBase::STATE::DRAW);

		//�X�e�[�g��؂�ւ���
		this->AddData1(CHOICESSTATE::INPUT);
	}
}

//StateInput�̏���
void ChoicesScene::StateInput()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//�L�[���͂��擾����
	Input* input = Input::GetInstance();

	//�g���b�J�[���擾����
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�X�y�[�X�L�[�������ꂽ��
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//�͂���I�𒆂̎�
		if (this->GetNowData2() == YES)
		{
			//�I�����ڂ�ݒ肷��
			staticData->Add("�I����", (std::string)"�͂�");
		}
		//��������I�𒆂̎�
		else if (this->GetNowData2() == NO)
		{
			//�I�����ڂ�ݒ肷��
			staticData->Add("�I����", (std::string)"������");

			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
		}

		//���݂�Scene������
		sceneManager->PopScene();

		//�ȍ~�̏����͍s��Ȃ�
		return;
	}

	//�㉺�̃L�[�������ꂽ��A�I����ύX����
	if (keyTrakker.IsKeyPressed(Keyboard::Left) ||
		keyTrakker.IsKeyPressed(Keyboard::Right))
	{
		//�I��Ώۂ��擾����
		int target = this->GetNowData2();

		//�I��Ώۂ��f�N�������g����
		target--;

		//�I��Ώۂ��Βl�ɒ���
		target = abs(target);

		//�F���擾����
		Color preColor = m_yesTexture->GetColor();
		Color nowColor = m_noTexture->GetColor();

		//����ւ���
		std::swap(preColor, nowColor);

		//�F���Đݒ肷��
		m_yesTexture->SetColor(preColor);
		m_noTexture->SetColor(nowColor);

		//�I��Ώۂ�ݒ肷��
		this->AddData2(target);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__07_�㉺��);
	}
}