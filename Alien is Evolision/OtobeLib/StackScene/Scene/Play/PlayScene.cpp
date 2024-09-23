#include "pch.h"
#include "PlayScene.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "GameObject/Player/CoroKun.h"
#include "GameObject/CollisionUI/CollisionUI.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "GameObject/GameCamera/GameCamera.h"
#include "GameObject/TargetScrollCamera/TargetScrollCamera.h"
#include "GameObject/Item/Drop/ItemDrop.h"
#include "Game.h"
#include "ExternalLib/Easing/Easing.h"
#include "GameObject/Stage/Maptip/HoleFloor/HoleFloor.h"
#include "GameObject/Stage/Maptip/HoleSwitch/HoleSwitch.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace OtobeLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace ExternalLib;

PlayScene::PlayScene():
	m_GameManager(nullptr),//�摜�̊Ǘ��N���X
	m_MainInfoManager(nullptr),//�摜�̊Ǘ��N���X
	m_SubInfoManager(nullptr),//�摜�̊Ǘ��N���X
	m_MiniMapManager(nullptr),//�摜�̊Ǘ��N���X
	m_cameraManager(nullptr),//�J�����̊Ǘ��N���X
	m_BG(nullptr),//�w�i
	m_MiniMap(nullptr),//�~�j�}�b�v
	m_Stage(nullptr),//�X�e�[�W
	m_TimerJudge(nullptr),//���Ԃ̊Ǘ�
	m_StarManager(nullptr),//���̃G�t�F�N�g�̊Ǘ��N���X
	m_LagStart(nullptr),//�X�^�[�g�̃G�t�F�N�g
	m_LagGo(nullptr),//Go�̃G�t�F�N�g
	m_LagFinish(nullptr),//�I���̃G�t�F�N�g
	m_LevelUp(nullptr),//���x���A�b�v�̃G�t�F�N�g
	m_score(nullptr),//�X�R�A
	m_healManager(nullptr),//�񕜃A�C�e���̊Ǘ��N���X
	m_easingTime(0.0f)//�C�[�W���O�֐��̎���
{
}

void PlayScene::Initialize()
{
	//�A�j���[�V�����̃f�[�^��ǂݍ���
	//File�̏����擾����
	Stream* File = Stream::GetInstance();

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�K�E�Z�̃f�[�^��ǉ�����(�f�t�H���g)
	staticData->Add("�K�E�Z", (std::string)"�f�t�H���g");

	//�J�b�g�C���̃f�[�^��ǉ�����(�f�t�H���g)
	staticData->Add("�J�b�g�C��", (std::string)"�f�t�H���g");

	//���݂̃y�[�W����ǉ�����
	staticData->Add("�y�[�W��", 1);

	//�t�@�C�����J������J��
	if (File->Open(L"FileData/AnimationInfomation.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		std::vector<std::vector<std::string>> InputData = File->Load_Data();
		//�f�[�^��ϊ�����
		std::vector<AnimationImporter> ImportData = AnimationImporter::Cast(InputData);
		//�A�j���[�V�����̃f�[�^��ǉ�����
		Animation::Add(ImportData);
		//�t�@�C�������
		File->Close();
	}

	//�t�@�C�����J������J��
	if (File->Open(L"FileData/ItemDrop.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		std::vector<std::vector<std::string>> InputData = File->Load_Data();
		//�f�[�^��ϊ�����
		std::vector<ItemDrop> ImportData = ItemDrop::Cast(InputData);
		//�A�j���[�V�����̃f�[�^��ǉ�����
		ItemDrop::AddItem(ImportData);
		//�t�@�C�������
		File->Close();
	}

	//�t�@�C�����J������J��
	if (File->Open(L"FileData/StatusInfo.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		std::vector<std::vector<std::string>> InputData = File->Load_Data();
		//�f�[�^��ϊ�����
		std::vector<StatusImporter> ImportData = StatusImporter::Cast(InputData);
		//�X�e�[�^�X�̃f�[�^��ǉ�����
		StatusBase::Add(ImportData);
		//�t�@�C�������
		File->Close();
	}

	//�t�@�C���̃p�X��ݒ肷��
	std::wstring enemyFile = L"FileData/EnemyInfo" + std::to_wstring(staticData->Get_IntData("�X�e�[�W")) + L".csv";

	//�t�@�C�����J������J��
	if (File->Open(enemyFile.c_str(),Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		std::vector<std::vector<std::string>> LoadData = File->Load_Data();

		for (int i = 0; i < (int)LoadData.size(); i++)
		{
			//�f�[�^��ǉ�����
			staticData->Add(LoadData[i][0],std::stoi(LoadData[i][2]));
		}

		//�t�@�C�������
		File->Close();
	}

	//�t�@�C�����J������J��
	if (File->Open(L"Save.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		m_saveData = File->Load_Data();

		//�t�@�C�������
		File->Close();
	}

	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//�t�F�[�h�C�����s��
	state->SetFadeState(SceneState::FADEIN);

	//�t�F�[�h�̉��Z���x��ݒ肷��
	state->SetFadeSpeed(SceneState::ALFA_SPEED);

	//�����̏���
	this->Create();

	//BGM���Đ�����
	soundManager->Play(CRI_CUESHEET_0__03_�v���C�pBGM_);
}

void PlayScene::Update()
{
	//�z��̃T�|�[�g�N���X���擾����
	Array* arraySupport = Array::GetInstance();

	//�t�@�C�����擾����
	Stream* file = Stream::GetInstance();

	//�L�[�̏�Ԃ̎擾
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//���݂̃v���C���[�̃X�e�[�^�X���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�J�����̍X�V
	m_cameraManager->Update();

	//�~�j�}�b�v�̍X�V
	m_MiniMap->Update();

	//�t�F�[�h�A�E�g�I����
	if (state->GetFade() == SceneState::FADERESULT)
	{
		//���U���g�̃X�e�[�g�������ꍇ
		if (this->GetPreState() == BACKOUT::RESULT)
		{
			//���U���g�ɑJ�ڂ���
			Manager->ChangeScene("Result");

			//�擾�����A�C�e���̏����L�^����
			std::vector<std::string> itemInfo =
				arraySupport->CreateSingleArray<std::string>((int)m_healManager->GetItem().size(), [&](int i)
					{
						return m_healManager->GetItem()[i]->GetName();
					});

			std::vector<std::string> statusInfo =
				arraySupport->CreateSingleArray<std::string>((int)PLAYER_STATUS::MAX, [&](int i)
					{
						switch ((PLAYER_STATUS)i)
						{
						case PLAYER_STATUS::LEVEL:
							return std::to_string(player->GetLevel());
							break;
						case PLAYER_STATUS::HP:
							return std::to_string(player->GetMaxHP());
							break; 
						case PLAYER_STATUS::MP:
							return std::to_string(player->GetMP());
							break;
						case PLAYER_STATUS::ATACK:
							return std::to_string(player->GetAtack());
							break;
						case PLAYER_STATUS::DEFENCE:
							return std::to_string(player->GetDefence());
							break; 
						case PLAYER_STATUS::SPECIAL:
							return std::to_string(player->GetSpecial());
							break;
						case PLAYER_STATUS::NEXT:
							return std::to_string(player->GetNextLevelPoint());
							break;
						}
					});

			//�A�C�e����1�ȏ�擾���Ă�����
			if ((int)itemInfo.size() > NULL)
			{
				//�Z�[�u�f�[�^���㏑������
				m_saveData[(int)SAVE_DATA::HEAL_INFO] = itemInfo;
			}

			//�Z�[�u�f�[�^���㏑������
			m_saveData[(int)SAVE_DATA::PLAYER_STATUS] = statusInfo;

			//�t�@�C�����J��
			if (file->Open(L"Save.csv", Stream::FILE::STREAM_OFSTREAM))
			{
				//Csv�`���ɕϊ�����
				file->Data_Input(m_saveData);

				//�t�@�C�������
				file->Close();
			}

			//�X�R�A�����Z���̎��́A���f����
			if (m_score->GetSaving() != NULL)
			{
				//�V�����X�R�A��ݒ肷��
				m_score->SetScore(m_score->GetScore() + m_score->GetSaving());

				//���~��0�ɂ���
				m_score->SetSaving(NULL);
			}

			float time = m_TimerJudge->GetLimitTimer();

			//�c��^�C����ۑ�����
			staticData->Add("�̂���^�C��", (int)time);

			time = 300 - time;

			//�N���A�^�C����ۑ�����
			staticData->Add("�N���A�^�C��",(int)time);

			//���x����ۑ�����
			staticData->Add("���x��", statusInfo[(int)PLAYER_STATUS::LEVEL]);

			//�X�R�A��ۑ�����
			staticData->Add("�X�R�A", (int)m_score->GetScore());

		}

		//�^�C�g���̃X�e�[�g�������ꍇ
		else if(this->GetPreState() == BACKOUT::TITLE ||
				player->GetNowData2() == PLAYERMOVE::ANIMEFINISH)
		{
			Manager->ChangeScene("Title");
		}
	}

	//�t�F�[�h�C�����́A�������Ȃ�
	if (state->GetFade() != SceneState::WAITKEY && 
		state->GetFade() != SceneState::FADERESULT)return;

	//X�L�[�������ꂽ��A���j���[�ֈڍs����
	if (keyTrakker.IsKeyPressed(Keyboard::X))
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__39_���j���[���J��);

		Manager->PushScene("Menu");
		return;
	}

	//Esp�L�[�������ꂽ��A�|�[�Y�ֈڍs����
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__39_���j���[���J��);

		Manager->PushScene("Poce");
		return;
	}

	//�e�N�X�`���̍X�V
	m_GameManager->Update();
	m_MainInfoManager->Update();
	m_SubInfoManager->Update();
	m_MiniMapManager->Update();
	m_healManager->Update();

	//�X�e�[�W�̍X�V
	m_Stage->Update();

	//TimeLag�̍X�V
	m_LagStart->Update();
	m_LagGo->Update();
	m_LagFinish->Update();

	//LagEffect�̍X�V
	this->UpdateLag();

	//StarManager�̍X�V
	m_StarManager->Update();

	//�����蔻��̍X�V
	m_CollisionManager->Update();

	//�^�C�}�[�̍X�V
	m_TimerJudge->Update();

	//�X�R�A�̍X�V
	m_score->Update();
}

void PlayScene::Render()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�t�H���g�̏�Ԃ��擾����
	TextureFont* Font = TextureFont::GetInstance();

	//�v���C���[�̃|�C���^���擾
	CoroKun* player = GameContext::Get<CoroKun>();

	//�`��n�̏�Ԃ��擾����
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); },
		m_cameraManager->GetNowCamera()->GetMatrix());

	//�摜�̕`�揈��
	m_GameManager->Render();

	Graph->GetSpriteBatch()->End();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//�摜�̕`�揈��
	m_MainInfoManager->Render();

	////�t���[�����[�g��`�悷��
	//int fps = staticData->Get_IntData("FPS");

	//Font->DrawFormatString("FPS  " + std::to_string(fps), Vector2(1000.0f, 50.0f));

	Graph->GetSpriteBatch()->End();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); },
		m_MiniMap->GetWorld());

	//�摜�̕`�揈��
	m_MiniMapManager->Render();

	Graph->GetSpriteBatch()->End();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//�摜�̕`�揈��
	m_SubInfoManager->Render();

	//HP��`�悷��(�t�H���g)
	std::string playerHpString = std::to_string(player->GetHP()) + "/" + std::to_string(player->GetMaxHP());
	Font->DrawFormatString("���׃�" + std::to_string(player->GetLevel()), Vector2(210, 10), 1.0f, Colors::Black);
	Font->DrawFormatString(playerHpString, Vector2(500.0f, 10.0f), 1.0f, Colors::Black);

	//MP��`�悷��(�t�H���g)
	std::string playerMpString = std::to_string(player->GetMP());
	playerMpString = Font->Convert_NumberOfDigits(playerMpString, 3, "0", 1);
	playerMpString += "/" + std::to_string(player->GetMaxMP());
	Font->DrawFormatString(playerMpString, Vector2(500.0f, 80.0f), 0.8f, Colors::Black);

	//�X�y�V�����|�C���g��`�悷��(�t�H���g)
	std::string playerSpString = std::to_string(player->GetSpecial());
	playerSpString = Font->Convert_NumberOfDigits(playerSpString, 3, "0", 1);
	playerSpString += "/" + std::to_string(player->GetMaxSpecialPoint());
	Font->DrawFormatString(playerSpString, Vector2(620.0f, 115.0f), 0.8f, Colors::Black);

	//�������Ԃ��擾����
	std::string limitTimer = m_TimerJudge->AskTimeStringMinute() + "�F" + m_TimerJudge->AskTimeStringSecound();

	//�������Ԃ�`�悷��
	Font->DrawFormatString(limitTimer, Vector2(850.0f, 10.0f), 1.0f, Colors::Black);

	//�X�R�A��`�悷��
	int score = (int)m_score->GetScore();
	std::string scoreStr = std::to_string(score);
	scoreStr = Font->Convert_NumberOfDigits(scoreStr, 7, "0", 1);
	Font->DrawFormatString(scoreStr, Vector2(1050.0f, 10.0f), 1.0f, Colors::White);

	//�K�E�Z��UI�̗��ʂ��擾����
	FadeTexture* specialUI = player->GetSpecialUI();
	XMVECTORF32 specialColor = Colors::Black;
	specialColor.f[3] = specialUI->GetColor().A();

	//�ʏ�U����UI��`��
	Font->DrawFormatString("Q�F�`���|�W�V���b�g", Vector2(210.0f, 670.0f), 1.0f, Colors::Black);

	//�K�E�Z��UI��`��
	Font->DrawFormatString("Z+V�FSP", Vector2(575.0f, 670.0f), 1.0f, specialColor);

	//���j���[��UI��`��
	Font->DrawFormatString("X�F���j���|", Vector2(820.0f, 670.0f), 1.0f, Colors::Black);

	//�|�[�Y��UI��`��
	Font->DrawFormatString("ESC�F�|�|�Y", Vector2(1025.0f, 670.0f), 1.0f, Colors::Black);

	//�J�n�̃t�H���g��`�悷��
	Font->DrawFormatString(m_LagStart->GetNowString(), Vector2(m_LagStart->GetPosition()), 3.0f, Colors::Aqua);

	Font->DrawFormatString(m_LagGo->GetNowString(), Vector2(m_LagGo->GetPosition()), 10.0f, Colors::White,64);

	Font->DrawFormatString(m_LagFinish->GetNowString(), Vector2(m_LagFinish->GetPosition()), 3.0f, Colors::Red);

	Graph->GetSpriteBatch()->End();
}

void PlayScene::Finalize()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//���Ƃ����̃O���[�v������������
	HoleFloor::ResetHoleGrop();

	//StaticData�̈ꕔ�̃f�[�^���폜����
	for (HoleSwitch* swt : m_Stage->GetHoleSwitches())
	{
		//�f�[�^�����݂���ꍇ
		if (staticData->Request_Data(swt->GetName() + std::to_string(swt->GetSerealNumber())))
		{
			//�X�C�b�`�̃V���A���i���o�[���폜����
			staticData->IntRemove(swt->GetName() + std::to_string(swt->GetSerealNumber()));
		}
		
		//�f�[�^�����݂���ꍇ
		if (staticData->Request_Data(swt->GetName() + std::to_string(swt->GetSerealNumber()) + "Count"))
		{
			//�N���J�E���g���폜����
			staticData->FloatRemove(swt->GetName() + std::to_string(swt->GetSerealNumber()) + "Count");
		}
	}

	//BGM���~����
	soundManager->Stop(CRI_CUESHEET_0__03_�v���C�pBGM_);
}

void PlayScene::Create()
{
	//Manager�N���X����
	this->CreateManager();

	//�w�i�𐶐�
	this->CreateBack();

	//�X�e�[�W�𐶐�
	this->CreateStage();

	//�J�����𐶐�
	this->CreateCamera();

	//�^�C�}�[�𐶐�
	this->CreateTimer();

	//�^�C�����O�G�t�F�N�g�𐶐�
	this->CreateEffect();

	//�X�R�A�𐶐�
	this->CreateScore();
}

//�w�肵���f�[�^��z��ɂ��ĕԂ�(std::vector<>)
const std::vector<std::string> PlayScene::GetSaveDataArray(const int& data)
{
	//�ϐ�������������
	std::vector<std::string> dataArray;

	//�w�肵���f�[�^���擾����
	for (std::string& info : m_saveData[data])
	{
		//�f�[�^����̎��́A�������΂�
		if (info.empty())continue;

		//�f�[�^��ǉ�����
		dataArray.push_back(info);
	}

	//�w�肵���f�[�^��Ԃ�
	return dataArray;
}

void PlayScene::CreateManager()
{
	//GameContext�ɓo�^����
	GameContext::Register<PlayScene>(this);

	//CollisionManager�N���X����
	m_CollisionManager = std::make_unique<CollisionManager>();
	GameContext::Register<CollisionManager>(m_CollisionManager.get());

	//TextureManager�N���X����
	m_GameManager = std::make_unique<TextureManager>();
	m_MainInfoManager = std::make_unique<TextureManager>();
	m_SubInfoManager = std::make_unique<TextureManager>();
	m_MiniMapManager = std::make_unique<TextureManager>();
	m_healManager = std::make_unique<HealManager>();

	//StarManager�N���X����
	m_StarManager = std::make_unique<StarManager>();
	GameContext::Register<StarManager>(m_StarManager.get());
}

void PlayScene::CreateBack()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* BG = resourceManager->GetTexture("�X�e�[�W" + staticData->Get_StringData("�X�e�[�W"));
	ITextureBase* Comand = resourceManager->GetTexture("�R�}���h");
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");

	//�w�i�𐶐�����
	std::unique_ptr<BackGround> pBG = std::make_unique<BackGround>(BG);
	m_BG = pBG.get();

	//�v���C���[�R�}���h�𐶐�����
	//std::unique_ptr<ITextureBase> pComand = std::make_unique<TextureBase>(Comand);

	//�~�j�}�b�v�𐶐�����
	m_MiniMap = std::make_unique<MiniMap>();
	GameContext::Register<MiniMap>(m_MiniMap.get());

	//�~�j�}�b�v�p�̔w�i�𐶐�����
	std::unique_ptr<ITextureBase> pMiniBack = std::make_unique<TextureBase>(whiteBack);
	pMiniBack->SetRect(RECT{ 0,0,480,95 });
	pMiniBack->SetPosition(Vector3(800.0f, 50.0f, 0.0f));
	pMiniBack->SetColor(Colors::Purple);

	//�^�C�}�[��UI�𐶐�����
	std::unique_ptr<TextureBase>pTimerUI = std::make_unique<TextureBase>(whiteBack);
	pTimerUI->SetRect(RECT{ 0,0,240,50 });
	pTimerUI->SetPosition(Vector3(800.0f, 0.0f, 0.0f));
	pTimerUI->SetColor(Colors::Yellow);

	//�X�R�A��UI�𐶐�����
	std::unique_ptr<TextureBase>pScoreUI = std::make_unique<TextureBase>(whiteBack);
	pScoreUI->SetRect(RECT{ 0,0,240,50 });
	pScoreUI->SetPosition(Vector3(1040.0f, 0.0f, 0.0f));
	pScoreUI->SetColor(Colors::Magenta);

	//�o���l�Q�[�W�̗��ʂ𐶐�����
	std::unique_ptr<ITextureBase>pExperienceBack = std::make_unique<TextureBase>(whiteBack);
	pExperienceBack->SetRect(RECT{ 0,0,600,25 });
	pExperienceBack->SetPosition(Vector3(200.0f, 50.0f, 0.0f));
	pExperienceBack->SetColor(Colors::Gray);

	//�R�}���h�Z�Q�[�W�̗��ʂ��쐬����
	std::unique_ptr<ITextureBase>pTechnique = std::make_unique<TextureBase>(whiteBack);
	pTechnique->SetRect(RECT{ 0,0,500,40 });
	pTechnique->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	pTechnique->SetColor(Colors::SteelBlue);
	pTechnique->SetDrawOrder(3);
		
	//�K�E�Z�Q�[�W�̗��ʂ��쐬����
	std::unique_ptr<ITextureBase>pDeathBlow = std::make_unique<TextureBase>(whiteBack);
	pDeathBlow->SetRect(RECT{ 0,0,600,70 });
	pDeathBlow->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	pDeathBlow->SetColor(Colors::HotPink);
	pDeathBlow->SetDrawOrder(4);

	//�Q�[�W�ƃ~�j�}�b�v�̗��ʂ𐶐�����
	std::unique_ptr<CollisionUI> pTopBackUI = std::make_unique<CollisionUI>(whiteBack);
	pTopBackUI->SetRect(RECT{ 0,0,4240,141 });
	pTopBackUI->SetPosition(Vector3(200.0f, 0.0f, 0.0f));
	pTopBackUI->SetDrawOrder(4);
	pTopBackUI->Initialize();

	std::unique_ptr<CollisionUI> pBotomBackUI = std::make_unique<CollisionUI>(whiteBack);
	pBotomBackUI->SetRect(RECT{ 0,0,4240,141 });
	pBotomBackUI->SetPosition(Vector3(200.0f, 661.0f, 0.0f));
	pBotomBackUI->SetDrawOrder(6);
	pBotomBackUI->Initialize();

	//���E�n�_�𐶐�����
	std::unique_ptr<CollisionUI> pLimitUI = std::make_unique<CollisionUI>(whiteBack);
	pLimitUI->SetRect(RECT{ 0,0,200,720 });
	pLimitUI->SetPosition(Vector3(4240.0f, 0.0f, 0.0f));
	pLimitUI->SetDrawOrder(4);
	pLimitUI->Initialize();

	//�R�}���h�̗��ʂ𐶐�����
	std::unique_ptr<CollisionUI> pCommandBackUI = std::make_unique<CollisionUI>(whiteBack);
	pCommandBackUI->SetRect(RECT{ 0,0,200,720 });
	pCommandBackUI->SetDrawOrder(2);
	pCommandBackUI->SetColor(Colors::DarkCyan);
	pCommandBackUI->Initialize();

	//GameManager�ɒǉ�����
	m_GameManager->Add(std::move(pBG));
	m_GameManager->Add(std::move(pLimitUI));

	//MainInfoManager�ɒǉ�����
	m_MainInfoManager->Add(std::move(pTimerUI));
	m_MainInfoManager->Add(std::move(pScoreUI));
	m_MainInfoManager->Add(std::move(pMiniBack));
	m_MainInfoManager->Add(std::move(pTopBackUI));
	m_MainInfoManager->Add(std::move(pBotomBackUI));

	//SubInfoManager�ɒǉ�����
	m_SubInfoManager->Add(std::move(pCommandBackUI));
	//m_SubInfoManager->Add(std::move(pComand));
	m_SubInfoManager->Add(std::move(pExperienceBack));
	m_SubInfoManager->Add(std::move(pDeathBlow));
	m_SubInfoManager->Add(std::move(pTechnique));
}

void PlayScene::CreateStage()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* mapTip = resourceManager->GetTexture("�}�b�v�`�b�v");

	//�X�e�[�W�𐶐�����
	m_Stage = std::make_unique<Stage>();

	//�X�e�[�W�N���X��o�^����
	GameContext::Register<Stage>(m_Stage.get());

	//�w�肵���X�e�[�W�f�[�^��ǂݍ���
	m_Stage->Initialize(L"FileData/Stage" + std::to_wstring(staticData->Get_IntData("�X�e�[�W")) + L"main.csv",
		L"FileData/Stage" + std::to_wstring(staticData->Get_IntData("�X�e�[�W")) + L"sub.csv");
}

void PlayScene::CreateCamera()
{
	//�J�����𐶐�����
	m_cameraManager = std::make_unique<CameraManager>();

	//�֐���o�^����
	m_cameraManager->AddCamera("�Q�[���J����", std::make_unique<GameCamera>);
	m_cameraManager->AddCamera("�X�N���[���J����", std::make_unique<TargetScrollCamera>);

	//�J������ݒ肷��
	m_cameraManager->ChangeCamera("�Q�[���J����");

	//�J�����̃C���X�^���X�𐶐����Ă���
	m_cameraManager->Update();

	//�J������o�^����
	GameContext::Register<CameraManager>(m_cameraManager.get());

	//�~�j�}�b�v�̏��������s��
	//�T�u�f�[�^��ǂݍ���
	m_MiniMap->Initialize(m_Stage->GetSubBeforeData(), m_Stage->GetSubAfterData());

	//���C���f�[�^��ǂݍ���
	m_MiniMap->Initialize(m_Stage->GetMainBeforeData(), m_Stage->GetMainAfterData());

	//�񕜃A�C�e���̏�����
	m_healManager->Initialize();

	//�z�����ݒ肷��
	m_MiniMap->ArraySetUp();

	//�s����X�V���Ă���
	m_MiniMap->Update();
}

void PlayScene::CreateTimer()
{
	//�^�C�}�[�𐶐�����
	m_TimerJudge = std::make_unique<TimerJudge>();

	//�^�C�}�[�̏��������s��
	m_TimerJudge->Initialize(300, false);

	//�^�C�}�[��o�^����
	GameContext::Register<TimerJudge>(m_TimerJudge.get());
}

void PlayScene::CreateEffect()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//LagStart�𐶐�����
	m_LagStart = std::make_unique<TimeLag>();
	//LagStart�̏��������s��
	m_LagStart->Initialize(true, "", Vector3(400.0f, Game::MAX_HEIGHT / 2, 0.0f),
		Vector3::Zero);

	//�`��̊J�n���Ԃ�ݒ肷��
	m_LagStart->SetInTime("", 30);
	m_LagStart->SetInTime("R", 60);
	m_LagStart->SetInTime("RE", 90);
	m_LagStart->SetInTime("REA", 120);
	m_LagStart->SetInTime("READ", 150);
	m_LagStart->SetInTime("READY", 180);
	m_LagStart->SetInTime("READY�I", 210);

	//���ɕ\�����镶����ݒ肷��
	m_LagStart->SetNextString("", "R");
	m_LagStart->SetNextString("R", "RE");
	m_LagStart->SetNextString("RE", "REA");
	m_LagStart->SetNextString("REA", "READ");
	m_LagStart->SetNextString("READ", "READY");
	m_LagStart->SetNextString("READY", "READY�I");

	//�`��̏I�����Ԃ�ݒ肷��
	m_LagStart->SetOutTime(360);

	//�W�����v�̍ۂ̃W�����v�񐔂ƁA�I�v�V�������J�����Ă���
	m_LagStart->GetJumpState()->SetExecute(true);
	m_LagStart->GetJumpState()->SetJumpLimit(120.0f);
	m_LagStart->GetJumpState()->SetJumpSpeed(10.0f);
	m_LagStart->GetJumpState()->SetJumpLimitCount(2);
	m_LagStart->GetJumpState()->SetJumpOption(true);

	//1�t���[���Ɉړ����鑬�x��ݒ肷��
	m_LagStart->GetAutoMoveState()->SetExecute(true);
	m_LagStart->GetAutoMoveState()->SetSpeed(Vector3(4.0f, 4.0f, 0.0f));

	//LagGo�𐶐�����
	m_LagGo = std::make_unique<TimeLag>();
	//LagGo�̏��������s��
	m_LagGo->Initialize(false,"", Vector3(350,250,0),Vector3::Zero);

	//�`��̊J�n���Ԃ�ݒ肷��
	m_LagGo->SetInTime("", 0);

	//���ɕ\�����镶����ݒ肷��
	m_LagGo->SetNextString("", "GO");

	//�`��̏I�����Ԃ�ݒ肷��
	m_LagGo->SetOutTime(80);

	//LagFinish�𐶐�����
	m_LagFinish = std::make_unique<TimeLag>();
	//LagFinish�̏��������s��
	m_LagFinish->Initialize(false, "",Vector3(-500.0f, 300.0f, 0.0f), Vector3::Zero);

	//�`��̊J�n���Ԃ�ݒ肷��
	m_LagFinish->SetInTime("", 0);
	m_LagFinish->SetInTime("1", 0);

	//���ɕ\�����镶����ݒ肷��
	m_LagFinish->SetNextString("", "STAGE" + std::to_string(staticData->Get_IntData("�X�e�[�W")) + "CLEAR");
	m_LagFinish->SetNextString("1", "TimeUp Miss");

	//�`��̏I�����Ԃ�ݒ肷��
	m_LagFinish->SetOutTime(300);

	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//���x���A�b�v�̃e�N�X�`�����擾����
	ITextureBase* useTexture = resourceManager->GetTexture("���x���A�b�v1");

	//���x���A�b�v�G�t�F�N�g�𐶐�����
	std::unique_ptr<LevelUp> plevelUp = std::make_unique<LevelUp>(useTexture);
	m_LevelUp = plevelUp.get();
	m_LevelUp->SetState(ITextureBase::STATE::NONE);
	m_LevelUp->Initialize();

	//GameContext�ɓo�^����
	GameContext::Register<LevelUp>(m_LevelUp);

	//SubInfoManager�ɒǉ�����
	m_SubInfoManager->Add(std::move(plevelUp));
}

void PlayScene::CreateScore()
{
	//�X�R�A�𐶐�����
	m_score = std::make_unique<Score>();

	//���~�ƃX�R�A��ݒ肷��
	m_score->SetScore(0.0f);
	m_score->SetSaving(0.0f);
	m_score->SetAdd(1);
}

void PlayScene::UpdateLag()
{
	//SceneManager����SceneState���擾����
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�O��̃t���[������̌o�ߎ���
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//LagEffect�̍X�V

	if (!m_LagStart->GetCustomFlag() && m_LagStart->GetFrameCount() > 30)
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__27_READY);

		//�J�X�^���t���O��true�ɂ���
		m_LagStart->SetCustomFlag(true);
	}

	if (m_LagStart->GetAllStringFlag())
	{
		if (m_LagStart->GetAutoMoveState()->GetExecute())
		{
			switch (m_LagStart->GetAutoMoveState()->GetNowState())
			{
			case DIRECT::NNONE:
				//���Ɉړ�������
				m_LagStart->GetAutoMoveState()->SetNowState(DIRECT::LEFT);
				break;
			case DIRECT::LEFT:
				//���W���X�e�[�W�̍��[�𒴂��悤�Ƃ�����A�X�e�[�g��ύX����
				if (m_LagStart->GetPosition().x <= 200.0f)
				{
					//�E�Ɉړ�������
					m_LagStart->GetAutoMoveState()->SetNowState(DIRECT::RIGHT);
					//�W�����v������
					m_LagStart->GetJumpState()->SetNowState(JUMP::START);
				}
				break;
			}
		}

		//2��W�����v���āA���n������
		if (m_LagStart->GetJumpState()->GetJumpmDesignation())
		{
			if (m_LagStart->GetAutoMoveState()->GetNowState() == DIRECT::RIGHT)
			{
				//���s�֐��𗎂Ƃ�
				m_LagStart->GetAutoMoveState()->SetExecute(false);
				//�ړ����~������
				m_LagStart->GetAutoMoveState()->SetNowState(DIRECT::NNONE);
				//���̏������������s��
				m_StarManager->Initialize(m_LagStart->GetPosition());
			}
		}
	}

	//LagGo�̍X�V

	//�Ԃ����̃W�����v���I����āA�`�悪�؂�ꂽ�Ƃ�
	if (m_StarManager->GetPopStarRed()->GetJumpState()->GetJumpmDesignation() &&
		m_StarManager->GetPopStarRed()->GetState() == ITextureBase::STATE::NONE)
	{
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__28_GO);

		//�X�V�t���O��true�ɂ���
		m_LagGo->SetFrameFlag(true);

		//�W�����v�I���t���O�����ɖ߂�
		m_StarManager->GetPopStarRed()->GetJumpState()->SetJumpmDesignation(false);
	}

	//LagGo�̉��o���I��������A�^�C�}�[���X�V����
	if (m_LagGo->GetFinishFlag())
	{
		if (!m_LagGo->GetCustomFlag())
		{
			//�^�C�}�[�𓮂���
			m_TimerJudge->SetTimerUp(true);

			//�J�X�^���t���O��true�ɂ���
			m_LagGo->SetCustomFlag(true);
		}
	}

	Vector3 vel = Vector3::Zero;

	//LagFinish�̍X�V
	switch (this->GetNowState())
	{
	case BACKOUT::ADMISSON:
		//�o�ߎ��Ԃ��X�V����
		m_easingTime += elapsedTime;

		//UI�̃A�j���[�V������ݒ肷��(Easing�֐����g�p����)
		vel.x += -Easing::OutBack(m_easingTime, CoroKun::EASING_TOTALTIME,6.0f, CoroKun::EASING_MAX, CoroKun::EASING_MIN);

		//���Ԃ��o�߂�����X�e�[�g��؂�ւ���
		if (m_easingTime > CoroKun::EASING_MAX)
		{
			this->SetNowState(BACKOUT::EXIT);
			m_easingTime = CoroKun::EASING_MIN;
		}
		break;
	case BACKOUT::EXIT:
		//�o�ߎ��Ԃ��X�V����
		m_easingTime += elapsedTime;

		//UI�̃A�j���[�V������ݒ肷��(Easing�֐����g�p����)
		vel.x += Easing::InBack(m_easingTime, CoroKun::EASING_TOTALTIME,0.0f, CoroKun::EASING_MAX, CoroKun::EASING_MIN) * 100.0f;

		//���Ԃ��o�߂�����X�e�[�g��؂�ւ���
		if (m_easingTime > CoroKun::EASING_MAX)
		{
			//�������Ԃ��c���Ă���Ƃ�
			if (!m_TimerJudge->GetTimerJudge())
			{
				this->SetNowState(BACKOUT::RESULT);
			}
			else {
				//�������Ԃ��c���Ă��Ȃ��Ƃ�
				this->SetNowState(BACKOUT::TITLE);
			}
			m_easingTime = CoroKun::EASING_MIN;
		}
		break;
	case BACKOUT::TITLE:
	case BACKOUT::RESULT:
		//���U���g�ɑJ�ڂ���
		state->SetFadeState(SceneState::FADEOUT);

		//�X�e�[�g��؂�ւ���
		this->SetNowState(BACKOUT::NONE);

		//�t�F�[�h�̉��Z���x��ݒ肷��
		state->SetFadeSpeed(SceneState::ALFA_SPEED);

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__23_�v���C�t�F�[�h�A�E�g);
		break;
	}

	m_LagFinish->SetVelocity(vel);
}

