#include "pch.h"
#include "TitleScene.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�X�e�[�W�Z���N�g��
const std::pair<std::string, Vector2> TitleScene::NEXT_STAGESELECT = { "�X�e�|�W�Z���N�g��" ,Vector2(200.0f, 450.0f) };

//�����L���O��
const std::pair<std::string, Vector2> TitleScene::NEXT_RANKING = { "�����L���O��" ,Vector2(350.0f, 450.0f) };

//�I�[�v�j���O��
const std::pair<std::string, DirectX::SimpleMath::Vector2> TitleScene::NEXT_OPENING = {"�I�|�v�j���O��",Vector2(300.0f,450.0f)};

TitleScene::TitleScene():
	m_TextureManager(nullptr),//�摜�̊Ǘ��N���X
	m_blink(nullptr),//�����̓_�ŃN���X
	m_leftArrowTexture(nullptr),//���̃e�N�X�`��
	m_rightArrowTexture(nullptr),//���̃e�N�X�`��
	m_targetStrings(std::pair<std::string,Vector3>("",Vector3::Zero))//�\�����镶��
{
}

void TitleScene::Initialize()
{
	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneState���擾����
	SceneState* sceneState = sceneManager->GetState();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�t�@�C���̎擾
	Stream* file = Stream::GetInstance();

	//�t�@�C�����J������A�J��
	if (file->Open(L"OpeningInfo.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		std::vector<std::vector<std::string>> LoadData = file->Load_Data();

		//�ÓI�f�[�^�ɏ���ǉ�����
		staticData->Add("�I�[�v�j���O", LoadData[1][0]);

		//�t�@�C�������
		file->Close();
	}

	//�������s��
	this->Create();

	//BGM���Đ�����
	soundManager->Play(CRI_CUESHEET_0__01_�^�C�g���pBGM);

	//�t�F�[�h�C�����s��
	sceneState->SetFadeState(SceneState::FADEIN);

	//�I�[�v�j���O�������ς݂̏ꍇ
	if (staticData->Get_StringData("�I�[�v�j���O") == "�����ς�")
	{
		//�\���^�[�Q�b�g��ݒ肷��
		m_targetStrings = NEXT_STAGESELECT;
	}
	//�I�[�v�j���O���������̏ꍇ
	else
	{
		//�\���^�[�Q�b�g��ݒ肷��
		m_targetStrings = NEXT_OPENING;
	}
}

void TitleScene::Update()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneState���擾����
	SceneState* sceneState = sceneManager->GetState();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�L�[�{�[�h�̏�Ԃ��擾
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//�t�F�[�h�A�E�g������������A�^�C�g���Ɉڍs����
	if (sceneState->GetFade() == SceneState::FADERESULT)
	{
		//�X�e�[�W�Z���N�g�̎�
		if (m_targetStrings == NEXT_STAGESELECT)
		{
			//�X�e�[�W�Z���N�gScene�ɐ؂�ւ���
			sceneManager->ChangeScene("StageSelect");
		}
		//�I�[�v�j���O�̎�
		else if (m_targetStrings == NEXT_OPENING)
		{
			//�X�e�[�W�Z���N�gScene�ɐ؂�ւ���
			sceneManager->ChangeScene("Opening");
		}
	}

	//�L�[���͑҂��ȊO�̎��́A�������΂�
	if (sceneState->GetFade() != SceneState::WAITKEY)return;

	//Space�L�[�����
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//�X�e�[�W�Z���N�g�̎�
		if (m_targetStrings == NEXT_STAGESELECT)
		{
			//�t�F�[�h�A�E�g���s��
			sceneState->SetFadeState(SceneState::FADEOUT);
		}
		//�����L���O�̎�
		else if (m_targetStrings == NEXT_RANKING)
		{
			//RankingScene��ǉ���������
			sceneManager->PushScene("Ranking");
		}
		//�I�[�v�j���O�̎�
		else if (m_targetStrings == NEXT_OPENING)
		{
			//�t�F�[�h�A�E�g���s��
			sceneState->SetFadeState(SceneState::FADEOUT);
		}

		//����̉���炷
		soundManager->Play(CRI_CUESHEET_0__05_���艹);
	}

	//�I�[�v�j���O�������ς݂̏ꍇ
	if (staticData->Get_StringData("�I�[�v�j���O") == "�����ς�")
	{
		//�E�L�[�����
		if (keyTrakker.IsKeyPressed(Keyboard::Right))
		{
			//�X�e�[�W�Z���N�g�̎�
			if (m_targetStrings == NEXT_STAGESELECT)m_targetStrings = NEXT_RANKING;

			//�����L���O�̎�
			else if (m_targetStrings == NEXT_RANKING)m_targetStrings = NEXT_OPENING;

			//�I�[�v�j���O�̎�
			else if (m_targetStrings == NEXT_OPENING)m_targetStrings = NEXT_STAGESELECT;

			//�I������炷
			soundManager->Play(CRI_CUESHEET_0__08_�X���C�h);
		}

		//���L�[�����
		if (keyTrakker.IsKeyPressed(Keyboard::Left))
		{
			//�X�e�[�W�Z���N�g�̎�
			if (m_targetStrings == NEXT_STAGESELECT)m_targetStrings = NEXT_OPENING;

			//�����L���O�̎�
			else if (m_targetStrings == NEXT_RANKING)m_targetStrings = NEXT_STAGESELECT;

			//�I�[�v�j���O�̎�
			else if (m_targetStrings == NEXT_OPENING)m_targetStrings = NEXT_RANKING;

			//�I������炷
			soundManager->Play(CRI_CUESHEET_0__08_�X���C�h);

		}
	}

	//TextureManager�̍X�V
	m_TextureManager->Update();

	//�_�ł̍X�V
	m_blink->Update();
}

void TitleScene::Render()
{
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	Graphics* Graph = Graphics::GetInstance();

	TextureFont* Font = TextureFont::GetInstance();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	m_TextureManager->Render();

	//�t�H���g��`�悷��
	if (m_blink->NowBlinking())
	{
		//���[�h�̕`��
		Font->DrawFormatString(m_targetStrings.first, m_targetStrings.second, 3.0f, Colors::Red);

		//�X�y�[�X�L�[�̕`��
		Font->DrawFormatString("PUSH SPACE", Vector2(350.0f, 600.0f), 2.0f, Colors::Black);
	}


	Graph->GetSpriteBatch()->End();
}

void TitleScene::Finalize()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//BGM������
	soundManager->Stop(CRI_CUESHEET_0__01_�^�C�g���pBGM);
}

void TitleScene::Create()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//�摜�̊Ǘ��N���X�𐶐�����
	m_TextureManager = std::make_unique<TextureManager>();

	//�g���e�N�X�`�����擾����
	ITextureBase* TitleTexture = resourceManager->GetTexture("Title");
	ITextureBase* arrowTexture = resourceManager->GetTexture("���");

	//Title��Texture�𐶐�����
	std::unique_ptr<ITextureBase>pTexture = std::make_unique<TextureBase>(TitleTexture);

	//TextureManager�ɒǉ�����
	m_TextureManager->Add(std::move(pTexture));

	//����Texture�𐶐�����
	//�I�[�v�j���O�������ς݂̏ꍇ
	if (staticData->Get_StringData("�I�[�v�j���O") == "�����ς�")
	{
		//���̖��𐶐�����
		std::unique_ptr<Arrow> pleftArrow = std::make_unique<Arrow>(arrowTexture);
		pleftArrow->InitArrow(Vector3(0.5f), Vector3(0.75f), Vector3(0.5f), 60.0f, ARROWSCALE::UPSCALE);
		pleftArrow->SetOrigin(pleftArrow->AskOrigin());
		pleftArrow->SetPosition(Vector3(100.0f, 500.0f, 0.0f));
		pleftArrow->SetAngle(180.0f);
		pleftArrow->SetColor(Colors::Blue);
		m_leftArrowTexture = pleftArrow.get();

		//TextureManager�ɒǉ�����
		m_TextureManager->Add(std::move(pleftArrow));


		//�E�̖��𐶐�����
		std::unique_ptr<Arrow> prightArrow = std::make_unique<Arrow>(arrowTexture);
		prightArrow->InitArrow(Vector3(0.5f), Vector3(0.75f), Vector3(0.5f), 60.0f, ARROWSCALE::UPSCALE);
		prightArrow->SetOrigin(prightArrow->AskOrigin());
		prightArrow->SetPosition(Vector3(1150.0f, 500.0f, 0.0f));
		prightArrow->SetColor(Colors::Blue);
		m_rightArrowTexture = prightArrow.get();

		//TextureManager�ɒǉ�����
		m_TextureManager->Add(std::move(prightArrow));
	}

	//�����̓_�ŃN���X�𐶐�����
	m_blink = std::make_unique<Blinking>();
	m_blink->SetBlink(true);
	m_blink->SetTarget(0.5f);
}

