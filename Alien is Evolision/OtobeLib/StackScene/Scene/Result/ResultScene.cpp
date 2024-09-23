#include "pch.h"
#include "ResultScene.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "GameObject/Player/CoroKun.h"
#include "ExternalLib/Easing/Easing.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;
using namespace ExternalLib;

//�ŏ��ɕ\�������UI�ƃX�N���[������UI�̋���
const float ResultScene::UI_INTERVAL = 1230.0f;

ResultScene::ResultScene():
	m_TextureManager(nullptr),//�摜�̊Ǘ��N���X
	m_timerJudge(nullptr),//�N���A�^�C��
	m_score(nullptr),//�l���X�R�A
	m_totalScore(nullptr),//�g�[�^���X�R�A
	m_time(0.0f),//�o�ߎ���
	m_pushCount(NULL),//�L�[����������
	m_func(nullptr),//�֐��I�u�W�F�N�g
	m_fontX(0.0f),//�t�H���g��X���W
	m_spacekeyBlink(nullptr)//�X�y�[�X�L�[�̓_�ł̐���
{
}


ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�t�F�[�h�C�����s��
	state->SetFadeState(SceneState::FADEIN);

	//�t�F�[�h�̉��Z���x��ݒ肷��
	state->SetFadeSpeed(SceneState::ALFA_SPEED);

	//BGM���Đ�����
	soundManager->Play(CRI_CUESHEET_0__04_���U���g�pBGM);

	//��������
	this->Create();

}

void ResultScene::Update()
{
	//SceneManager�̏�Ԃ��擾
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//�L�[�{�[�h�̏��
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//�t�F�[�h�A�E�g���I��
	if (state->GetFade() == SceneState::FADERESULT)
	{
		Manager->ChangeScene("Title");
	}

	//�摜�̊Ǘ��N���X�̍X�V
	m_TextureManager->Update();

	//�X�R�A�̍X�V
	m_score->Update();
	m_totalScore->Update();

	//�N���A�^�C���̍X�V
	m_timerJudge->Update();

	for (std::unique_ptr<Ranking>& ranking : m_rankings)
	{
		//�����L���O�̍X�V
		ranking->Update();
	}

	//�_�ł̍X�V
	m_spacekeyBlink->Update();

	//�X�e�[�g�̍X�V����
	this->UpdateState();
}

void ResultScene::Render()
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

	m_TextureManager->Render();

	//�t�H���g��`��

	//�N���A�^�C��
	font->DrawFormatString("�̂���^�C���F" + m_timerJudge->AskTimeStringMinute() + "�F" + m_timerJudge->AskTimeStringSecound(),
		Vector2(m_fontX, 200.0f), 1.5f, Colors::Black);

	//�X�R�A�̒l��ҏW����
	int score = (int)m_score->GetScore();
	std::string scoreStr1 = std::to_string(score);
	scoreStr1 = font->Convert_NumberOfDigits(scoreStr1, 7, "0", 1);

	int totalScore = (int)m_totalScore->GetScore();
	std::string scoreStr2 = std::to_string(totalScore);
	scoreStr2 = font->Convert_NumberOfDigits(scoreStr2, 7, "0", 1);

	//�l���X�R�A
	font->DrawFormatString("�����Ƃ��X�R�A�F" + scoreStr1, Vector2(m_fontX, 300.0f), 1.5f, Colors::Black);

	//�g�[�^���X�R�A
	font->DrawFormatString("�g�|�^���X�R�A�F" + scoreStr2, Vector2(m_fontX, 400.0f), 1.5f, Colors::Black);

	//�����L���O�̕`��
	for (int i = 0; i < (int)m_rankings.size(); i++)
	{
		if (m_rankings[i]->GetBlinking()->NowBlinking())
		{
			//�X�R�A�̒l��ҏW����
			int score = (int)m_rankings[i]->GetScore()->GetScore();
			std::string scoreStr = std::to_string(score);
			scoreStr = font->Convert_NumberOfDigits(scoreStr, 7, "0", 1);

			font->DrawFormatString("No." + std::to_string(i + 1), Vector2(m_fontX + UI_INTERVAL, 100.0f + (200.0f * i)), 1.0f, m_rankings[i]->GetColor());
			font->DrawFormatString("���׃��F" + m_rankings[i]->GetLevel(), Vector2(m_fontX + UI_INTERVAL, 150.0f + (200.0f * i)), 1.0f, m_rankings[i]->GetColor());
			font->DrawFormatString("�X�R�A�F" + scoreStr, Vector2(m_fontX + UI_INTERVAL, 200.0f + (200.0f * i)), 1.0f, m_rankings[i]->GetColor());
			font->DrawFormatString("�N���A�^�C���F" +
				m_rankings[i]->GetTimer()->AskTimeStringMinute() + "�F" + m_rankings[i]->GetTimer()->AskTimeStringSecound(),
				Vector2(m_fontX + UI_INTERVAL, 250.0f + (200.0f * i)), 1.0f, m_rankings[i]->GetColor());
		}
	}

	//�X�y�[�X�L�[��`��
	if (!m_spacekeyBlink->NowBlinking())
	{
		font->DrawFormatString("PUSH SPACE", Vector2(350.0f, 520.0f), 1.5f, Colors::Black);
	}

	Graph->GetSpriteBatch()->End();
}

void ResultScene::Finalize()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//BGM���~�߂�
	soundManager->Stop(CRI_CUESHEET_0__04_���U���g�pBGM);
}

void ResultScene::Create()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�t�@�C�����擾����
	Stream* file = Stream::GetInstance();

	//�K�v�ȃ��\�[�X���Q�Ƃ���
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* ResultTexture = resourceManager->GetTexture("Result");
	std::unique_ptr<ITextureBase>pTexture = std::make_unique<TextureBase>(ResultTexture);

	//Manager�N���X�ɓo�^����
	m_TextureManager = std::make_unique<TextureManager>();
	m_TextureManager->Add(std::move(pTexture));

	//�^�C�}�[�𐶐�����
	m_timerJudge = std::make_unique<TimerJudge>();
	m_timerJudge->SetLimitTimer(staticData->Get_IntData("�̂���^�C��"));
	m_timerJudge->SetCorrection(60.0f);
	m_timerJudge->SetTimerUp(false);

	//�l���X�R�A�𐶐�����
	m_score = std::make_unique<Score>();
	m_score->SetSaving(NULL);
	m_score->SetScore(staticData->Get_IntData("�X�R�A"));
	m_score->SetAdd(-1);
	m_score->SetSoundCheck(true);

	//�g�[�^���X�R�A�𐶐�����
	m_totalScore = std::make_unique<Score>();
	m_totalScore->SetSaving(NULL);
	m_totalScore->SetScore(0);
	m_totalScore->SetSoundCheck(true);

	//�X�y�[�X�L�[�̓_�ŃN���X�𐶐�����
	m_spacekeyBlink = std::make_unique<Blinking>();
	m_spacekeyBlink->SetTarget(0.5f);
	m_spacekeyBlink->SetBlink(false);

	//�����L���O�f�[�^��ǂݍ���
	//�t�@�C�����J������A�J��
	if (file->Open(L"RankingData.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^�����[�h����
		m_rankingData = file->Load_Data();

		//�t�@�C�������
		file->Close();
	}

	//�t�H���g��X���W��ݒ肷��
	m_fontX = 100.0f;

	for (int i = 0; i < (int)RESULT_INFO::RANKING_NUM; i++)
	{
		//�����L���O�𐶐�����
		std::unique_ptr<Ranking> pranking = std::make_unique<Ranking>();

		//Move����
		m_rankings.push_back(std::move(pranking));
	}

	//�X�e�[�W1����4�̒�����A�����o���f�[�^�����肷��
	int stageSelect = 10 * (staticData->Get_IntData("�X�e�[�W") - 1) + (int)STAGE1;

	//���[�h�����f�[�^����A�ꕔ�����f�[�^�𔲂��o��
	std::vector<std::vector<std::string>> convertArray =
		this->GetSaveDataDoubleArray(stageSelect + 1,
			CREAR_RESULT_RANGE, stageSelect + 3, STAGE_INTERVAL);

	for (int i = 0; i < (int)m_rankings.size(); i++)
	{
		//�p�����[�^�[��ݒ肷��

		//���x����ݒ肷��
		m_rankings[i]->SetLevel(convertArray[i][0]);

		//�X�R�A��ݒ肷��
		m_rankings[i]->GetScore()->SetScore(std::stoi(convertArray[i][1]));

		//�N���A�^�C����ݒ肷��
		m_rankings[i]->GetTimer()->SetLimitTimer(std::stoi(convertArray[i][2]));
	}
}

void ResultScene::UpdateState()
{
	//�X�e�[�g�̍X�V����

	//Switch���ŏ����𕪂���
	switch (this->GetNowState())
	{
	case CLEARSTATE::NONE:
		this->State_None();
		break;
	case CLEARSTATE::WAITKEY:
		this->State_WaitKey();
		break;
	case CLEARSTATE::NEXT:
		this->State_Next();
		break;
	case CLEARSTATE::ADMISSON:
		this->State_Admisson();
		break;
	case CLEARSTATE::RANKING:
		this->State_Ranking();
		break;
	}
}

void ResultScene::State_None()
{
	//State_None�̏���

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�O��̃t���[������̌o�ߎ��Ԃ��擾����
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//�_�ł��Ȃ�
	m_spacekeyBlink->SetTargetCount(NULL);
	m_spacekeyBlink->SetBlink(false);

	//1��ڂ̎�
	if (m_pushCount == NULL)
	{
		//�֐��I�u�W�F�N�g��ݒ肷��
		m_func = [&]()
		{
			//�X�R�A�̐ݒ������
			m_score->SetSaving(staticData->Get_IntData("�X�R�A"));
			m_score->SetFrameSave(staticData->Get_IntData("�X�R�A"), 60.0f);
			m_totalScore->SetSaving(staticData->Get_IntData("�X�R�A"));
			m_totalScore->SetFrameSave(staticData->Get_IntData("�X�R�A"), 60.0f);

			//�_�ŊJ�n
			m_spacekeyBlink->SetBlink(true);
		};
	}
	//�Q��ڈȍ~�̎�
	else
	{
		//�֐��I�u�W�F�N�g��ݒ肷��
		m_func = [&]()
		{
			//�X�R�A�̐ݒ������
			m_totalScore->SetSaving(m_timerJudge->GetLimitTimer() * 10);
			m_totalScore->SetFrameSave(m_timerJudge->GetLimitTimer() * 10, 60.0f);

			//�N���A�^�C���̐ݒ������
			m_timerJudge->SetTimerUp(true);

			//�_�ŊJ�n
			m_spacekeyBlink->SetBlink(true);
		};

	}


	//1�b�o��
	if (m_time > 1.0f)
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(CLEARSTATE::WAITKEY);

		//�o�ߎ��Ԃ�����������
		m_time = 0.0f;

		//�ۑ������֐������s
		m_func();

		//�o�ߎ��Ԃ̍X�V�͍s��Ȃ�
		return;
	}

	//�o�ߎ��Ԃ��X�V
	m_time += elapsedTime;
}

void ResultScene::State_WaitKey()
{
	//State_WaitKey�̏���
	
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�L�[�{�[�h�̏��
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//���݂̃X�e�[�g��ݒ肷��
	CLEARSTATE state = this->GetNowState();

	//1��ڂ̎�
	if (m_pushCount == NULL)
	{
		//�X�e�[�g��ݒ肷��
		state = CLEARSTATE::NONE;

		//�֐��I�u�W�F�N�g��ݒ肷��
		m_func = [&]()
		{
			//�X�R�A�̐ݒ������
			m_score->SetScore(0);
			m_score->SetSaving(0);
			m_totalScore->SetScore(staticData->Get_IntData("�X�R�A"));
			m_totalScore->SetSaving(0);
		};
	}
	//�Q��ڈȍ~�̎�
	else
	{
		//�X�e�[�g��ݒ肷��
		state = CLEARSTATE::NEXT;

		//�֐��I�u�W�F�N�g��ݒ肷��
		m_func = [&]() 
		{
			//�^�C�}�[�̐ݒ������
			m_timerJudge->SetLimitTimer(0);
			m_timerJudge->SetTimerUp(false);

			//�X�R�A�̐ݒ������
			m_totalScore->SetScore(m_totalScore->GetScore() + m_totalScore->GetSaving());
			m_totalScore->SetSaving(0); 
		};
	}

	//�X�y�[�X�L�[�������ꂽ��
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//�X�e�[�g��؂�ւ���
		this->SetNowState(state);

		//pushCount���X�V����
		m_pushCount++;

		//�ۑ������֐������s
		m_func();

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__05_���艹);
	}
}

void ResultScene::State_Next()
{
	//State_Next�̏���

	//�t�@�C�����擾����
	Stream* file = Stream::GetInstance();

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�L�[�{�[�h�̏��
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//�X�e�[�W1����4�̒�����A�����o���f�[�^�����肷��
	int stageSelect = 10 * (staticData->Get_IntData("�X�e�[�W") - 1) + (int)STAGE1;

	//�X�y�[�X�L�[�������ꂽ��
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//�����L���O�̓���ւ������������ꍇ�A����ւ���
		for (int i = 0;i < (int)m_rankings.size(); i++)
		{
			//�X�R�A�������L���O�̂��̂��A�傫����΁A
			if (m_totalScore->GetScore() > m_rankings[i]->GetScore()->GetScore())
			{
				//�X�R�A�̏�����P��������
				for (int j = (int)m_rankings.size() - 1; j > i; j--)
				{
					m_rankings[j]->GetScore()->SetScore(m_rankings[j - 1]->GetScore()->GetScore());
					m_rankings[j]->GetTimer()->SetLimitTimer((float)m_rankings[j - 1]->GetTimer()->GetLimitTimer());
					m_rankings[j]->SetLevel(m_rankings[j - 1]->GetLevel());
				}

				//����ւ����s��
				m_rankings[i]->GetScore()->SetScore(m_totalScore->GetScore());
				m_rankings[i]->GetTimer()->SetLimitTimer(staticData->Get_IntData("�N���A�^�C��"));
				m_rankings[i]->SetLevel(staticData->Get_StringData("���x��"));
				m_rankings[i]->GetBlinking()->SetBlink(true);
				m_rankings[i]->SetColor(Colors::Red);

				//�f�[�^���㏑������
				for (int z = 1; z <= 3; z++)
				{
					m_rankingData[stageSelect + (z * 3)][(int)RESULT_INFO::LEVEL] = m_rankings[z - 1]->GetLevel();
					m_rankingData[stageSelect + (z * 3)][(int)RESULT_INFO::SCORE] = std::to_string(m_rankings[z - 1]->GetScore()->GetScore());
					m_rankingData[stageSelect + (z * 3)][(int)RESULT_INFO::CLEATIME] = std::to_string(m_rankings[z - 1]->GetTimer()->GetLimitTimer());
				}

				//�t�@�C�����J��
				if (file->Open(L"RankingData.csv", Stream::FILE::STREAM_OFSTREAM))
				{
					//Csv�`���ɕϊ�����
					file->Data_Input(m_rankingData);

					//�t�@�C�������
					file->Close();
				}

				//����ւ����s������A���[�v�𔲂���
				break;
			}
		}

		//�X�e�[�g��؂�ւ���
		this->SetNowState(CLEARSTATE::ADMISSON);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__05_���艹);
	}
}

void ResultScene::State_Admisson()
{
	//State_Admisson�̏���

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�O��̃t���[������̌o�ߎ��Ԃ��擾����
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//���Ԃ��o��
	if (m_time > CoroKun::EASING_TOTALTIME)
	{
		//���Ԃ���������
		m_time = 0.0f;

		//�X�e�[�g��؂�ւ���
		this->SetNowState(CLEARSTATE::RANKING);

		//�o�ߎ��Ԃ̍X�V�͍s��Ȃ�
		return;
	}

	//���W���X�V����
	m_fontX -= Easing::InExp(m_time, CoroKun::EASING_TOTALTIME) * 120.0f;

	//�o�ߎ��Ԃ̍X�V���s��
	m_time += elapsedTime;
}

void ResultScene::State_Ranking()
{
	//State_Ranking�̏���

	//SceneManager�̏�Ԃ��擾
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* State = Manager->GetState();

	//�L�[�{�[�h�̏��
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�X�y�[�X�L�[�������ꂽ��
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//�t�F�[�h�A�E�g����
		State->SetFadeState(SceneState::FADEOUT);

		//�X�e�[�g��؂�ւ���
		this->SetNowState(CLEARSTATE::RESULT);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__25_���U���g�t�F�[�h�A�E�g);

		//�_�ł��~�߂�
		m_spacekeyBlink->SetTargetCount(NULL);
		m_spacekeyBlink->SetBlink(false);
	}
}

//�w�肵���f�[�^��z��ɂ��ĕԂ�(std::vector<>)
const std::vector<std::string> ResultScene::GetSaveDataArray(const int& data)
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

//�w�肵���͈͂̃f�[�^��2�����z��ɂ��ĕԂ�(std::vector<std::vector<>>)
const std::vector<std::vector<std::string>> ResultScene::GetSaveDataDoubleArray
(const int& first, const int& finishi,const int& target, const int interval)
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
