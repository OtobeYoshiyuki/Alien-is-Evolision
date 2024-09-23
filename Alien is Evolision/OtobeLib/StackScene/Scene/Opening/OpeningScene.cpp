#include "pch.h"
#include "OpeningScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "GameObject/Player/CoroKun.h"
#include "ExternalLib/Easing/Easing.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/Array/Array.h"

using namespace OtobeLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace ExternalLib;

//�X�N���[���O�̍��W
const Vector3 OpeningScene::BEFORE_POS = Vector3(1550.0f, 75.0f, 0.0f);

//�X�N���[����̍��W
const Vector3 OpeningScene::AFTER_POS = Vector3(-1300.0f, 75.0f, 0.0f);

//�R���X�g���N�^
OpeningScene::OpeningScene():
	m_textureManager(nullptr),//�摜�̊Ǘ��N���X
	m_time(0.0f),//�C�[�W���O�֐��̎���
	m_direct(NULL),//�x�N�g���̌���
	m_ifState(nullptr),//�֐����Ŏ��s����֐�
	m_novel(nullptr)//�m�x���̊Ǘ��N���X
{
}

//����������
void OpeningScene::Initialize()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//��������
	this->Create();

	//�Y������ݒ肷��
	this->AddData1(NULL);

	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�t�F�[�h�C�����s��
	state->SetFadeState(SceneState::FADEIN);

	//�t�F�[�h�̉��Z���x��ݒ肷��
	state->SetFadeSpeed(SceneState::ALFA_SPEED);

	//�m�x���̊Ǘ��N���X�𐶐�����
	m_novel = std::make_unique<Novel>();
	m_novel->Initialize(L"FileData/OpeningStory.csv", m_textureManager.get());

	//�I�����̏����l��ݒ肷��
	staticData->Add("�I����", (std::string)"�f�t�H���g");

	//�e�L�X�g�f�[�^�̃p�X��ݒ肷��
	staticData->Add("�X�g�[���[�p�X", (std::string)"FileData/SkipRequest.csv");

	//BGM���Đ�����
	soundManager->Play(CRI_CUESHEET_0__48_�I�[�v�j���OBGM);
}

//�X�V����
void OpeningScene::Update()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//�t�@�C�����擾����
	Stream* file = Stream::GetInstance();

	//�t�F�[�h�A�E�g���I�������
	if (state->GetFade() == SceneState::FADERESULT)
	{
		//�I�[�v�j���O���������̎�
		if (staticData->Get_StringData("�I�[�v�j���O") == "������")
		{
			//�X�e�[�W�Z���N�g�ɐ؂�ւ���
			Manager->ChangeScene("StageSelect");

			//�I�[�v�j���O�f�[�^��ύX����
			staticData->Add("�I�[�v�j���O", (std::string)"�����ς�");

			//�t�@�C�����J��
			if(file->Open(L"OpeningInfo.csv", Stream::FILE::STREAM_OFSTREAM))
			{
				//�t�@�C���̏㏑���p�̔z���錾����
				std::vector<std::vector<std::string>> outputData(2, std::vector<std::string>(1, ""));

				//�t�@�C���ɏ㏑������p�����[�^�[��������
				outputData[0][0] = "�I�[�v�j���O";
				outputData[1][0] = staticData->Get_StringData("�I�[�v�j���O");

				//�t�@�C���ɏ㏑������
				file->Data_Input(outputData);

				//�t�@�C�������
				file->Close();
			}
		}
		//�I�[�v�j���O�������ς݂̎�
		else
		{
			//�^�C�g���ɖ߂�
			Manager->ChangeScene("Title");
		}
	}

	//�L�[���͑҂��̎�
	if (state->GetFade() != SceneState::WAITKEY)return;

	//�摜�̍X�V����
	m_textureManager->Update();

	//�L�[�̍X�V
	this->UpdateInput();

	//�C�[�W���O�֐��̍X�V����
	this->UpdateEasing();

	//�m�x���̍X�V����
	m_novel->Update();
}

//�`�揈��
void OpeningScene::Render()
{
	//�`��n�̏����擾
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//�t�H���g���擾����
	TextureFont* font = TextureFont::GetInstance();

	//�`��̐ݒ�
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//�摜�̕`�揈��
	m_textureManager->Render();

	//�t�H���g��`�悷��

	//SPACE�L�[��`��
	font->DrawFormatString("SPACE�L�|�F��b��i�߂�", Vector2(20.0f, 675.0f), 1.0f, Colors::Black);

	//ESC�L�[��`��
	font->DrawFormatString("ESC�L�|�F��b���X�L�b�v����", Vector2(700.0f, 675.0f), 1.0f, Colors::Black);

	//�`��̏I��
	Graph->GetSpriteBatch()->End();
}

//�I������
void OpeningScene::Finalize()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//BGM���~����
	soundManager->Stop(CRI_CUESHEET_0__48_�I�[�v�j���OBGM);
}

//��������
void OpeningScene::Create()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager�𐶐�����
	m_textureManager = std::make_unique<TextureManager>();

	//�g�p����摜���擾����
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");
	ITextureBase* arrow = resourceManager->GetTexture("���");
	ITextureBase* stageBack = resourceManager->GetTexture("�X�e�[�W�Z���N�g�w�i");

	//�w�i�𐶐�����
	std::unique_ptr<ITextureBase> pback = std::make_unique<TextureBase>(stageBack);

	//TextureManager�ɒǉ�����
	m_textureManager->Add(std::move(pback));

	for (int i = 1; i <= SCROLL_NUM; i++)
	{
		//�g�p����e�N�X�`�����擾����
		ITextureBase* scroll = resourceManager->GetTexture("�v�����[�O" + std::to_string(i));

		//�X�N���[���p�̃e�N�X�`���𐶐�
		std::unique_ptr<ITextureBase> pscroll = std::make_unique<TextureBase>(scroll);
		pscroll->SetPosition(BEFORE_POS);
		m_scrollTexture.push_back(pscroll.get());

		//TextureManager�ɒǉ�����
		m_textureManager->Add(std::move(pscroll));
	}

	//��ʉ�����UI���쐬����
	std::unique_ptr<ITextureBase> pbotomUI = std::make_unique<TextureBase>(whiteBack);
	pbotomUI->SetPosition(Vector3(0.0f, 665.0f, 0.0f));
	pbotomUI->SetRect(RECT{ 0,0,1280,60 });

	//TextureManager�ɒǉ�����
	m_textureManager->Add(std::move(pbotomUI));

	//�P���ڂ̉摜�̂ݕ␳����
	m_scrollTexture[0]->SetPosition(Vector3(150.0f, 75.0f, 0.0f));
}

void OpeningScene::UpdateEasing()
{
	//�X�e�[�g�̏���

	//switch���ŏ����𕪂���
	switch (this->GetNowData2())
	{
	case BACKOUT::NONE:
		this->State_None();
		break;
	case BACKOUT::ADMISSON:
		this->State_Admisson();
		break;
	case BACKOUT::EXIT:
		this->State_Exit();
		break;
	case BACKOUT::RESULT:
		this->State_Result();
		break;
	}
}

void OpeningScene::State_None()
{
	//State_None�̏���

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneState���擾����
	SceneState* sceneState = sceneManager->GetState();

	//�L�[�{�[�h�̏�Ԃ��擾
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//Story���擾����
	Story* story = m_novel->GetStory();

	//TextWindow���擾����
	TextWindow* textWindow = m_novel->GetTextWindow();

	//���ݓǂ܂�Ă���e�L�X�g���擾����
	std::string text = story->GetStoryText(textWindow->GetDrawCount());

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�ǂ܂�Ă���e�L�X�g��FF�̂Ƃ�
	if (text == "FF")
	{
		//�x�N�g���̌�����ݒ肷��
		m_direct = 1;

		//�o�ߎ��Ԃ�ݒ肷��
		m_time = CoroKun::EASING_MAX;

		//�X�e�[�g��؂�ւ���
		this->AddData2(BACKOUT::ADMISSON);

		//�֐��I�u�W�F�N�g��ݒ肷��
		m_ifState = [&](int& nextState)
		{
			//�̈�O���Q�Ƃ��悤�Ƃ���
			if (nextState >= (int)m_scrollTexture.size() - 1)
			{
				nextState = (int)m_scrollTexture.size() - 1;
			}
		};

		//�X�N���[���ΏۈȊO�������ʒu�ɖ߂�
		this->OtherInitPos(this->GetNowData1(), BEFORE_POS);

		if (this->GetNowData1() != (int)m_scrollTexture.size() - 1)
		{
			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__08_�X���C�h);
		}

	}

	//�e�L�X�g�̊J�n�̋L���̎�
	if (text == "S" + std::to_string(this->GetNowData1() + 1))
	{
		//None�̎������A�m�x����i�߂�
		textWindow->SetDrawCheck(true);
	}

	//�������I���̎�
	if (text == "END")
	{
		//�t�F�[�h�A�E�g����
		sceneState->SetFadeState(SceneState::FADEOUT);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__24_�X�e�[�W�t�F�[�h�A�E�g);
	}

	//�X�L�b�v����Ƃ�
	if (staticData->Get_StringData("�I����") == "�͂�")
	{
		//�t�F�[�h�A�E�g����
		sceneState->SetFadeState(SceneState::FADEOUT);

		//�I����������������
		staticData->Add("�I����", (std::string)"�f�t�H���g");

		//SE���~�߂�
		soundManager->Stop(CRI_CUESHEET_0__47_���b�Z�[�W�\��);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__24_�X�e�[�W�t�F�[�h�A�E�g);
	}
	//�L�����Z�������Ƃ�
	else if (staticData->Get_StringData("�I����") == "������")
	{
		//�I����������������
		staticData->Add("�I����", (std::string)"�f�t�H���g");

		//SE���~�߂�
		soundManager->Play(CRI_CUESHEET_0__47_���b�Z�[�W�\��);
	}

}

void OpeningScene::State_Admisson()
{
	//State_Admisson�̏���

	//�x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//TextWindow���擾����
	TextWindow* textWindow = m_novel->GetTextWindow();

	//�X�N���[�����Ă���Ƃ��́A�m�x���̍X�V���~�߂�
	textWindow->SetDrawCheck(false);
	textWindow->SetActive(false);

	//�O��̃t���[������̌o�ߎ��Ԃ��擾����
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//���݂̓Y�������擾����
	int nowState = this->GetNowData1();

	//���̓Y�������擾����
	int nextState = nowState + m_direct;

	//�ݒ肳�ꂽif���̏������s��
	m_ifState(nextState);

	//�X�e�[�W�̉摜���擾����
	ITextureBase* nowStageTexture = m_scrollTexture[nowState];

	//���̃X�e�[�W�̉摜���擾����
	ITextureBase* nextStageTexture = m_scrollTexture[nextState];

	//�X�N���[�����Ȃ��ꍇ
	if (nowStageTexture == nextStageTexture)
	{
		//�X�e�[�g��؂�ւ���
		this->AddData2(BACKOUT::RESULT);

		//�ȍ~�̏����͍s��Ȃ�
		return;
	}

	//�x�N�g����ݒ肷��
	vel.x = Easing::InExp(m_time, CoroKun::EASING_TOTALTIME, CoroKun::EASING_MAX) * 250.0f * (m_direct * -1);
	nowStageTexture->SetVelocity(vel);
	nextStageTexture->SetVelocity(vel);

	//�o�ߎ��Ԃ��X�V����
	m_time -= elapsedTime * 2.0f;

	//���Ԃ��o�߂�����A�X�e�[�g��؂�ւ���
	if (m_time <= CoroKun::EASING_MIN)
	{
		m_time = CoroKun::EASING_MIN;
		nowStageTexture->SetVelocity(Vector3::Zero);
		nextStageTexture->SetVelocity(Vector3::Zero);
		this->AddData2(BACKOUT::RESULT);
	}
}

void OpeningScene::State_Exit()
{
	//State_Exit�̏���
}

void OpeningScene::State_Result()
{
	//State_Result�̏���

	//���݂̓Y�������擾����
	int nowState = this->GetNowData1();

	//���̓Y�������擾����
	int nextState = nowState + m_direct;

	//if���̏���
	m_ifState(nextState);

	//�Y�������Đݒ肷��
	this->AddData1(nextState);

	//�X�e�[�g��؂�ւ���
	this->AddData2(BACKOUT::NONE);

	Story* story = m_novel->GetStory();

	//TextWindow���擾����
	TextWindow* textWindow = m_novel->GetTextWindow();

	//�t���[����������������
	textWindow->SetFrameCount(5);
	textWindow->SetActive(true);
}

void OpeningScene::OtherInitPos(const int& target, const Vector3& pos)
{
	for (int i = 0; i < (int)m_scrollTexture.size(); i++)
	{
		//�^�[�Q�b�g�ȊO�̂��̂ł���Ƃ�
		if (i != target)
		{
			//�X�e�[�W�摜���擾
			ITextureBase* stage = m_scrollTexture[i];

			//���W��ݒ肷��
			stage->SetPosition(pos);
		}
	}
}

//�L�[�̍X�V����
void OpeningScene::UpdateInput()
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

	//Story���擾����
	Story* story = m_novel->GetStory();

	//TextWindow���擾����
	TextWindow* textWindow = m_novel->GetTextWindow();

	//ESC�L�[�������ꂽ��
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//�I������Scene��ǉ���������
		sceneManager->PushScene("Choices");

		//SE���~����
		soundManager->Stop(CRI_CUESHEET_0__47_���b�Z�[�W�\��);
	}
}
