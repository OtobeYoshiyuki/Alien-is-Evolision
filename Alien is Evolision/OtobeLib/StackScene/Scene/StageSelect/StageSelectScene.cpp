#include "pch.h"
#include "StageSelectScene.h"
#include "ExternalLib/Easing/Easing.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;
using namespace ExternalLib;

//�X�N���[���O�̍��W
const Vector3 StageSelectScene::BEFORE_POS = Vector3(1550.0f, 150.0f, 0.0f);

//�X�N���[����̍��W
const Vector3 StageSelectScene::AFTER_POS = Vector3(-1300.0f, 150.0f, 0.0f);

StageSelectScene::StageSelectScene():
	m_TextureManager(nullptr),//�摜�̊Ǘ��N���X
	m_time(0.0f),//�C�[�W���O�֐��̌o�ߎ���
	m_direct(NULL),//�x�N�g���̌���
	m_ifState(nullptr),//�֐��I�u�W�F�N�g
	m_rightArrow(nullptr),//�E���̉摜
	m_leftArrow(nullptr),//�����̉摜
	m_spaceBackTexture(nullptr)//�X�y�[�X�L�[�̔w�ʂ̃e�N�X�`��
{
}

void StageSelectScene::Initialize()
{
	//�t�@�C�����擾����
	Stream* file = Stream::GetInstance();

	//��������
	this->Create();

	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneState���擾����
	SceneState* sceneState = sceneManager->GetState();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�t�F�[�h�C�����s��
	sceneState->SetFadeState(SceneState::FADEIN);

	//�Y������ݒ肷��
	this->AddData1(NULL);

	//�X�e�[�W�̃N���A�󋵂�ǂݍ���
	//�t�@�C�����J������A�J��
	if (file->Open(L"FileData/StageClear.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^���擾����
		m_clearInfo = file->Load_Data();

		//�t�@�C�������
		file->Close();
	}

	//Adx2���擾����
	ADX2* adx2 = ADX2::GetInstance();

	//BGM���Đ�����
	soundManager->Play(CRI_CUESHEET_0__02_�X�e�[�W�Z���N�g�pBGM_);
}

void StageSelectScene::Update()
{
	//SceneManager���擾����
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneState���擾����
	SceneState* sceneState = sceneManager->GetState();

	//�L�[�{�[�h�̏�Ԃ��擾
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//�t�F�[�h�A�E�g������������A�^�C�g���Ɉڍs����
	if (sceneState->GetFade() == SceneState::FADERESULT)
	{
		sceneManager->ChangeScene("Play");
	}

	//�L�[���͑҂��ȊO�̎��́A�������΂�
	if (sceneState->GetFade() != SceneState::WAITKEY)return;

	//TextureManager�̍X�V
	m_TextureManager->Update();

	//�X�e�[�W�̃X�N���[���̏���
	this->UpdateEasing();
}

void StageSelectScene::Render()
{
	//�`��n�̏����擾
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//�t�H���g���擾
	TextureFont* font = TextureFont::GetInstance();
	
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//TextureManager�̕`��
	m_TextureManager->Render();

	for (int i = 0;i < (int)m_stageTexture.size();i++)
	{
		//�X�e�[�W�摜�̍��W���擾����
		Vector3 pos = m_stageTexture[i]->GetPosition();

		//�t�H���g��`�悷��
		font->DrawFormatString(m_clearInfo[(int)STAGEINFO::STAGENAME][i], Vector2(pos.x + 250.0f, 50.0f), 3.0f, Colors::Black);
	}

	if (m_spaceBackTexture->NowBlinking())
	{
		//�X�y�[�X�L�[�̕`��
		font->DrawFormatString("PUSH SPACE", Vector2(350.0f, 600.0f), 2.0f, Colors::Black);
	}

	Graph->GetSpriteBatch()->End();
}

void StageSelectScene::Finalize()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//BGM���~�߂�
	soundManager->Stop(CRI_CUESHEET_0__02_�X�e�[�W�Z���N�g�pBGM_);
}

void StageSelectScene::Create()
{
	//ResourceManager���擾
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager�𐶐�
	m_TextureManager = std::make_unique<TextureManager>();

	//�g�p����摜���擾
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");
	ITextureBase* stageBack = resourceManager->GetTexture("�X�e�[�W�Z���N�g�w�i");
	ITextureBase* arrow = resourceManager->GetTexture("���");

	//�w�i�̉摜�𐶐�
	std::unique_ptr<ITextureBase> pback = std::make_unique<TextureBase>(stageBack);

	//TextureManager�ɒǉ�
	m_TextureManager->Add(std::move(pback));

	for (int i = 1; i <= 4; i++)
	{
		//�g�p����摜���擾
		ITextureBase* stageTex = resourceManager->GetTexture("�X�e�[�W�Z���N�g" + std::to_string(i));

		//�X�e�[�W1����4�̉摜�𐶐�
		std::unique_ptr<ITextureBase> pstage = std::make_unique<TextureBase>(stageTex);
		pstage->SetPosition(Vector3(1500.0f, 150.0f, 0.0f));
		pstage->SetScale(Vector3(0.75f));
		m_stageTexture.push_back(pstage.get());

		//TextureManager�ɒǉ�
		m_TextureManager->Add(std::move(pstage));
	}


	//����4�̃X�e�[�W�́A���J���ɂ���
	//���J���̉摜
	std::unique_ptr<ITextureBase> punopened = std::make_unique<TextureBase>(whiteBack);
	//���W��ݒ肷��
	punopened->SetPosition(Vector3(1500.0f, 150.0f, 0.0f));
	//RECT��ݒ肷��
	punopened->SetRect(RECT{ 0,0,LONG(m_stageTexture[0]->GetRect().right * 0.75f),
		LONG(m_stageTexture[0]->GetRect().bottom * 0.75f) });
	//�F��ݒ肷��
	punopened->SetColor(Colors::Black);
	//���|�C���^��o�^����
	m_stageTexture.push_back(punopened.get());
	//TextureManager�ɒǉ�
	m_TextureManager->Add(std::move(punopened));

	//�X�e�[�W1�̂ݏ���������
	m_stageTexture[0]->SetPosition(Vector3(150.0f, 150.0f, 0.0f));

	//���̖��𐶐�����
	std::unique_ptr<Arrow> pArrowLeft = std::make_unique<Arrow>(arrow);
	pArrowLeft->InitArrow(Vector3(0.5f), Vector3(1.2f), Vector3(0.5f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowLeft->SetPosition(Vector3(100.0f, 360.0f, 0.0f));
	pArrowLeft->SetOrigin(pArrowLeft->AskOrigin());
	pArrowLeft->SetAngle(180.0f);
	pArrowLeft->SetColor(Colors::Red);
	m_leftArrow = pArrowLeft.get();

	//�E�̖��𐶐�����
	std::unique_ptr<Arrow> pArrowRight = std::make_unique<Arrow>(arrow);
	pArrowRight->InitArrow(Vector3(0.5f), Vector3(1.2f), Vector3(0.5f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowRight->SetPosition(Vector3(1150.0f, 360.0f, 0.0f));
	pArrowRight->SetOrigin(pArrowRight->AskOrigin());
	pArrowRight->SetColor(Colors::Red);
	m_rightArrow = pArrowRight.get();

	//�X�y�[�X�L�[�̔w�ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<TextureBase> pspaceBack = std::make_unique<TextureBase>(whiteBack);
	pspaceBack->SetPosition(Vector3(300.0f, 550.0f, 0.0f));
	pspaceBack->SetRect(RECT{ 0,0,750,150 });
	pspaceBack->SetTarget(0.5f);
	pspaceBack->SetBlink(true);
	m_spaceBackTexture = pspaceBack.get();

	//TextureManager�ɒǉ�
	m_TextureManager->Add(std::move(pArrowLeft));
	m_TextureManager->Add(std::move(pArrowRight));
	m_TextureManager->Add(std::move(pspaceBack));
}

void StageSelectScene::UpdateEasing()
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

void StageSelectScene::State_None()
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

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�E�L�[�������ꂽ��
	if (keyState.Right)
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
			if (nextState >= (int)m_stageTexture.size())
			{
				nextState = NULL;
			}
		};

		//�X�N���[���ΏۈȊO�������ʒu�ɖ߂�
		this->OtherInitPos(this->GetNowData1(),BEFORE_POS);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__08_�X���C�h);
	}

	//���L�[�������ꂽ��
	if (keyState.Left)
	{
		//�x�N�g���̌�����ݒ肷��
		m_direct = -1;

		//�o�ߎ��Ԃ�ݒ肷��
		m_time = CoroKun::EASING_MAX;

		//�X�e�[�g��؂�ւ���
		this->AddData2(BACKOUT::ADMISSON);

		//�֐��I�u�W�F�N�g��ݒ肷��
		m_ifState = [&](int& nextState)
		{
			//�̈�O���Q�Ƃ��悤�Ƃ���
			if (nextState < NULL)
			{
				nextState = (int)m_stageTexture.size() - 1;
			}
		};

		//�X�N���[���ΏۈȊO�������ʒu�ɖ߂�
		this->OtherInitPos(this->GetNowData1(), AFTER_POS);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__08_�X���C�h);
	}

	//�X�y�[�X�L�[�őJ�ڂ���
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//�I�������X�e�[�W���������Ă���
		if (m_clearInfo[(int)STAGEINFO::RELEASEINFO][this->GetNowData1()] == "����ς�")
		{
			//�ǂݍ��ރX�e�[�W��ݒ肷��
			staticData->Add("�X�e�[�W", std::to_string(this->GetNowData1() + 1));
			staticData->Add("�X�e�[�W",this->GetNowData1() + 1);

			//Play�ɑJ�ڂ���
			sceneState->SetFadeState(SceneState::FADEOUT);

			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__24_�X�e�[�W�t�F�[�h�A�E�g);
		}
		//�{�X�X�e�[�W
		else
		{
			//SE���Đ�����
			soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
		}
	}
}

void StageSelectScene::State_Admisson()
{
	//State_Admisson�̏���

	//�x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�O��̃t���[������̌o�ߎ��Ԃ��擾����
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//���݂̓Y�������擾����
	int nowState = this->GetNowData1();

	//���̓Y�������擾����
	int nextState = nowState + m_direct;

	//�ݒ肳�ꂽif���̏������s��
	m_ifState(nextState);

	//�X�e�[�W�̉摜���擾����
	ITextureBase* nowStageTexture = m_stageTexture[nowState];

	//���̃X�e�[�W�̉摜���擾����
	ITextureBase* nextStageTexture = m_stageTexture[nextState];

	//�x�N�g����ݒ肷��
	vel.x = Easing::InExp(m_time, CoroKun::EASING_TOTALTIME,CoroKun::EASING_MAX) * 250.0f * (m_direct * -1);
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

void StageSelectScene::State_Exit()
{
	//State_Exit�̏���
}

void StageSelectScene::State_Result()
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
}

void StageSelectScene::OtherInitPos(const int& target, const Vector3& pos)
{
	for (int i = 0; i < (int)m_stageTexture.size(); i++)
	{
		//�^�[�Q�b�g�ȊO�̂��̂ł���Ƃ�
		if (i != target)
		{
			//�X�e�[�W�摜���擾
			ITextureBase* stage = m_stageTexture[i];

			//���W��ݒ肷��
			stage->SetPosition(pos);
		}
	}
}
