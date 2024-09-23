#include "pch.h"
#include "MenuScene.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�X�e�[�^�X�̍��W
const Vector3 MenuScene::STATUS_POS = Vector3(50.0f,150.0f,0.0f);

//�A�C�e���̍��W
const Vector3 MenuScene::ITEM_POS = Vector3(50.0f, 350.0f, 0.0f);

//�L�����u�b�N�̍��W
const Vector3 MenuScene::CHARBOOK_POS = Vector3(50.0f, 550.0f, 0.0f);

//�摜�̍��W
const std::vector<DirectX::SimpleMath::Vector3> MenuScene::TEXPOS =
{
	STATUS_POS,//�X�e�[�^�X�̍��W
	ITEM_POS,//�A�C�e���̍��W
	CHARBOOK_POS,//�L�����u�b�N�̍��W
};

//�eScene�̏��
const std::vector<std::string> MenuScene::SCENES_INFO =
{
	"Status",//�X�e�[�^�X�V�[��
	"Item",//�A�C�e���V�[��
	"CharBook",//�L�����u�b�N�V�[��
};

MenuScene::MenuScene():
	m_TextureManager(nullptr),//�摜�̊Ǘ��N���X
	m_backSpaceTexture(nullptr)//�X�y�[�X�L�[�̗��ʂ̃e�N�X�`��
{
}

void MenuScene::Initialize()
{
	//�����̏���
	this->Create();

	//�X�e�[�g��؂�ւ���
	this->SetNowState((int)SELECT_INFO::STATUS);

	//�I�����Ă���F��ύX����
	m_selectTextures[this->GetNowState()]->SetColor(Colors::Red);

	//GameContext�ɓo�^����
	GameContext::Register<MenuScene>(this);
}

void MenuScene::Update()
{
	//�L�[�{�[�h�̏��
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();
	const auto& Mouse_Trakker = Input::GetInstance()->GetMouse_Trakker();

	//SceneManager���擾����
	SceneManager* manager = SceneManager::GetInstance();

	//CollisionManager���擾����
	CollisionManager* collisionManager = GameContext::Get<CollisionManager>();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�I�𒆂̉摜���擾����
	ITextureBase* nowTexture = m_selectTextures[this->GetNowState()];

	//X�L�[�������ꂽ��A���j���[�����
	if (keyTrakker.IsKeyPressed(Keyboard::X))
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);

		manager->PopScene();
		collisionManager->ResetUseHero();
		return;
	}

	//Space�L�[�������ꂽ��A�I������Scene�ֈڍs����
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		this->InputEntry();
	}
	//�F���Ԃ̎�
	else if (nowTexture->GetName() == "��")
	{
		//�F�𔒂ɕύX����
		nowTexture->SetColor(Colors::Red);

		//�^�O��ύX����
		nowTexture->SetName("��");

		//Scene��ǉ���������
		if (SCENES_INFO[this->GetNowState()] != "CharBook")
		{
			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__05_���艹);

			manager->PushScene(SCENES_INFO[this->GetNowState()]);
		}
	}
		
	//���L�[�������ꂽ�Ƃ�
	if (keyTrakker.IsKeyPressed(Keyboard::Down))
	{
		this->InputSelect(1, [&](int& num)
			{
				//�z��̎Q�Ɨ̈�𒴂�����A�␳����
				if (num >= (int)m_selectTextures.size())num = NULL;});

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__07_�㉺��);
	}

	//��L�[�������ꂽ�Ƃ�
	if (keyTrakker.IsKeyPressed(Keyboard::Up))
	{
		this->InputSelect(-1, [&](int& num)
			{
				//�z��̎Q�Ɨ̈�𒴂�����A�␳����
				if (num <= -1)num = (int)m_selectTextures.size() - 1;});

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__07_�㉺��);
	}

	//�摜�̊Ǘ��N���X�̍X�V����
	m_TextureManager->Update();
}

void MenuScene::Render()
{
	//�f�o�C�X�R���e�L�X�g���擾����
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//�`��n�̏����擾����
	Graphics* Graph = Graphics::GetInstance();

	//�t�H���g���擾����
	TextureFont* font = TextureFont::GetInstance();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//�摜�̊Ǘ��N���X�̕`�揈��
	m_TextureManager->Render();

	//�t�H���g��`�悷��
	font->DrawFormatString("�X�e�|�^�X", Vector2(STATUS_POS) + Vector2(10.0f,20.0f),1.8f,Colors::Black);
	font->DrawFormatString("�A�C�e��", Vector2(ITEM_POS) + Vector2(30.0f, 20.0f), 1.8f, Colors::Black);
	font->DrawFormatString("�L�����u�b�N", Vector2(CHARBOOK_POS) + Vector2(0.0f, 20.0f), 1.5f, Colors::Black);
	font->DrawFormatString("X�L�|�F�v���C��", Vector2(50.0f, 60.0f), 1.0f, Colors::Black);

	//�_�Œ�����Ȃ��Ƃ�
	if (m_backSpaceTexture->NowBlinking())
	{
		font->DrawFormatString("PUSH SPACE", Vector2(500.0f, 350.0f), 2.0f, Colors::Black);
	}

	Graph->GetSpriteBatch()->End();
}

void MenuScene::Finalize()
{
}

void MenuScene::Create()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager�𐶐�����
	m_TextureManager = std::make_unique<TextureManager>();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* menuTexture = resourceManager->GetTexture("Menu");
	ITextureBase* whiteback = resourceManager->GetTexture("WhiteBack");

	//���j���[�̉摜�𐶐�����
	std::unique_ptr<ITextureBase> pmenuTexture = std::make_unique<TextureBase>(menuTexture);

	//TextureManager�ɒǉ�����
	m_TextureManager->Add(std::move(pmenuTexture));

	for (const Vector3& pos : TEXPOS)
	{
		//�摜�𐶐�����
		std::unique_ptr<FadeTexture> ptexture = std::make_unique<FadeTexture>(whiteback);

		//���W��ݒ肷��
		ptexture->SetPosition(pos);

		//RECT��ݒ肷��
		ptexture->SetRect(RECT{ 0,0,300,100 });

		//�t�F�[�h�̃A���t�@�l�̍ŏ��l��ݒ肷��
		ptexture->SetMinAlfa(0.25f);

		//�t�F�[�h�̃A���t�@�l�̍ő�l��ݒ肷��
		ptexture->SetMaxAlfa(1.0f);

		//���|�C���^��o�^����
		m_selectTextures.push_back(ptexture.get());

		//TextureManager�ɒǉ�����
		m_TextureManager->Add(std::move(ptexture));
	}

	//�I�𒆂̉摜�̃t�F�[�h���J�n����
	m_selectTextures[0]->SetNowState(FADE_STATE::DOWN);

	//SPACE�L�[�̗��ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<TextureBase> pbackSpace = std::make_unique<TextureBase>(whiteback);
	pbackSpace->SetPosition(Vector3(450.0f, 320.0f, 0.0f));
	pbackSpace->SetRect(RECT{ 0,0,750,120 });
	pbackSpace->SetBlink(true);
	pbackSpace->SetTarget(0.5f);
	m_backSpaceTexture = pbackSpace.get();

	//X�L�[�̗��ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase> pbackXkey = std::make_unique<TextureBase>(whiteback);
	pbackXkey->SetPosition(Vector3(30.0f, 50.0f, 0.0f));
	pbackXkey->SetRect(RECT{ 0,0,300,50 });

	//TextureManager�ɒǉ�����
	m_TextureManager->Add(std::move(pbackSpace));
	m_TextureManager->Add(std::move(pbackXkey));
}

void MenuScene::InputSelect(const int& add, std::function<void(int&)> func)
{
	//���݂̏�Ԃ��擾����
	int nowSelect = this->GetNowState();

	//���݂̐F���擾����
	Color nowColor = m_selectTextures[nowSelect]->GetColor();

	//�V������Ԃ��擾����
	int newSelect = nowSelect + add;

	//�����_������֐������s
	func(newSelect);

	//�V�����F���擾����
	Color newColor = m_selectTextures[newSelect]->GetColor();

	//�F�����ւ���
	std::swap(nowColor, newColor);

	//�X�e�[�g���X�V����
	this->SetNowState(newSelect);

	//�F��ݒ肷��
	m_selectTextures[nowSelect]->SetColor(nowColor);
	m_selectTextures[newSelect]->SetColor(newColor);

	//�t�F�[�h�̃X�e�[�g��ݒ肷��
	m_selectTextures[nowSelect]->SetNowState(FADE_STATE::NONE);
	m_selectTextures[newSelect]->SetNowState(FADE_STATE::DOWN);
}

void MenuScene::InputEntry()
{
	//�I�𒆂̉摜���擾����
	ITextureBase* nowTexture = m_selectTextures[this->GetNowState()];

	//�F��ύX����
	nowTexture->SetColor(Colors::White);

	//�^�O��ύX����
	nowTexture->SetName("��");
}

