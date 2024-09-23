#include "pch.h"
#include "PoceScene.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�t�H���g�̏��
const std::vector<std::pair<std::string, Vector2>> PoceScene::FONT_INFO =
{
	{"�v���C",Vector2(600.0f,150.0f)},
	{"�^�C�g��",Vector2(550.0f,450.0f)}
};

PoceScene::PoceScene():
	m_TextureManager(nullptr)//�摜�̊Ǘ��N���X
{
}



void PoceScene::Initialize()
{
	//���݂̓Y������ݒ肷��
	this->SetNowState(NULL);

	//�����������s��
	this->Create();
}

void PoceScene::Update()
{
	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();
	SceneState* sceneState = sceneManager->GetState();

	//�L�[���͂��擾
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�^�C�g���֖߂�
	if (sceneState->GetFade() == SceneState::FADERESULT)
	{
		sceneManager->ChangeScene("Title");
	}

	//�L�[���͑҂��ȊO�̎��́A�������Ȃ�
	if (sceneState->GetFade() != SceneState::WAITKEY)return;

	//TextureManager�̍X�V
	m_TextureManager->Update();

	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);

		//���݂�Scene������
		sceneManager->PopScene();
	}


	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//�v���C�֖߂�ꍇ
		if (this->GetNowState() == NULL)
		{
			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__05_���艹);

			//���݂�Scene������
			sceneManager->PopScene();
		}
		//�^�C�g���֖߂�ꍇ
		else {
			//�t�F�[�h�A�E�g����
			sceneState->SetFadeState(SceneState::FADEOUT);

			//�t�F�[�h�̉��Z���x��ݒ肷��
			sceneState->SetFadeSpeed(SceneState::ALFA_SPEED);

			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__23_�v���C�t�F�[�h�A�E�g);
		}
	}

	if (keyTrakker.IsKeyPressed(Keyboard::Up) ||
		keyTrakker.IsKeyPressed(Keyboard::Down))
	{
		//�I�����ڂ̍X�V
		int nowState = this->GetNowState();
		int newState = nowState;

		//�f�N�������g
		newState--;

		//��Βl�ɒ���
		newState = abs(newState);

		//�F���擾����
		Color nowColor = m_sceneTexture[nowState]->GetColor();
		Color newColor = m_sceneTexture[newState]->GetColor();

		//��������
		std::swap(nowColor, newColor);

		//�F��ݒ肷��
		m_sceneTexture[nowState]->SetColor(nowColor);
		m_sceneTexture[newState]->SetColor(newColor);

		//�Y�������X�V����
		this->SetNowState(newState);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__07_�㉺��);
	}

}

void PoceScene::Render()
{
	//�`��n�̏����擾
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//�t�H���g���擾����
	TextureFont* font = TextureFont::GetInstance();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//TextureManager�̕`��
	m_TextureManager->Render();

	for (const std::pair<std::string, Vector2>& info : FONT_INFO)
	{
		//�t�H���g��`�悷��
		font->DrawFormatString(info.first, info.second, 2.0f, Colors::Black);
	}

	//�㉺�L�[�̃t�H���g��`��
	font->DrawFormatString("�㉺�L�|�F�ړ�", Vector2(210.0f, 670.0f), 1.0f, Colors::Black);

	//�X�y�[�X�L�[�̃t�H���g�̕`��
	font->DrawFormatString("SPACE�L�|�F����", Vector2(550.0f, 670.0f), 1.0f, Colors::Black);

	//ECS�L�[�̃t�H���g�̕`��
	font->DrawFormatString("ECS�L�|�F�߂�", Vector2(1000.0f, 670.0f), 1.0f, Colors::Black);

	Graph->GetSpriteBatch()->End();

}

void PoceScene::Finalize()
{
}

void PoceScene::Create()
{
	//Manager�N���X�ɓo�^����
	m_TextureManager = std::make_unique<TextureManager>();

	//�K�v�ȃ��\�[�X���Q�Ƃ���
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");
	
	//for���[�v�ŉ�
	for (int i = 0; i < 2; i++)
	{
		//�e�N�X�`���𐶐�����
		std::unique_ptr<ITextureBase> pwhiteBack = std::make_unique<TextureBase>(whiteBack);

		//RECT��ݒ肷��
		pwhiteBack->SetRect(RECT{ 0,0,800,150 });

		//���W��ݒ肷��
		pwhiteBack->SetPosition(Vector3(250.0f, i * 300 + 100, 0.0f));

		//���|�C���^��o�^����
		m_sceneTexture.push_back(pwhiteBack.get());

		m_TextureManager->Add(std::move(pwhiteBack));
	}

	//���݂̐F��ɂ���
	m_sceneTexture[this->GetNowState()]->SetColor(Colors::Aquamarine);

	//��ʉ�����UI��`��
	std::unique_ptr<ITextureBase> pBotomBackUI = std::make_unique<TextureBase>(whiteBack);
	pBotomBackUI->SetRect(RECT{ 0,0,1080,141 });
	pBotomBackUI->SetPosition(Vector3(200.0f, 661.0f, 0.0f));

	//TextureManager�ɒǉ�����
	m_TextureManager->Add(std::move(pBotomBackUI));

}
