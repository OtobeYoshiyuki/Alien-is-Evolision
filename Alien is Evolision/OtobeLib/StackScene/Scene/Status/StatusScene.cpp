#include "pch.h"
#include "StatusScene.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/StackScene/Scene/Menu/MenuScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�X�e�[�^�X�֘A�̘A�z�z��
std::vector<std::pair<std::string, Vector2>> StatusScene::s_statusInfo =
{
	{"�Ȃ܂��F�R������",Vector2(450.0f,25.0f)},
	{"LV",Vector2(425.0f,65.0f)},
	{"HP",Vector2(425.0f,105.0f)},
	{"MP",Vector2(425.0f,145.0f)},
	{"AT",Vector2(425.0f,185.0f)},
	{"DF",Vector2(425.0f,225.0f)},
	{"SP",Vector2(425.0f,265.0f)},
	{"NT",Vector2(425.0f,305.0f)},
	{"TL",Vector2(425.0f,345.0f)}
};

//�R�}���h�֘A�̘A�z�z��
const std::vector<std::pair<std::string, Vector2>> StatusScene::COMMAND_INFO =
{
	{"�R�}���h�킴",Vector2(475.0f,400.0f)},
	{"����������F�j�|�h��",Vector2(425.0f,440)},
	{"�r�|���F���U���N�V����",Vector2(425.0f,480.0f)},
	{"�Ђ����킴",Vector2(475.0f,550.0f)},
	{"�_�|�N",Vector2(425.0f,590.0f)},
	{"�G�N�X�v���|�W����",Vector2(475.0f,622.0f)},
	{"���o�|�X�A�r���e�B",Vector2(425.0f,662.0f)}
};

StatusScene::StatusScene():
	m_TextureManager(nullptr),//�摜�̊Ǘ��N���X
	m_backSpaceTexture(nullptr)//�X�y�[�X�L�[�̗��ʂ̃e�N�X�`��
{
}

void StatusScene::Initialize()
{
	//��������
	this->Create();

	//PlayScene���擾����
	PlayScene* playScene = GameContext::Get<PlayScene>();

	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//���̃��x���܂ł̌o���l
	int nextLevel = NULL;

	//�X�^�e�B�b�N�ϐ����㏑������
	s_statusInfo[(int)STATUS_INFO::LEVEL].first = "LV�F" + std::to_string(player->GetLevel());
	s_statusInfo[(int)STATUS_INFO::HP].first = "HP�F" + std::to_string(player->GetHP()) + "/" + std::to_string(player->GetMaxHP());
	s_statusInfo[(int)STATUS_INFO::MP].first = "MP�F" + std::to_string(player->GetMP()) + "/" + std::to_string(player->GetMaxMP());
	s_statusInfo[(int)STATUS_INFO::AT].first = "AK�F" + std::to_string(player->GetAtack());
	s_statusInfo[(int)STATUS_INFO::DF].first = "DF�F" + std::to_string(player->GetDefence());

	s_statusInfo[(int)STATUS_INFO::SP].first = "SP�F" + std::to_string(player->GetSpecial()) +
													"/" + std::to_string(player->GetMaxSpecialPoint());
	s_statusInfo[(int)STATUS_INFO::TOTAL].first = "TL�F" + std::to_string(player->GetNextLevelPoint());

	//���̃��x���܂ł̌o���l�́A���x��1���猻�݂̃��x���܂ł̌o���l�̍ő�𑫂��āA
	//�l���ς݂̌o���l�����������̂Ƃ���
	for (int i = CoroKun::MIN_LEVEL; i <= player->GetLevel(); i++)
	{
		//���̃��x���̃X�e�[�^�X���擾����
		StatusImporter* importer = StatusBase::FindStatus(player->GetName(), i);

		//�o���l�̍ő�����Z���Ă���
		nextLevel += importer->GetNextLevelPoint();
	}

	//1�O�̃��x���̌o���l�̍ő������
	if (player->GetLevel() - 1 > NULL)
	{
		nextLevel -= player->GetNextLevelPoint();
		nextLevel = player->GetMaxNextLevelPoint() - nextLevel;

		//if (nextLevel < NULL)
		//{
		//	nextLevel += player->GetNextLevelPoint();
		//}
	}
	else
		//���x����1�̎�
	{
		nextLevel = player->GetNextLevelPoint();
	}

	//���x�����ő�̎��́A�����I�ɂO�ɂ���
	if (player->GetLevel() == CoroKun::MAX_LEVEL)
	{
		nextLevel = player->GetMaxNextLevelPoint();
	}

	//���̃��x���܂ł̌o���l
	s_statusInfo[(int)STATUS_INFO::NEXT].first = "NT�F" + std::to_string(nextLevel) +
		"/" + std::to_string(player->GetMaxNextLevelPoint());

	//�o���l���v�Z��
	if (player->GetExperienceGage()->CheckExp())
	{
		s_statusInfo[(int)STATUS_INFO::NEXT].first = "NT�F�������񂿂イ";
	}
}

void StatusScene::Update()
{
	//�L�[�{�[�h�̏��
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();
	const auto& Mouse_Trakker = Input::GetInstance()->GetMouse_Trakker();

	//SceneManager���擾����
	SceneManager* manager = SceneManager::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//MenuScene���擾����
	MenuScene* menu = GameContext::Get<MenuScene>();

	//X�L�[�������ꂽ��A���j���[�ɖ߂�
	if (keyTrakker.IsKeyPressed(Keyboard::X))
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);

		manager->PopScene();
	}

	//ESC�L�[�������ꂽ��APlay�ɖ߂�
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);

		manager->PopScene(2);
	}

	//TextureManager�̍X�V
	m_TextureManager->Update();

	//Menu��TextureManager�̍X�V��������(UI�𓮂�������)
	menu->GetTextureManager()->Update();
}

void StatusScene::Render()
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

	for (const std::pair<std::string, Vector2>& status : s_statusInfo)
	{
		//�t�H���g��`�悷��
		font->DrawFormatString(status.first, status.second, 1.0f, Colors::Black);
	}

	for (const std::pair<std::string, Vector2>& tech : COMMAND_INFO)
	{
		//�t�H���g��`�悷��
		font->DrawFormatString(tech.first, tech.second, 1.0f, Colors::Black);
	}

	//X�L�[�̃t�H���g�̕`��
	font->DrawFormatString("X�L�|�F���j���|��", Vector2(50.0f, 60.0f), 1.0f, Colors::Black);

	//ESC�L�[�̃t�H���g�̕`��
	font->DrawFormatString("ESC�L�|�F�v���C��", Vector2(925.0f, 60.0f), 1.0f, Colors::Black);

	Graph->GetSpriteBatch()->End();
}

void StatusScene::Finalize()
{
}

void StatusScene::Create()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager�𐶐�����
	m_TextureManager = std::make_unique<TextureManager>();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* back = resourceManager->GetTexture("�X�e�[�^�X���");
	ITextureBase* player = resourceManager->GetTexture("�X�e�[�^�X�v���C���[");
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");

	//�w�i�𐶐�����
	std::unique_ptr<ITextureBase> pback = std::make_unique<TextureBase>(back);
	std::unique_ptr<ITextureBase> pPlayer = std::make_unique<TextureBase>(player);
	std::unique_ptr<ITextureBase> pwhite = std::make_unique<TextureBase>(whiteBack);

	//���W��ݒ肷��
	pwhite->SetPosition(Vector3(400.0f, 0.0f, 0.0f));

	//�F��ݒ肷��
	pwhite->SetColor(Color{ 1.0f,1.0f,1.0f,0.5f });

	//�؂���ʒu��ݒ肷��
	pwhite->SetRect(RECT{ 0,0,480,720 });

	//X�L�[�̗��ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase> pbackXkey = std::make_unique<TextureBase>(whiteBack);
	pbackXkey->SetPosition(Vector3(30.0f, 50.0f, 0.0f));
	pbackXkey->SetRect(RECT{ 0,0,330,50 });

	//ESC�L�[�̗��ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<ITextureBase> pbackEsckey = std::make_unique<TextureBase>(whiteBack);
	pbackEsckey->SetPosition(Vector3(900.0f, 50.0f, 0.0f));
	pbackEsckey->SetRect(RECT{ 0,0,370,50 });

	//TextureManager�ɒǉ�����
	m_TextureManager->Add(std::move(pback));
	m_TextureManager->Add(std::move(pPlayer));
	m_TextureManager->Add(std::move(pwhite));
	m_TextureManager->Add(std::move(pbackXkey));
	m_TextureManager->Add(std::move(pbackEsckey));
}
