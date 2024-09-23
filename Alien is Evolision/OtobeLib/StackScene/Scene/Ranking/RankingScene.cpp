#include "pch.h"
#include "RankingScene.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/StackScene/Scene/Result/ResultScene.h"
#include "OtobeLib/StackScene/Scene/StageSelect/StageSelectScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Player/CoroKun.h"
#include "ExternalLib/Easing/Easing.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;
using namespace ExternalLib;

//�X�N���[���O�̍��W
const Vector3 RankingScene::BEFORE_POS = Vector3(1550.0f, 75.0f, 0.0f);

//�X�N���[����̍��W
const Vector3 RankingScene::AFTER_POS = Vector3(-1300.0f, 75.0f, 0.0f);

RankingScene::RankingScene():
	m_TextureManager(nullptr),//�摜�̊Ǘ��N���X
	m_rightArrow(nullptr),//�E���̉摜
	m_leftArrow(nullptr),//�����̉摜
	m_escapeBack(nullptr),//Escape�L�[�̔w�ʂ̉摜
	m_direct(NULL),//�x�N�g���̌���
	m_time(0.0f),//�C�[�W���O�֐��p�̌o�ߎ���
	m_ifState(nullptr)//�֐��I�u�W�F�N�g
{
	//�e�ʂ��m�ۂ���
	m_rankingDatas.resize((int)ResultScene::STAGENUM);

	for (int i = 0; i < (int)ResultScene::STAGENUM; i++)
	{
		for (int j = 0; j < (int)ResultScene::RESULT_INFO::RANKING_NUM; j++)
		{
			//�����L���O�𐶐�����
			std::unique_ptr<Ranking> pranking = std::make_unique<Ranking>();

			//Move����
			m_rankingDatas[i].push_back(std::move(pranking));
		}
	}

	//�t�@�C�����擾����
	Stream* file = Stream::GetInstance();

	//�t�@�C�����J��
	if (file->Open(L"RankingData.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		m_rankingData = file->Load_Data();

		for (int i = 0; i < (int)ResultScene::STAGENUM; i++)
		{
			//�w�肵���͈͂̃f�[�^��񎟌��z��Ŏ擾����
			//���[�h�����f�[�^����A�ꕔ�����f�[�^�𔲂��o��
			std::vector<std::vector<std::string>> convertArray =
				this->GetSaveDataDoubleArray((int)ResultScene::RANKING_INFO::STAGE1 + 1 + (i * 10),
					ResultScene::CREAR_RESULT_RANGE, (int)ResultScene::RANKING_INFO::STAGE1 + 3 + (i * 10),
					ResultScene::STAGE_INTERVAL);
			
			for (int j = 0; j < (int)ResultScene::RESULT_INFO::RANKING_NUM; j++)
			{
				//�p�����[�^�[��ݒ肷��

				//���x����ݒ肷��
				m_rankingDatas[i][j]->SetLevel(convertArray[j][0]);

				//�X�R�A��ݒ肷��
				m_rankingDatas[i][j]->GetScore()->SetScore(std::stoi(convertArray[j][1]));

				//�N���A�^�C����ݒ肷��
				m_rankingDatas[i][j]->GetTimer()->SetLimitTimer(std::stoi(convertArray[j][2]));
			}
		}

		//�t�@�C�������
		file->Close();
	}

	//�X�e�[�W�̃N���A�󋵂�ǂݍ���
	//�t�@�C�����J������A�J��
	if (file->Open(L"FileData/StageClear.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^���擾����
		m_clearInfo = file->Load_Data();

		//�t�@�C�������
		file->Close();
	}
}

void RankingScene::Initialize()
{
	//��������
	this->Create();

	//�Y������ݒ肷��
	this->AddData1(NULL);
}

void RankingScene::Update()
{
	//SceneManager�̏�Ԃ��擾
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//�L�[�{�[�h�̏��
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�摜�̍X�V����
	m_TextureManager->Update();

	//Escape�L�[�������ꂽ��
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//���݂�Scene������
		Manager->PopScene();

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
	}

	//�C�[�W���O�֐��̍X�V����
	this->UpdateEasing();
}

void RankingScene::Render()
{
	//�`��n�̏����擾
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//�t�H���g���擾
	TextureFont* font = TextureFont::GetInstance();

	//�`��̐ݒ�
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//�摜�̕`�揈��
	m_TextureManager->Render();

	//�t�H���g��`�悷��
	for (int i = 0; i < (int)ResultScene::STAGENUM; i++)
	{
		font->DrawFormatString(m_clearInfo[(int)StageSelectScene::STAGEINFO::STAGENAME][i],
			Vector2(m_rankingBack[i]->GetPosition().x + 300.0f, 100.0f), 2.0f, Colors::Red);

		for (int j = 0; j < (int)ResultScene::RESULT_INFO::RANKING_NUM; j++)
		{
			font->DrawFormatString("No." + std::to_string(j + 1) + "  ���׃�" + m_rankingDatas[i][j]->GetLevel(),
				Vector2(m_rankingBack[i]->GetPosition().x + 50.0f, 180.0f + float(j * 150)), 1.3f, Colors::Black);

			//�X�R�A�̒l��ҏW����
			int score = (int)m_rankingDatas[i][j]->GetScore()->GetScore();
			std::string scoreStr = std::to_string(score);
			scoreStr = font->Convert_NumberOfDigits(scoreStr, 7, "0", 1);

			font->DrawFormatString("�X�R�A " + scoreStr + " "
				"�^�C�� " + m_rankingDatas[i][j]->GetTimer()->AskTimeStringMinute() + "�F" + m_rankingDatas[i][j]->GetTimer()->AskTimeStringSecound(),
				Vector2(m_rankingBack[i]->GetPosition().x + 50.0f, 230.0f + float(j * 150)), 1.3f, Colors::Black);
		}
	}
	
	//Escape�L�[�̕`��
	if (m_escapeBack->NowBlinking())
	{
		font->DrawFormatString("PUSH ESCAPE", Vector2(300.0f, 610.0f), 2.0f, Colors::Black);
	}
	
	//�`��̏I��
	Graph->GetSpriteBatch()->End();
}

void RankingScene::Finalize()
{
}

void RankingScene::Create()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager�𐶐�����
	m_TextureManager = std::make_unique<TextureManager>();

	//�g�p����摜���擾����
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");
	ITextureBase* arrow = resourceManager->GetTexture("���");

	//���̖��𐶐�����
	std::unique_ptr<Arrow> pArrowLeft = std::make_unique<Arrow>(arrow);
	pArrowLeft->InitArrow(Vector3(0.5f), Vector3(0.75f), Vector3(0.5f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowLeft->SetPosition(Vector3(100.0f, 360.0f, 0.0f));
	pArrowLeft->SetOrigin(pArrowLeft->AskOrigin());
	pArrowLeft->SetAngle(180.0f);
	pArrowLeft->SetColor(Colors::Aqua);
	m_leftArrow = pArrowLeft.get();

	//�E�̖��𐶐�����
	std::unique_ptr<Arrow> pArrowRight = std::make_unique<Arrow>(arrow);
	pArrowRight->InitArrow(Vector3(0.5f), Vector3(0.75f), Vector3(0.5f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowRight->SetPosition(Vector3(1150.0f, 360.0f, 0.0f));
	pArrowRight->SetOrigin(pArrowRight->AskOrigin());
	pArrowRight->SetColor(Colors::Aqua);
	m_rightArrow = pArrowRight.get();

	//Escape�L�[�̔w�ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<TextureBase> pEscapeBack = std::make_unique<TextureBase>(whiteBack);
	pEscapeBack->SetPosition(Vector3(250.0f, 580.0f, 0.0f));
	pEscapeBack->SetRect(RECT{ 0,0,800,120 });
	pEscapeBack->SetBlink(true);
	pEscapeBack->SetTarget(0.5f);
	m_escapeBack = pEscapeBack.get();

	//�����L���O�̔w�ʂ𐶐�����
	for (int i = 0; i < (int)ResultScene::STAGENUM; i++)
	{
		std::unique_ptr<ITextureBase> pRankingBack = std::make_unique<TextureBase>(whiteBack);
		pRankingBack->SetRect(RECT{ 0,0,960,600 });
		pRankingBack->SetPosition(Vector3(1500.0f, 75.0f, 0.0f));
		pRankingBack->SetColor(Colors::Orange);
		m_rankingBack.push_back(pRankingBack.get());

		//TextureManager�ɒǉ�����
		m_TextureManager->Add(std::move(pRankingBack));
	}

	//�X�e�[�W1�̂ݕ␳����
	m_rankingBack[0]->SetPosition(Vector3(150.0f, 75.0f, 0.0f));

	//TextureManager�ɒǉ�����
	m_TextureManager->Add(std::move(pArrowLeft));
	m_TextureManager->Add(std::move(pArrowRight));
	m_TextureManager->Add(std::move(pEscapeBack));
}

const std::vector<std::string> RankingScene::GetSaveDataArray(const int& data)
{
	//�ϐ�������������
	std::vector<std::string> dataArray;

	//�w�肵���f�[�^���擾����
	for (std::string& info : m_rankingData[data])
	{
		//�f�[�^����̎��́A�������΂�
		if (info.empty())continue;

		//�f�[�^��ǉ�����
		dataArray.push_back(info);
	}

	//�w�肵���f�[�^��Ԃ�
	return dataArray;
}

const std::vector<std::vector<std::string>> RankingScene::GetSaveDataDoubleArray(const int& first, const int& finishi, const int& target, const int interval)
{
	//�ϐ�������������
	std::vector<std::vector<std::string>> dataArray;

	//���[�v�̖�����ݒ肷��
	int fs = first + finishi;

	//�w�肵���͈͂̃f�[�^���擾����
	for (int i = target; i <= fs; i += interval)
	{
		//�f�[�^��ǉ�����
		dataArray.push_back(this->GetSaveDataArray(i));
	}

	//�w�肵���͈͂̃f�[�^��Ԃ�
	return dataArray;
}

void RankingScene::UpdateEasing()
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

void RankingScene::State_None()
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
			if (nextState >= (int)m_rankingBack.size())
			{
				nextState = NULL;
			}
		};

		//�X�N���[���ΏۈȊO�������ʒu�ɖ߂�
		this->OtherInitPos(this->GetNowData1(), BEFORE_POS);

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
				nextState = (int)m_rankingBack.size() - 1;
			}
		};

		//�X�N���[���ΏۈȊO�������ʒu�ɖ߂�
		this->OtherInitPos(this->GetNowData1(), AFTER_POS);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__08_�X���C�h);
	}
}

void RankingScene::State_Admisson()
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
	ITextureBase* nowStageTexture = m_rankingBack[nowState];

	//���̃X�e�[�W�̉摜���擾����
	ITextureBase* nextStageTexture = m_rankingBack[nextState];

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

void RankingScene::State_Exit()
{
	//State_Exit�̏���
}

void RankingScene::State_Result()
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

void RankingScene::OtherInitPos(const int& target, const Vector3& pos)
{
	for (int i = 0; i < (int)m_rankingBack.size(); i++)
	{
		//�^�[�Q�b�g�ȊO�̂��̂ł���Ƃ�
		if (i != target)
		{
			//�X�e�[�W�摜���擾
			ITextureBase* stage = m_rankingBack[i];

			//���W��ݒ肷��
			stage->SetPosition(pos);
		}
	}
}
