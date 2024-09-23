#include "CoroKun.h"
#include "pch.h"
#include "CoroKun.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "GameContext.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/StackScene/Scene/Select/SelectScene.h"
#include "OtobeLib/StackScene/Scene/DirecTing/DirecTingScene.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "GameObject/Stage/Maptip/MapTip.h"
#include "GameObject/Enemy/Hyper/HyperEnemy.h"
#include "GameObject/Enemy/Super/SuperEnemy.h"
#include "GameObject/Enemy/Normal/NormalEnemy.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/Stage/Maptip/Wall/Wall.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameObject/MoveState/AccelState/AccelState.h"
#include "GameObject/TimeLag/TimeLag.h"
#include "GameObject/Gage/NormalGage/NormalGage.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "ExternalLib/Easing/Easing.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;
using namespace ExternalLib;

//�s����͈͂̍ő�l
const Vector3 CoroKun::MAX_LIMIT = Vector3(4240.0f, 661.0f,0.0f);

//�s����͈͂̍ŏ��l
const Vector3 CoroKun::MIN_LIMIT = Vector3(200.0f, 141.0f,0.0f);

//1�t���[���ɉ��Z����X�s�[�h
const float CoroKun::SPEED = 5.0f;

//Easing�֐��Ɏg�p����g�[�^���^�C��
const float CoroKun::EASING_TOTALTIME = 1.0f;

//Easing�֐��Ɏg�p����ő�l
const float CoroKun::EASING_MAX = 1.0f;

//Easing�֐��Ɏg�p����ŏ��l
const float CoroKun::EASING_MIN = 0.0f;

//���o�[�X�A�r���e�B�̎�������
const float CoroKun::DEATHBLOW_ABILITY_SUSTAIN_TIME = 10.0f;

//�w�ʂ̓_�ł̊J�n����
const float CoroKun::BACKTEXTURE_BLINK_START = 7.0f;

CoroKun::CoroKun(ITextureBase* texture):
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_HPGage(nullptr),//HP�Q�[�W
	m_mpGage(nullptr),//MP�Q�[�W
	m_specialGage(nullptr),//�X�y�V�����Q�[�W
	m_backTexture(nullptr),//�w�ʂ̉摜
	m_WalkState(nullptr),//WalkState
	m_DamageState(nullptr),//DamageState
	m_BodyBlow(nullptr),//BodyBlow
	m_VirusBeam(nullptr),//VirusBeam
	m_NeedleOverAll(nullptr),//Needle�̔z����Ǘ����Ă���N���X
	m_Resurrection(nullptr),//Needle�̔z����Ǘ����Ă���N���X
	m_Normal(nullptr),//�ʏ�Z�̃N���X
	m_easingTime(0.0f),//Easing�֐���Time
	m_timer(0.0f),//�o�ߎ���
	m_timerUp(false),//���Ԃ̍X�V
	m_warpPos(Vector3::Zero),//���V��̍��W
	m_frameCount(NULL),//�t���[�����̃J�E���g
	m_damageCheck(false),//�_���[�W�̃`�F�b�N�t���O
	m_fadeBodyBlow(nullptr),//�R�}���h���̑̓�����̃G�t�F�N�g
	m_fadeBeam(nullptr),//�R�}���h���̃r�[���̃G�t�F�N�g
	m_fadeNeedleOverAll(nullptr),//�R�}���h���̐j�̑S�̍U���̃G�t�F�N�g
	m_fadeResurection(nullptr),//�R�}���h���̐j�͈͍̔U���̃G�t�F�N�g
	m_fadeSpecial(nullptr),//�K�E�ZUI�̔w�i�̃e�N�X�`��
	m_sweatTexture(nullptr)//���̃e�N�X�`��
{
	//�`�揇��ݒ肷��
	this->SetDrawOrder(4);

	//�C�[�W���O�^�C���̏�����
	m_easingTime = 0.0f;

	//GameContext�ɓo�^����
	GameContext::Register<CoroKun>(this);

	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* Gage = resourceManager->GetTexture("WhiteBack");
	ITextureBase* back = resourceManager->GetTexture("SickBack");
	ITextureBase* command = resourceManager->GetTexture("�R�}���h");
	ITextureBase* sweat = resourceManager->GetTexture("��");

	//PlayScene���擾����
	PlayScene* playScene = GameContext::Get<PlayScene>();

	//TextureManager���擾����
	TextureManager* gameManager = playScene->GetGameManager().get();
	TextureManager* subInfoManager = playScene->GetSubInfoManager().get();

	//�w�ʂ̃e�N�X�`���𐶐�����
	std::unique_ptr<TextureBase> pbackTexture = std::make_unique<TextureBase>(back);

	//HP�Q�[�W�𐶐�����
	std::unique_ptr<HPGage> pHPGage = std::make_unique<HPGage>(Gage);

	//�Z�Q�[�W�𐶐�����
	std::unique_ptr<TechniqueGage> pTechniqueGage = std::make_unique<TechniqueGage>(Gage);

	//�K�E�Z�Q�[�W�𐶐�����
	std::unique_ptr<DeathblowGage> pDeathblowGage = std::make_unique<DeathblowGage>(Gage);

	//�o���l�Q�[�W�𐶐�����
	std::unique_ptr<ExperienceGage> pExperienceGage = std::make_unique<ExperienceGage>(Gage);

	//���|�C���^��o�^����
	m_backTexture = pbackTexture.get();
	m_HPGage = pHPGage.get();
	m_mpGage = pTechniqueGage.get();
	m_specialGage = pDeathblowGage.get();
	m_expGage = pExperienceGage.get();

	//�w�ʂ̃e�N�X�`���̐F��ύX����
	m_backTexture->SetColor(Colors::SkyBlue);
	//HP�Q�[�W�̐F��ύX����
	m_HPGage->SetColor(Colors::GreenYellow);
	//�Z�Q�[�W�̐F��ύX����
	m_mpGage->SetColor(Colors::Blue);
	//�K�E�Z�Q�[�W�̐F��ύX����
	m_specialGage->SetColor(Colors::Red);
	//�o���l�Q�[�W�̐F��ύX����
	m_expGage->SetColor(Colors::Aqua);

	//�w�ʂ̃e�N�X�`���̕`���؂�
	m_backTexture->SetState(STATE::NONE);
	//�`�揇��ݒ肷��
	m_backTexture->SetDrawOrder(5);
	//���_��ݒ肷��
	m_backTexture->SetOrigin(m_backTexture->AskOrigin());
	//�_�ł̐ݒ������
	m_backTexture->SetBlink(false);
	//�_�ł̊Ԋu��ݒ肷��
	m_backTexture->SetTarget(0.1f);
	//�_�ł̃I�v�V������ݒ肷��
	m_backTexture->SetFinalOption(true);
	//�_�ł̏I�����Ԃ�ݒ肷��
	m_backTexture->SetFinal(3);

	//�؂���ʒu��ύX����
	//HP�Q�[�W
	m_HPGage->SetRect(RECT{ 0,0,0,50 });
	m_HPGage->SetPosition(Vector3(200.0f, 0.0f, 0.0f));
	m_HPGage->SetMaxTexLength(600);

	//�Z�Q�[�W
	m_mpGage->SetRect(RECT{0,0,0,40});
	m_mpGage->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	m_mpGage->SetDrawOrder(2);
	m_mpGage->SetMaxTexLength(500);

	//�K�E�Z�Q�[�W
	m_specialGage->SetRect(RECT{ 0,0,0,70 });
	m_specialGage->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	m_specialGage->SetDrawOrder(4);
	m_specialGage->SetMaxTexLength(600);

	//�o���l�Q�[�W
	m_expGage->SetRect(RECT{ 0,0,0,25 });
	m_expGage->SetPosition(Vector3(200.0f, 50.0f, 0.0f));
	m_expGage->SetMaxTexLength(600);
	m_expGage->SetFrameCalcSpeed((int)SPEED);

	//TextureManager�ɒǉ�����
	gameManager->Add(std::move(pbackTexture));
	subInfoManager->Add(std::move(pHPGage));
	subInfoManager->Add(std::move(pTechniqueGage));
	subInfoManager->Add(std::move(pDeathblowGage));
	subInfoManager->Add(std::move(pExperienceGage));

	//�摜�̒��S�ʒu��ݒ肷��
	this->SetOrigin(this->AskOrigin());

	//�X�V�֐����N����
	this->SetActive(true);

	//�X�e�[�g��ݒ肷��
	m_WalkState = std::make_unique<WalkState>();
	m_DamageState = std::make_unique<DamageState>();
	m_AccelState = std::make_unique<AccelState>();
	GameContext::Register<WalkState>(m_WalkState.get());
	GameContext::Register<DamageState>(m_DamageState.get());
	GameContext::Register<AccelState>(m_AccelState.get());
	m_StateList.push_back(m_WalkState.get());
	m_StateList.push_back(m_DamageState.get());
	m_StateList.push_back(m_AccelState.get());

	//WalkState��on�ɂ���
	m_WalkState->SetExecute(true);

	//DamageState��off�ɂ���
	m_DamageState->SetExecute(false);

	//AccelState��off�ɂ���
	m_AccelState->SetExecute(false);

	//�_�ł̊Ԋu��ݒ肷��
	TextureBase::SetTarget(0.1f);

	//�_�ł̏I���̐ݒ������
	TextureBase::SetFinal(3);
	TextureBase::SetFinalOption(true);

	//�Z�𐶐�����
	this->CreateTechnique();

	//�v���C���[�̃X�e�[�^�X���擾����
	std::vector<std::string> playerStatus = playScene->GetSaveDataArray((int)PlayScene::SAVE_DATA::PLAYER_STATUS);

	//�G�̃��x�������擾����
	int hp = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::HP]);
	int mp = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::MP]);
	int atack = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::ATACK]);
	int defence = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::DEFENCE]);
	int sp = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::SPECIAL]);
	int level = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::LEVEL]);
	int next = std::stoi(playerStatus[(int)PlayScene::PLAYER_STATUS::NEXT]);

	//�X�e�[�^�X��ݒ肷��
	StatusImporter* importer = StatusBase::FindStatus(this->GetName(), level);

	//�v���C���[�̃X�e�[�^�X��ݒ肷��
	this->SetStatus(NULL, NULL, atack, defence, NULL, level, NULL);

	//�X�e�[�^�X�̍ő�l��ݒ肷��
	this->SetMaxStatus(importer->GetHp(), importer->GetMp(), importer->GetAtack(), importer->GetDefence(),
		importer->GetSpecial(), MAX_LEVEL, importer->GetNextLevelPoint());

	//�Q�[�W�̕␳���s��
	this->OnHealHp(hp);
	this->OnHealCommand(mp);
	this->OnHealSpecial(sp);

	//���x����1�̎�
	if (this->GetLevel() == MIN_LEVEL)
	{
		//�Q�[�W���񕜂�����
		this->OnHealExp(next);
	}
	//���x�����Q�ȏ�̎�
	else
	{
		//����ւ��p�̌o���l��������
		int tempNext = NULL;

		for (int i = MIN_LEVEL; i <= level;i++)
		{
			//���̃��x���̃X�e�[�^�X�̏�����擾����
			StatusImporter* ip = StatusBase::FindStatus(this->GetName(), i);

			//�o���l�̏�������Z���Ă���
			tempNext += ip->GetNextLevelPoint();
		}

		//���݂̌o���l�������
		tempNext -= next;
		tempNext = this->GetMaxNextLevelPoint() - tempNext;

		//�Q�[�W���񕜂�����
		this->OnHealExp(tempNext);

		//�o���l��ݒ肷��
		this->SetNextLevelPoint(next);
	}

	//�摜�̑傫���𓖂��蔻��Ɏg�p����
	this->SetRadius(Vector3(this->AskOrigin()));

	//���݂̃X�e�[�g��ݒ肷��
	this->AddData2(PLAYERMOVE::INPUT);

	//�R�}���h���̃G�t�F�N�g�𐶐�����

	//�R�}���h���̑̓�����̃G�t�F�N�g�𐶐�
	std::unique_ptr<ITextureBase> pfadeBody = std::make_unique<TextureBase>(command);
	pfadeBody->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	pfadeBody->SetRect(RECT{ 0,0,200,180 });
	pfadeBody->SetColor(Colors::LightGreen);
	pfadeBody->SetState(STATE::DRAW);
	m_fadeBodyBlow = pfadeBody.get();

	//�R�}���h���̃r�[���̃G�t�F�N�g�𐶐�����
	std::unique_ptr<ITextureBase> pfadeBeam = std::make_unique<TextureBase>(command);
	pfadeBeam->SetPosition(Vector3(0.0f, 180.0f, 0.0f));
	pfadeBeam->SetRect(RECT{ 0,180,200,360 });
	pfadeBeam->SetColor(Colors::LightGreen);
	pfadeBeam->SetState(STATE::DRAW);
	m_fadeBeam = pfadeBeam.get();

	//�R�}���h���̐j�̑S�̍U���̃G�t�F�N�g�𐶐�����
	std::unique_ptr<ITextureBase> pfadeNeedle = std::make_unique<TextureBase>(command);
	pfadeNeedle->SetPosition(Vector3(0.0f, 360.0f, 0.0f));
	pfadeNeedle->SetRect(RECT{ 0,360,200,540 });
	pfadeNeedle->SetColor(Colors::LightGreen);
	pfadeNeedle->SetState(STATE::DRAW);
	m_fadeNeedleOverAll = pfadeNeedle.get();

	//�R�}���h���̐j�͈͍̔U���̃G�t�F�N�g�𐶐�����
	std::unique_ptr<ITextureBase> pfadeResurection = std::make_unique<TextureBase>(command);
	pfadeResurection->SetPosition(Vector3(0.0f, 540.0f, 0.0f));
	pfadeResurection->SetRect(RECT{ 0,540,200,720 });
	pfadeResurection->SetColor(Colors::LightGreen);
	pfadeResurection->SetState(STATE::DRAW);
	m_fadeResurection = pfadeResurection.get();

	//�K�E�Z�̔w�i��UI�𐶐�����
	std::unique_ptr<FadeTexture> pfadeSpecial = std::make_unique<FadeTexture>(Gage);
	pfadeSpecial->SetPosition(Vector3(550.0f, 661.0f, 0.0f));
	pfadeSpecial->SetRect(RECT{ 0,0,250,59 });
	pfadeSpecial->SetColor(Colors::Red);
	pfadeSpecial->SetMaxAlfa(1.0f);
	pfadeSpecial->SetMinAlfa(0.0f);
	pfadeSpecial->SetFadeSpeed(0.02f);
	pfadeSpecial->SetDrawOrder(5);
	m_fadeSpecial = pfadeSpecial.get();

	//���̃e�N�X�`���𐶐�����
	std::unique_ptr<TextureBase> psweat = std::make_unique<TextureBase>(sweat);
	psweat->SetState(STATE::NONE);
	m_sweatTexture = psweat.get();

	//TextureManager�ɒǉ�����
	subInfoManager->Add(std::move(pfadeBody));
	subInfoManager->Add(std::move(pfadeBeam));
	subInfoManager->Add(std::move(pfadeNeedle));
	subInfoManager->Add(std::move(pfadeResurection));
	subInfoManager->Add(std::move(pfadeSpecial));
	gameManager->Add(std::move(psweat));
}

void CoroKun::Initialize()
{
	//PlayScene���擾����
	PlayScene* playScene = GameContext::Get<PlayScene>();

	//TextureManager���擾����
	TextureManager* subInfoManager = playScene->GetSubInfoManager().get();

	//�̓�����̏������W��ݒ�
	m_BodyBlow->SetPosition(this->GetPosition() + Vector3(this->AskOrigin().x * 2, 0.0f, 0.0f));

	//�w�ʂ̃e�N�X�`���̏������W��ݒ肷��
	m_backTexture->SetPosition(this->GetPosition());

	//Stage���擾����
	Stage* stage = GameContext::Get<Stage>();

	//�}�b�v�����擾����
	std::vector<std::vector<ITextureBase*>>& mapAll = stage->GetMap();

	//�}�b�v����ǉ�����
	mapAll[mapAll.size() - 1].push_back(m_BodyBlow);
	mapAll[mapAll.size() - 1].push_back(m_VirusBeam);

	//�j�̐����A�X�e�[�W����ǉ�����
	for (Needle* needle : m_NeedleOverAll->GetNeedleList())
	{
		//�}�b�v����ǉ�����
		mapAll[mapAll.size() - 1].push_back(needle);
	}

	//�j�̐����A�X�e�[�W����ǉ�����
	for (Needle* needle : m_Resurrection->GetNeedleList())
	{
		//�}�b�v����ǉ�����
		mapAll[mapAll.size() - 1].push_back(needle);
	}

	//���V�p�̍��W��ݒ肷��
	m_warpPos = this->GetPosition();

	//HP�Q�[�W�̏�����
	m_HPGage->CreateGage(subInfoManager);

	//�F�𔽉f�����邽�߂ɁA�X�V��������
	this->UpdateCommandEffect();

	//���̍��W��ݒ肷��
	m_sweatTexture->SetPosition(this->GetPosition());
}

void CoroKun::Update()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//UI�̕\�����I���܂ŉ������Ȃ�
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//���s�֐��������Ă���Ƃ��́A�������Ȃ�
	if (!this->GetActive())return;

	//���̃I�u�W�F�N�g���������
	this->GetObjectList().clear();

	//�ړ��̏���
	this->Move();
	
	//�v���C���[�֘A�̏���
	this->PlayerState();

	//HP�Q�[�W�̍X�V����
	this->UpdateHpGage();

	//�摜�̍X�V����
	TextureBase::Update();

	//�ϐ��̏�����
	Vector3 sweatPos = Vector3::Zero;

	//�E�������Ă���Ƃ�
	if (this->DirectX() == 1)
	{
		sweatPos = m_warpPos + Vector3(50.0f, -50.0f, 0.0f);
	}
	//���������Ă���Ƃ�
	else
	{
		sweatPos = this->GetPosition() + Vector3(-45.0f, -25.0f, 0.0f);
	}

	m_sweatTexture->SetPosition(sweatPos);

	//���̕\���`����ύX
	m_sweatTexture->SetSpriteEffect(this->GetSpriteEffect());

	//�v���C���[�Ɠ����`����@
	m_BodyBlow->SetSpriteEffect(this->GetSpriteEffect());

	//AccelState�����s����Ă���Ƃ�
	if (m_AccelState->GetExecute())
	{
		m_BodyBlow->SetState(STATE::DRAW);
	}
	else
	{
		m_BodyBlow->SetState(STATE::NONE);
	}

	//�_�Œ��̎�
	if (this->GetBlink())
	{
		switch (this->GetNowData3())
		{
		case COLOR_CHANGE::NONE:
			//�F�𔒂ɂ���
			this->SetColor(Colors::White);

			//5�t���[�����ƂɐF��؂�ւ���
			if (m_frameCount % 5 == NULL && m_frameCount != NULL)
			{
				//�X�e�[�g��؂�ւ���
				this->AddData3(COLOR_CHANGE::CHANGE);
			}

			break;
		case COLOR_CHANGE::CHANGE:
			//�F��Ԃɂ���
			this->SetColor(Colors::Red);

			//5�t���[�����ƂɐF��؂�ւ���
			if (m_frameCount % 5 == NULL && m_frameCount != NULL)
			{
				//�X�e�[�g��؂�ւ���
				this->AddData3(COLOR_CHANGE::NONE);
			}

			break;
		}

		//�t���[�������X�V����
		m_frameCount++;
	}
	else
	{
		//�_�ł��Ă��Ȃ��Ƃ�
		this->SetColor(Colors::White);

		//�t���[����������������
		m_frameCount = NULL;
	}

	//�R�}���h�G�t�F�N�g�̍X�V����
	this->UpdateCommandEffect();

	//�A�j���[�V�����̍X�V����
	Animation::Update();

	//�A�j���[�V�����̏���ݒ肷��
	this->SetRect(this->GetNowAnimation());

	//�����蔻��̍��W��ݒ肷��
	this->SetCenter(this->GetPosition());

	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseHero(this);

	//HyperEnemy�̃��X�g���擾����
	std::vector<HyperEnemy*> hyeneList = GameContext::Get<Stage>()->GetHyperEnemies();

	for (HyperEnemy* hyene : hyeneList)
	{
		//�G���������������Ă���Ƃ�
		if (hyene->GetHP() > NULL && this->GetHP() > NULL)
		{
			//�����蔻���ݒ肷��
			GameContext::Get<CollisionManager>()->AddUseOther(hyene, this, 100.0f);
			//�A�̃C���X�^���X���擾����
			for (SoapBabble* babble : hyene->GetBabbleList())
			{
				//�����蔻���ǉ�����
				GameContext::Get<CollisionManager>()->AddUseOther(babble, this, 100.0f);
			}
		}
	}

	//SuperEnemy�̃��X�g���擾����
	std::vector<SuperEnemy*> superList = GameContext::Get<Stage>()->GetSuperEnemies();

	for (SuperEnemy* super : superList)
	{
		//�G���������������Ă���Ƃ�
		if (super->GetHP() > NULL && this->GetHP() > NULL)
		{
			//�����蔻���ݒ肷��
			GameContext::Get<CollisionManager>()->AddUseOther(super, this, 100.0f);
			//�A�̃C���X�^���X���擾����
			for (std::unique_ptr<FixedTurret>& turret : super->GetTurretList())
			{
				for (SoapBabble* babble : turret->GetBabbleList())
				{
					//�����蔻���ǉ�����
					GameContext::Get<CollisionManager>()->AddUseOther(babble, this, 300.0f);
				}
			}
		}
	}

	//NormalEnemy�̃��X�g���擾����
	std::vector<NormalEnemy*> normalList = GameContext::Get<Stage>()->GetNormalEnemies();

	for (NormalEnemy* normal : normalList)
	{
		//�G���������������Ă���Ƃ�
		if (normal->GetHP() > NULL && this->GetHP() > NULL)
		{
			//�����蔻���ݒ肷��
			GameContext::Get<CollisionManager>()->AddUseOther(normal, this, 100.0f);
		}
	}

	//�A�C�e���̃��X�g���擾����
	std::vector<Item*> itemList = GameContext::Get<PlayScene>()->GetHealManager()->GetAllItem();

	for (Item* item : itemList)
	{
		//HP��0���傫���Ƃ�
		if (this->GetHP() > NULL)
		{
			//�����蔻���ݒ肷��
			GameContext::Get<CollisionManager>()->AddUseOther(this, item, 100.0f);
		}

	}

	//�f�[�^�����݂��邩�ǂ����A�m�F����
	if (!staticData->Request_Data("�v���C���[�_���[�W"))return;

	//�X�e�[�g�̍X�V����
	//�_���[�W�X�e�[�g�����s���ŁA7��ȏ�_���[�W��^������WalkState�����s����
	if (m_DamageState->GetExecute() && m_DamageState->GetDamageCount() >= DamageState::MAX_DAMAGE)
	{
		if (this->GetNowData2() < PLAYERMOVE::SWEATANIME)
		{
			m_WalkState->SetExecute(true);
		}
		m_DamageState->InitDamageState();
		staticData->Add("�v���C���[�_���[�W", "");
	}
}

void CoroKun::Render()
{
	//�摜�̕`�揈��
	//�폜���N�G�X�g����������A�������Ȃ�
	if (this->GetIsVal())return;

	//�e�N�X�`���n���h���������ĂȂ������牽�����Ȃ�
	if (this->GetHandle() == nullptr)return;

	//�_�ł�off�̂Ƃ��͉������Ȃ�
	if (!this->NowBlinking())return;

	//�`�悪off�̂Ƃ��͉������Ȃ�
	if (this->GetState() != STATE::DRAW)return;

	//�`��n�̏��
	Graphics* Graph = Graphics::GetInstance();
	SpriteBatch* Sprite = Graph->GetSpriteBatch();
	CommonStates* State = Graph->GetCommonStates();

	//�`��
	Sprite->Draw(this->GetHandle().Get(), m_warpPos, &this->GetRect(), this->GetColor(),
		XMConvertToRadians(this->GetAngle()), this->GetOrigin(), this->GetScale(), this->GetSpriteEffect());
}

void CoroKun::Finalize()
{
}

void CoroKun::OnCollisionEnter(ITextureBase* obj)
{
	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	CollisionBase::OnCollisionEnter(obj);
	
	//���ɓ���������
	if (obj->GetName() == "floor")this->OnCollisionFloor(obj);

	//�ǂɓ���������
	if (obj->GetName() == "wall")this->OnCollisionWall(obj);

	//�A�ɓ��������Ƃ�
	if (obj->GetName() == "babul")this->OnCollisionBabul(obj);

	//���]��ɓ��������Ƃ�
	if (obj->GetName() == "rebound")this->OnCollisionRebound(obj);

	//�N���X�C�b�`�ɓ���������
	if (obj->GetName() == "switch")this->OnCollisionSwitch(obj);

	//���Ƃ����ɓ���������
	if (obj->GetName() == "hole")this->OnCollisionFoleFloor(obj);

	//HyperEnemy�ɓ���������
	if (obj->GetName() == "HyperEnemy")this->OnCollisionHyperEnemy(obj);

	//SuperEnemy�ɓ���������
	if (obj->GetName() == "SuperEnemy")this->OnCollision_SuperEnemy(obj);

	//NormalEnemy�ɓ��������Ƃ�
	if (obj->GetName() == "NormalEnemy")this->OnCollision_NormalEnemy(obj);

	//UI�ɓ���������
	if (obj->GetName() == "WhiteBack")this->OnCollisionWhiteBack(obj);

	//HyperEnemy�̖A�ɓ���������
	if (obj->GetName() == "Hypersyabondama")this->OnCollisionHyperBabble(obj);

	//HyperEnemy�̖A�ɓ���������
	if (obj->GetName() == "Supersyabondama")this->OnCollision_SuperBabble(obj);

	//�S�[���ɓ���������
	if (obj->GetName() == "�S�[��")this->OnCollision_Goal(obj);

}

void CoroKun::NonCollisionEnter(ITextureBase* obj)
{
	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	CollisionBase::NonCollisionEnter(obj);
}

//�v���C���[�֘A�̏���
void CoroKun::PlayerState()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�^�C�}�[���擾����
	TimerJudge* timer = GameContext::Get<TimerJudge>();

	//Switch���ŏ����𕪂���
	switch (this->GetNowData2())
	{
	case PLAYERMOVE::INPUT:

		//HP��0�ɂȂ�����A�X�e�[�g��؂�ւ���
		if (this->GetHP() <= NULL)
		{
			if (!m_DamageState->GetExecute())
			{
				this->AddData2(PLAYERMOVE::SWEATANIME);
				this->SetVelocity(Vector3::Zero);
			}
			m_WalkState->SetExecute(false);
			m_Normal->GetNormalGage()->SetState(STATE::NONE);
			m_Normal->GetBackGage()->SetState(STATE::NONE);
			return;
		}

		//�������Ԃ��s������A�X�e�[�g��؂�ւ���
		if (timer->GetTimerJudge())
		{
			TimeLag* finish = GameContext::Get<PlayScene>()->GetLagFinish().get();
			finish->SetTarget("1");
			this->Finish(PLAYERMOVE::DEAD,CRI_CUESHEET_0__30_TIMEUP);
			this->SetVelocity(Vector3::Zero);
			return;
		}

		//�X�e�[�g�̍X�V����
		this->UpdateState();

		//�w�ʂ̃t�F�[�h�̍X�V����
		this->UpdateBackFade();

		//�ʏ�e�̍X�V�֐������s
		m_Normal->Update();

		//�j�̍X�V�֐������s
		m_NeedleOverAll->Update();
		m_Resurrection->Update();

		//�Z�̍X�V����
		this->UpdateTechnique();

		//�A�j���[�V���������s���̎�
		if (this->GetAniFlag())
		{
			//�j����o����^�C�~���O�ŁA���o����
			if (this->GetNowInfo() == "�ʏ��1" && m_NeedleOverAll->GetNowState() == NEEDLEACTION::MOPE)
			{
				m_NeedleOverAll->SetNowState(NEEDLEACTION::RELEASE);
				m_WalkState->SetExecute(true);

				//SE��炷
				soundManager->Play(CRI_CUESHEET_0__18_�j���o);
			}

			//�I�����Ɍ��ɖ߂�
			if (this->GetNowInfo() == "�ʏ��2")
			{
				m_WalkState->SetExecute(true);
			}
		}

		break;
	case PLAYERMOVE::SWEATANIME:
		this->SweatAnime();
		break;
	case PLAYERMOVE::ROTSCALE:
		this->RotScale();
		break; 
	case PLAYERMOVE::ANIMESTART:
		this->DeadAnimeStart();
		break;
	case PLAYERMOVE::ANIMEFINISH:
		this->DeadAnimeResult();
		break;
	}
}

void CoroKun::Move()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�x�N�g���̏�����
	Vector3 vel = Vector3::Zero;

	//���s���̃X�e�[�g����x�N�g�����擾����
	for (MoveState* state : m_StateList)
	{
		vel += state->Update();
	}

	//�x�N�g����ݒ肷��
	this->SetVelocity(vel);

	//�C�[�W���O�֐��̍X�V����
	this->UpdateEasing();

	//�p�x���X�V����
	this->InputAngle();
}

//HP��0�ɂȂ������_�ł̃A�j���[�V����
void CoroKun::SweatAnime()
{
	//�A�j���[�V��������ύX
	this->SetNowInfo("���S1");

	//�p�x�𒲐�
	if (this->DirectX() == 1)this->SetAngle(15.0f);
	else this->SetAngle(330.0f);

	//���̃e�N�X�`����\��
	m_sweatTexture->SetState(STATE::DRAW);

	//�_�ł��Ă��Ȃ��Ƃ�
	if (!this->GetBlink())
	{
		//�X�e�[�g��؂�ւ���
		this->AddData2(PLAYERMOVE::ROTSCALE);

		//���̃e�N�X�`�����\��
		m_sweatTexture->SetState(STATE::NONE);
	}
}

//��]�Ək���̏���
void CoroKun::RotScale()
{
	//1�t���[�������Z����
	float angle = this->GetAngle();
	angle += 1.0f;
	this->SetAngle(angle);

	//�������A�k������
	Vector3 scale = this->GetScale();
	scale -= Vector3(0.01f);
	this->SetScale(scale);

	//�傫����10����1�ɂȂ�����
	if (scale.x <= 0.1f)
	{
		//�{�������ɖ߂�
		this->SetScale(Vector3(1.0f));

		//�p�x�����ɖ߂�
		this->SetAngle(0.0f);

		//�A�j���[�V��������ύX
		this->SetNowInfo("���S2");
		this->SetLoopFlag("���S10");

		//�A�j���[�V�������N��
		this->SetAniFlag(true);
		this->SetLoopFlag(false);

		//�_�ŊJ�n
		this->SetBlink(true);

		//�I�v�V������؂�
		this->SetFinalOption(false);

		//�X�e�[�g��؂�ւ���
		this->AddData2(PLAYERMOVE::ANIMESTART);
	}
}

void CoroKun::DeadAnimeStart()
{
	//�A�j���[�V�������I��
	if (this->GetNowInfo() == "���S10")
	{
		//�X�e�[�g��؂�ւ���
		this->AddData2(PLAYERMOVE::ANIMEFINISH);

		//�`���؂�
		this->SetState(STATE::NONE);
	}
}

void CoroKun::DeadAnimeResult()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//���s�֐��𗎂Ƃ�
	this->SetActive(false);

	//SceneManager���擾����
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();
	
	//���񂾂�A�J�ڂ���
	state->SetFadeState(SceneState::FADEOUT);

	//�t�F�[�h�̉��Z���x��ݒ肷��
	state->SetFadeSpeed(SceneState::ALFA_SPEED);

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SE���Đ�����
	soundManager->Play(CRI_CUESHEET_0__23_�v���C�t�F�[�h�A�E�g);
}

void CoroKun::UpdateState()
{
	//SceneManager���擾����
	SceneManager* Manager = SceneManager::GetInstance();

	//�L�[�̏�Ԃ��擾����
	Input* input = Input::GetInstance();

	//�L�[�{�[�h�̏�Ԃ��擾����
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//�J�b�g�C���̉��o��A�Z�����s����
	if (staticData->Get_StringData("�J�b�g�C��") == "�̓�����")
	{
		//���s�֐���true�ɂ���
		m_BodyBlow->SetActive(true);
		//WalkState���~�߂�
		m_WalkState->SetExecute(false);
		//AccelState������������
		m_AccelState->InitState();
		//���s�֐���true�ɂ���
		m_AccelState->SetExecute(true);
		//�����Ă���Ƃ�(�����ɂ���āA�x�N�g����؂�ւ���)
		vel = Vector3(SPEED * this->DirectX(), 0.0f, 0.0f);
		//�x�N�g����ݒ肷��
		m_AccelState->SetAccel(vel);
		//�J�b�g�C���̏����f�t�H���g�ɖ߂�
		staticData->Add("�J�b�g�C��", (std::string)"�f�t�H���g");
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__26_�̓�����);
	}

	//AccelState�����s����
	if (keyState.Z && keyState.A)
	{
		//AccelState�����s����Ă��Ȃ��Ƃ�
		if (!m_AccelState->GetExecute() && !this->ALLTechniqueExe() && !this->GetBlink())
		{
			//MP��50�ȏ�̎�
			if (this->GetMP() >= BodyBlow::GAGE_COST)
			{
				//�Q�[�W�������
				this->OnCommand(BodyBlow::GAGE_COST);
				//CutInScene��ǉ���������
				Manager->PushScene("CutIn");
				//�J�b�g�C���̏���ǉ�����
				staticData->Add("�J�b�g�C��", (std::string)"�̓�����");
			}
			//�Z���g���Ȃ��Ƃ�
			else
			{
				if (keyTrakker.IsKeyPressed(Keyboard::Z) || keyTrakker.IsKeyPressed(Keyboard::A))
				{
					//SE���Đ�����
					soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
				}
			}
		}
	}
}

void CoroKun::UpdateTechnique()
{
	//SceneManager���擾����
	SceneManager* Manager = SceneManager::GetInstance();

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�L�[�̏�Ԃ��擾����
	Input* input = Input::GetInstance();

	//�L�[�{�[�h�̏�Ԃ��擾����
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�J�b�g�C���̉��o��A�Z�����s����
	if (staticData->Get_StringData("�J�b�g�C��") == "�r�[��")
	{
		m_VirusBeam->SetPosition(this->GetPosition() + Vector3(this->AskOrigin().x * this->DirectX(), -this->AskOrigin().y / 2, 0.0f));
		//�r�[���̍��W��ݒ肷��
		m_VirusBeam->SetParticlesPos(this->GetPosition() + Vector3(this->AskOrigin().x * 2, 0.0f, 0.0f) * this->DirectX());
		//SpriteEffect��ݒ肷��
		m_VirusBeam->SetSpriteEffect(this->GetSpriteEffect());
		//�ړ����~�߂�
		m_WalkState->SetExecute(false);
		//���s�֐���true�ɂ���
		m_VirusBeam->SetActive(true);
		//�S�Ẵp�[�e�B�N���̕`����I���ɂ���
		for (Particle*& particle : m_VirusBeam->GetAllParticleList())
		{
			//�p�[�e�B�N�����`�悳��Ă��Ȃ��Ƃ�
			if (particle->GetState() == STATE::NONE)
			{
				//�`����I���ɂ���
				particle->SetState(STATE::DRAW);
			}
		}
		//�X�e�[�g��؂�ւ���
		m_VirusBeam->SetNowState(ENERGY_CHAGE::TRACKING);
		//�J�b�g�C���̏����f�t�H���g�ɖ߂�
		staticData->Add("�J�b�g�C��", (std::string)"�f�t�H���g");
		//SE��炷
		soundManager->Play(CRI_CUESHEET_0__14_�`���[�W);
	}

	//�J�b�g�C���̉��o��A�Z�����s����
	if (staticData->Get_StringData("�J�b�g�C��") == "�I�[�o�[�I�[��")
	{
		//�A�j���[�V�����̃t���O�𗧂Ă�
		this->SetAniFlag(true);
		//�A�j���[�V�����̌��݂̏���ݒ肷��
		this->SetNowInfo("�ʏ�1");
		//�A�j���[�V�����̃��[�v����ݒ肷��
		this->SetLoopInfo("�ʏ��1");
		//�X�e�[�g��ύX����
		m_NeedleOverAll->SetNowState(NEEDLEACTION::MOPE);
		m_WalkState->SetExecute(false);
		//�X�V�֐���true�ɂ���
		m_NeedleOverAll->SetActive(true);
		//�J�b�g�C���̏����f�t�H���g�ɖ߂�
		staticData->Add("�J�b�g�C��", (std::string)"�f�t�H���g");
		//SE��炷
		soundManager->Play(CRI_CUESHEET_0__17_�j�p��);
	}

	//�J�b�g�C���̉��o��A�Z�����s����
	if (staticData->Get_StringData("�J�b�g�C��") == "���U���N�V����")
	{
		//�A�j���[�V�����̃t���O�𗧂Ă�
		this->SetAniFlag(true);
		//�A�j���[�V�����̌��݂̏���ݒ肷��
		this->SetNowInfo("�ʏ�2");
		//�A�j���[�V�����̃��[�v����ݒ肷��
		this->SetLoopInfo("�ʏ��2");
		//�X�e�[�g��ύX����
		m_Resurrection->SetNowState(RENEEDLE::LITTLE);
		m_WalkState->SetExecute(false);
		//�X�V�֐���true�ɂ���
		m_Resurrection->SetActive(true);
		//�J�b�g�C���̏����f�t�H���g�ɖ߂�
		staticData->Add("�J�b�g�C��", (std::string)"�f�t�H���g");
		//SE��炷
		soundManager->Play(CRI_CUESHEET_0__17_�j�p��);
	}

	//�L�[�������āA�r�[�����o��
	if (keyState.Z && keyState.C)
	{
		//�r�[���̃`���[�W�X�e�[�g��NONE�̂Ƃ��ɂ����A������ʂ�
		if (m_VirusBeam->GetNowState() == ENERGY_CHAGE::ENERGYNONE && !this->ALLTechniqueExe() && !this->GetBlink())
		{
			//�v���C���[���E���A���������Ă���Ƃ�
			if (this->DirectX() != 0)
			{
				//MP��150�ȏ�̎�
				if (this->GetMP() >= VirusBeam::GAGE_COST)
				{
					//�Q�[�W�������
					this->OnCommand(VirusBeam::GAGE_COST);
					//CutInScene��ǉ���������
					Manager->PushScene("CutIn");
					//�J�b�g�C���̏���ǉ�����
					staticData->Add("�J�b�g�C��", (std::string)"�r�[��");
				}
				//�Z���g���Ȃ��Ƃ�
				else
				{
					if (keyTrakker.IsKeyPressed(Keyboard::Z) || keyTrakker.IsKeyPressed(Keyboard::C))
					{
						//SE���Đ�����
						soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
					}
				}
			}
		}
	}

	//�L�[�������āANeedle��S���ʂɏo��
	if (keyState.Z && keyState.D)
	{
		//�_���[�W���󂯂Ă��Ȃ��@�Z���g���Ă��Ȃ��@�A�j���[�V�����̃t���O�𗎂��Ă���Ƃ�
		if (!this->GetAniFlag() && !this->ALLTechniqueExe() && !this->GetBlink())
		{
			//MP��100�ȏ�̎�
			if (this->GetMP() >= NeedleOverall::GAGE_COST)
			{
				//�Q�[�W�������
				this->OnCommand(NeedleOverall::GAGE_COST);
				//CutInScene��ǉ���������
				Manager->PushScene("CutIn");
				//�J�b�g�C���̏���ǉ�����
				staticData->Add("�J�b�g�C��", (std::string)"�I�[�o�[�I�[��");
			}
			//�Z���g���Ȃ��Ƃ�
			else
			{
				if (keyTrakker.IsKeyPressed(Keyboard::Z) || keyTrakker.IsKeyPressed(Keyboard::D))
				{
					//SE���Đ�����
					soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
				}
			}
		}
	}

	//�L�[�������āANeedle�����͂ɏo��
	if (keyState.Z && keyState.S)
	{
		//�_���[�W���󂯂Ă��Ȃ��@�Z���g���Ă��Ȃ��@�A�j���[�V�����̃t���O�𗎂��Ă���Ƃ�
		if (!this->GetAniFlag() && !this->ALLTechniqueExe() && !this->GetBlink())
		{
			//MP��90�ȏ�̎�
			if (this->GetMP() >= Resurrection::GAGE_COST)
			{
				//�Q�[�W�������
				this->OnCommand(Resurrection::GAGE_COST);
				//CutInScene��ǉ���������
				Manager->PushScene("CutIn");
				//�J�b�g�C���̏���ǉ�����
				staticData->Add("�J�b�g�C��", (std::string)"���U���N�V����");
			}
			//�Z���g���Ȃ��Ƃ�
			else
			{
				if (keyTrakker.IsKeyPressed(Keyboard::Z) || keyTrakker.IsKeyPressed(Keyboard::S))
				{
					//SE���Đ�����
					soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
				}
			}
		}
	}

	//�ʏ�U��
	//�G�l���M�[���`���[�W����
	//�L�[��������Ă���Ƃ�
	//���E�ǂ��炩�������Ă���Ƃ��̂ݎ��s
	if (this->DirectX() != 0)
	{
		if (keyState.Q && !this->ALLTechniqueExe() 
			&& !this->GetBlink())
		{
			if (keyTrakker.IsKeyPressed(Keyboard::Q))
			{
				if (m_Normal->GetCheckPush())
				{
					m_Normal->SetCheckPush(false);
				}

				m_Normal->SetCheckRelease(false);

				//�擪�̒e��ݒ肷��
				m_Normal->SetFrontBullet(m_Normal->GetAllBulletFront());

				if (m_Normal->GetFrontBullet() != nullptr)
				{
					//�擪�̒e��`�悷��
					m_Normal->GetFrontBullet()->SetState(STATE::DRAW);

					//�擪�̒e�̍��W��ݒ肷��
					m_Normal->GetFrontBullet()->SetPosition(this->GetPosition() + Vector3(100.0f, 0.0f, 0.0f) * this->DirectX());

					//�擪�̒e�̊g��{����ݒ肷��
					m_Normal->GetFrontBullet()->SetScale(Vector3(0.2f));

					//SE��炷
					soundManager->Play(CRI_CUESHEET_0__14_�`���[�W);
				}
			}

			if (m_Normal->GetFrontBullet() != nullptr && !m_Normal->GetCheckPush())
			{
				//NormalTechnique����NomalGage���擾����
				NormalGage* nGage = m_Normal->GetNormalGage();

				//�G�l���M�[�`���[�W���s��
				nGage->ChangeGage();

				//�擪�̒e�̍��W��ݒ肷��
				m_Normal->GetFrontBullet()->SetPosition(this->GetPosition() + Vector3(100.0f, 0.0f, 0.0f) * this->DirectX());
			}

		}

		//�L�[�������ꂽ�Ƃ�
		if (keyTrakker.IsKeyReleased(Keyboard::Q))
		{
			if (m_Normal->GetFrontBullet() != nullptr &&
				!this->GetBlink())
			{
				//NormalTechnique����NomalGage���擾����
				NormalGage* nGage = m_Normal->GetNormalGage();

				//�Q�[�W�̏��������s��
				nGage->Initialize();

				//�擪�̒e�̃x�N�g����ݒ肷��
				m_Normal->GetFrontBullet()->SetVelocity(Vector3(7.5f, 0.0f, 0.0f) * this->DirectX());

				//�擪�̒e�̃X�e�[�g��ύX����
				m_Normal->GetFrontBullet()->SetNowState(ENERGY_CHAGE::ENERGYATACK);

				//PlayerTarget��ݒ肷��
				m_Normal->GetFrontBullet()->SetPlayerTarget(this->GetPosition() + Vector3(700.0f, 0.0f, 0.0f) * this->DirectX());

				if (m_Normal->GetCheckRelease())
				{
					//SE���~�߂�
					soundManager->Stop(CRI_CUESHEET_0__14_�`���[�W);
				}
				else 
				{
					//SE���~�߂�
					soundManager->Stop(CRI_CUESHEET_0__14_�`���[�W);

					//SE��炷
					soundManager->Play(CRI_CUESHEET_0__16_�ʏ�e);
				}
			}
		}
	}

	//�K�E�Z�̉��o���I�������A�_���[�W��^����(�_���[�W����������̂́A�_�[�N�G�N�X�v���[�W�����̂Ƃ��̂�)
	if (staticData->Get_StringData("�K�E�Z") == SelectScene::DEATHBLOW_NAME_EXPROSION)
	{
		//HyperEnemy�Ƀ_���[�W��^����
		this->OnExeDeathBlowHyperEnemy();

		//SuperEnemey�Ƀ_���[�W��^����
		this->OnExeDeathBlowSuperEnemy();

		//NormalEnemy�Ƀ_���[�W��^����
		this->OnExeDeathBlowNormalEnemy();

		//�K�E�Z�����f�t�H���g�ɖ߂�
		staticData->Add("�K�E�Z", (std::string)"�f�t�H���g");
	}

	//�K�E�Z�̉��o���I�������A10�b�Ԃ����A�I�[�����o��
	if (staticData->Get_StringData("�K�E�Z") == SelectScene::DEATHBLOW_NAME_ABILITY)
	{
		//�^�C�}�[�̃t���O���N������
		m_timerUp = true;

		//�w�ʂ̃e�N�X�`����`�悷��
		m_backTexture->SetState(STATE::DRAW);

		//�X�e�[�^�X��ݒ肷��(�U���͂Ɩh��͂����ւ���)
		int atack = this->GetAtack();
		int defence = this->GetDefence();
		std::swap(atack, defence);
		this->SetAtack(atack);
		this->SetDefence(defence);

		//�X�e�[�^�X�̍ő�l��ݒ肷��(�U���͂Ɩh��͂����ւ���)
		int maxAtack = this->GetMaxAtack();
		int maxDefence = this->GetMaxDefence();
		std::swap(maxAtack, maxDefence);
		this->SetMaxAtack(maxAtack);
		this->SetMaxDefence(maxDefence);

		//�K�E�Z�����f�t�H���g�ɖ߂�
		staticData->Add("�K�E�Z", (std::string)"�f�t�H���g");
	}

	//�K�E�Z(�ʂ�Scene�ŉ��o���s��)
	//���E�ǂ��炩�������Ă���Ƃ��̂ݎ��s
	if (this->DirectX() != 0 && !this->GetBlink())
	{
		//�L�[��������Ă���Ƃ�
		if (keyState.Z && keyState.V)
		{
			if (this->GetSpecial() >= SelectScene::GAGE_COST_REVASEAVIRITY)
			{
				//SE���Đ�����
				soundManager->Play(CRI_CUESHEET_0__39_���j���[���J��);

				//SelectScene��ǉ���������
				Manager->PushScene("Select");
			}
			//�Z���g���Ȃ��Ƃ�
			else
			{
				//SE���Đ�����
				soundManager->Play(CRI_CUESHEET_0__06_�L�����Z����);
			}
		}
	}

	//�̓�����̍��W���X�V����
	m_BodyBlow->SetPosition(this->GetPosition() + Vector3(this->AskOrigin().x * 2, 0.0f, 0.0f) * this->DirectX());
	m_VirusBeam->SetPosition(this->GetPosition() + Vector3(this->AskOrigin().x * this->DirectX(), -this->AskOrigin().y / 2, 0.0f));
}

void CoroKun::UpdateEasing()
{
	//Easing�֐��̍X�V����

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�O��̃t���[������̌o�ߎ��Ԃ��擾����
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//���݂̃x�N�g�����擾����
	Vector3 vel = this->GetVelocity();

	//�v���C���[��HP��0���傫����
	if (this->GetHP() > NULL)
	{
		//�T�C���J�[�u�Ōv�Z����
		vel.y += sinf(m_easingTime) * 0.1f;

		//�o�ߎ��Ԃ̍X�V
		m_easingTime += elapsedTime * 4.0f;
	}

	//���V��̍��W���X�V����
	m_warpPos += vel;
}

void CoroKun::UpdateBackFade()
{
	//�t�F�[�h�̍X�V����

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//elapsedTime�̎擾
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//�x�N�g����ݒ肷��
	m_backTexture->SetVelocity(this->GetVelocity());

	//�^�C�}�[�̃t���O���N�����Ă���Ƃ�
	if (m_timerUp)
	{
		//�w�i�̐F�����擾����
		Color color = m_backTexture->GetColor();
		float alfa = color.A();

		//�X�e�[�g�ɂ���āA������؂�ւ���
		switch (this->GetNowData1())
		{
		case FADE_STATE::DOWN:
			if (color.A() <= 0.5f)this->AddData1(FADE_STATE::FUP);
			else alfa -= 0.01f;
			break;
		case FADE_STATE::FUP:
			if (color.A() >= 1.0f)this->AddData1(FADE_STATE::DOWN);
			else alfa += 0.01f;
			break;
		}

		//�F�����Đݒ肷��
		color.A(alfa);
		m_backTexture->SetColor(color);

		//10�b�o��
		if (m_timer > DEATHBLOW_ABILITY_SUSTAIN_TIME)
		{
			//�_�ł��Ă��Ȃ�
			if (!m_backTexture->GetBlink())
			{
				//�`���؂�
				m_backTexture->SetState(STATE::NONE);

				//�o�ߎ��Ԃ�����������
				m_timer = 0.0f;

				//�^�C�}�[�̃t���O��؂�
				m_timerUp = false;

				//�X�e�[�^�X��ݒ肷��(�U���͂Ɩh��͂����ւ���)
				int atack = this->GetAtack();
				int defence = this->GetDefence();
				std::swap(atack, defence);
				this->SetAtack(atack);
				this->SetDefence(defence);

				//�X�e�[�^�X�̍ő�l��ݒ肷��(�U���͂Ɩh��͂����ւ���)
				int maxAtack = this->GetMaxAtack();
				int maxDefence = this->GetMaxDefence();
				std::swap(maxAtack, maxDefence);
				this->SetMaxAtack(maxAtack);
				this->SetMaxDefence(maxDefence);

				//�A���t�@�l�����ɖ߂�
				m_backTexture->SetColor(Colors::SkyBlue);

				//�X�e�[�g��؂�ւ���
				this->AddData1(FADE_STATE::NONE);

				//�o�ߎ��Ԃ̍X�V�͍s��Ȃ�
				return;
			}
		}
		//7�b�o��
		else if (m_timer > BACKTEXTURE_BLINK_START)
		{
			//�_�ł��Ă��Ȃ�
			if (!m_backTexture->GetBlink())
			{
				//�_�ł�����
				m_backTexture->SetBlink(true);
			}

			//�_�ł���Ƃ�
			if (m_backTexture->GetTargetCount() <= 0.0f)
			{
				soundManager->Play(CRI_CUESHEET_0__40_�_��);
			}
		}

		//�o�ߎ��Ԃ��X�V����
		m_timer += elapsedTime;
	}
}

void CoroKun::UpdateHpGage()
{
	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�O��̃t���[������̌o�ߎ��Ԃ��擾����
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//�_���[�W���󂯂Ă���Ƃ�
	if (m_damageCheck)
	{
		//�w�ʂ̃e�N�X�`�����擾����
		ITextureBase* backTexture = m_HPGage->GetBackGageTexture();

		//HP�Q�[�W�̗��ʂ̐؂�o���ʒu���擾����
		RECT back = m_HPGage->GetBackGageTexture()->GetRect();

		//HP�Q�[�W�̐؂�o���ʒu���擾����
		RECT front = m_HPGage->GetRect();

		//�_���[�W�̍��������߂�
		int diffrent = (int)(back.right - front.right);

		//���݂̎��Ԃ��擾����
		float time = m_HPGage->GetEasingTime();

		//���Ԃ��X�V����
		time += elapsedTime;

		//�_���[�W���X�J���␳����
		diffrent *= time;

		//�Q�[�W�ɓ���������
		back.right -= diffrent;

		//�؂�o���ʒu���Đݒ肷��
		m_HPGage->GetBackGageTexture()->SetRect(back);

		//���Ԃ��Đݒ肷��
		m_HPGage->SetEasingTime(time);

		if (m_HPGage->GetEasingTime() >= 1.0f)
		{
			//1�b�o�߂�����A�t���O��؂�
			m_damageCheck = false;

			//�؂�o���ʒu��ݒ肷��
			m_HPGage->GetBackGageTexture()->SetRect(m_HPGage->GetRect());
		}
	}
}

void CoroKun::UpdateCommandEffect()
{
	//�̓����肪�g�p�\�Ȏ�
	this->PlayCommandEffect(m_fadeBodyBlow, BodyBlow::GAGE_COST);

	//�r�[�����g�p�\�Ȏ�
	this->PlayCommandEffect(m_fadeBeam, VirusBeam::GAGE_COST);

	//�j�[�h���C���N�W�F�N�V�������g�p�\�Ȏ�
	this->PlayCommandEffect(m_fadeNeedleOverAll, NeedleOverall::GAGE_COST);

	//���U���N�V�������g�p�\�Ȏ�
	this->PlayCommandEffect(m_fadeResurection, Resurrection::GAGE_COST);

	//�K�E�Z���g�p�\�Ȏ�
	this->PlaySpecialEffect(m_fadeSpecial, SelectScene::GAGE_COST_REVASEAVIRITY,Colors::Red);
}

void CoroKun::PlayCommandEffect(ITextureBase* command, const int& cost)
{
	//�R�}���h���g�p�\�Ȏ�
	if (this->GetMP() >= cost)
	{
		//�F��ύX����
		command->SetColor(Colors::LightGreen);
	}
	//�R�}���h���g�p�s�\�Ȏ�
	else
	{
		//�F��ύX����
		command->SetColor(Colors::Gray);
	}
}

void CoroKun::PlaySpecialEffect(FadeTexture* special, const int& cost, const XMVECTORF32 color)
{
	//�K�E�Z���g�p�\�Ȏ�
	if (this->GetSpecial() >= cost)
	{
		//�A�j���[�V�������N�����Ă��Ȃ��Ƃ�
		if (special->GetNowState() == FADE_STATE::NONE)
		{
			//�A�j���[�V�������N������
			special->SetNowState(FADE_STATE::DOWN);
		}
	}
	//�K�E�Z���g�p�s�Ȏ�
	else
	{
		//�F���Œ肷��
		special->SetColor(color);

		//�A�j���[�V�������N�����Ȃ�
		special->SetNowState(FADE_STATE::NONE);
	}
}

void CoroKun::OnCollisionFloor(ITextureBase* floor)
{
	//�摜�������o��
	TextureBase::Extrude(this);

	//���V��̍��W�����킹��
	m_warpPos += -this->GetVelocity();

	//�̓�����̓����蔻��
	m_BodyBlow->WithPlayer(this);

	//�E�C���X�r�[���̓����蔻��
	m_VirusBeam->WithPlayer(this);

	//�w�ʂ̃e�N�X�`���������o��
	m_backTexture->Extrude(this);

	//���̃I�u�W�F�N�g�ɓ���������AWalkState��true�ɂ��āAAccelState�𗎂Ƃ�
	if (!m_VirusBeam->GetActive())
	{
		m_WalkState->SetExecute(true);
		m_AccelState->SetExecute(false);
	}

	//�̓����蒆�̎��́A���s�֐��𗎂Ƃ�
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}
}

void CoroKun::OnCollisionWall(ITextureBase* wall)
{
	bool flag = true;

	//�摜�������o��
	TextureBase::Extrude(this);

	//���V��̍��W�����킹��
	m_warpPos += -this->GetVelocity();

	//�̓�����̓����蔻��
	m_BodyBlow->WithPlayer(this);

	//�E�C���X�r�[���̓����蔻��
	m_VirusBeam->WithPlayer(this);

	//�w�ʂ̃e�N�X�`���������o��
	m_backTexture->Extrude(this);

	//���̃I�u�W�F�N�g�ɓ���������AWalkState��true�ɂ��āAAccelState�𗎂Ƃ�
	if (!m_VirusBeam->GetActive())
	{
		m_WalkState->SetExecute(true);
		m_AccelState->SetExecute(false);
	}

	//�̓����蒆�̎��́A���s�֐��𗎂Ƃ�
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}

	//���������I�u�W�F�N�g��ǉ�����
	CollisionBase::Add(wall);
}


//�A�ɓ��������Ƃ��̏���
void CoroKun::OnCollisionBabul(ITextureBase* babul)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�摜�������o��
	TextureBase::Extrude(this);

	//���V��̍��W�����킹��
	m_warpPos += -this->GetVelocity();

	//�̓�����̓����蔻��
	m_BodyBlow->WithPlayer(this);

	//�E�C���X�r�[���̓����蔻��
	m_VirusBeam->WithPlayer(this);

	//�w�ʂ̃e�N�X�`���������o��
	m_backTexture->Extrude(this);

	//���̃I�u�W�F�N�g�ɓ���������AWalkState��true�ɂ��āAAccelState�𗎂Ƃ�
	m_WalkState->SetExecute(true);
	m_AccelState->SetExecute(false);

	//�̓����蒆�̎��́A���s�֐��𗎂Ƃ�
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}

	//WalkState�̎��s�֐������s����Ă��āA�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�_���[�W���󂯂�
		this->OnDamage(this->GetMaxHP() * 0.1f, NULL);

		//���˕Ԃ��ē��������Ƃ�
		if (m_DamageState->GetExecute())
		{
			//�^����_���[�W��ݒ肷��
			m_DamageState->SetDamage(m_DamageState->GetFirstDamage());
			//�x�N�g����ݒ肷��
			this->SetVelocity(m_DamageState->GetFirstDamage());
			//�X�e�[�g������������
			m_DamageState->InitDamageState();
		}
		//�����œ��������Ƃ�
		else if(m_WalkState->GetExecute())
		{
			//�^����_���[�W��ݒ肷��
			m_DamageState->SetDamage(-this->GetVelocity());
		}
		//���s�֐��̐ݒ�
		m_DamageState->SetExecute(true);
		m_WalkState->SetExecute(false);
		//�_�ł̐ݒ�
		TextureBase::SetBlink(true);
		//�_���[�W����ݒ肷��
		staticData->Add("�v���C���[�_���[�W", babul->GetName());
	}
}

//���]��ɓ��������Ƃ��̏���
void CoroKun::OnCollisionRebound(ITextureBase* rebound)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�摜�������o��
	TextureBase::Extrude(this);

	//���V��̍��W�����킹��
	m_warpPos += -this->GetVelocity();

	//�̓�����̓����蔻��
	m_BodyBlow->WithPlayer(this);

	//�E�C���X�r�[���̓����蔻��
	m_VirusBeam->WithPlayer(this);

	//�w�ʂ̃e�N�X�`���������o��
	m_backTexture->Extrude(this);

	//�̓����蒆�̎��́A���s�֐��𗎂Ƃ�
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}

	//���̃I�u�W�F�N�g�ɓ���������AWalkState��true�ɂ��āAAccelState�𗎂Ƃ�
	m_WalkState->SetExecute(true);
	m_AccelState->SetExecute(false);

	//���g�̃x�N�g�����擾����
	Vector3 vel = this->GetVelocity();
	vel.Normalize();

	//�_�ł��Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�^����_���[�W��ݒ肷��
		m_DamageState->SetDamage(Vector3(SPEED) * -vel);
		//���s�֐��̐ݒ�
		m_DamageState->SetExecute(true);
		m_WalkState->SetExecute(false);
		//�_���[�W����ݒ肷��
		staticData->Add("�v���C���[�_���[�W", rebound->GetName());
		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__37_���]��);
	}
}

void CoroKun::OnCollisionSwitch(ITextureBase* switchi)
{
	//�摜�������o��
	TextureBase::Extrude(this);

	//���V��̍��W�����킹��
	m_warpPos += -this->GetVelocity();

	//�̓�����̓����蔻��
	m_BodyBlow->WithPlayer(this);

	//�E�C���X�r�[���̓����蔻��
	m_VirusBeam->WithPlayer(this);

	//�w�ʂ̃e�N�X�`���������o��
	m_backTexture->Extrude(this);

	//�̓����蒆�̎��́A���s�֐��𗎂Ƃ�
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}
}

void CoroKun::OnCollisionFoleFloor(ITextureBase* fole)
{
	//�摜�������o��
	TextureBase::Extrude(this);

	//���V��̍��W�����킹��
	m_warpPos += -this->GetVelocity();

	//�̓�����̓����蔻��
	m_BodyBlow->WithPlayer(this);

	//�E�C���X�r�[���̓����蔻��
	m_VirusBeam->WithPlayer(this);

	//�w�ʂ̃e�N�X�`���������o��
	m_backTexture->Extrude(this);

	//���̃I�u�W�F�N�g�ɓ���������AWalkState��true�ɂ��āAAccelState�𗎂Ƃ�
	m_WalkState->SetExecute(true);
	m_AccelState->SetExecute(false);

	//�̓����蒆�̎��́A���s�֐��𗎂Ƃ�
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}
}

void CoroKun::OnCollisionHyperEnemy(ITextureBase* hyene)
{	
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�G�̃C���X�^���X���擾����
	HyperEnemy* hyperenemy = dynamic_cast<HyperEnemy*>(hyene);

	//�G�ɓ��������Ƃ�
	if(!this->GetBlink() && !hyperenemy->GetBlink())
	{
		//�r�[�������s��
		if (m_VirusBeam->GetActive())
		{
			//�X�e�[�g��؂�ւ���
			m_VirusBeam->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//���s�֐��𗎂Ƃ�
			m_VirusBeam->SetActive(false);

			//�r�[���̏�����
			m_VirusBeam->InitBeam();
			m_VirusBeam->GetMainParticle()->Initialize();

			//�r�[���̕`���؂�
			for (Particle* particle : m_VirusBeam->GetAllParticleList())
			{
				particle->SetState(STATE::NONE);
			}
		}

		//�Z���g���Ă��Ȃ��Ƃ�
		if (!m_BodyBlow->GetActive())
		{
			//�摜�������o��
			TextureBase::Extrude(this);

			//���V��̍��W�����킹��
			m_warpPos += -this->GetVelocity();

			//�̓�����̓����蔻��
			m_BodyBlow->WithPlayer(this);

			//�E�C���X�r�[���̓����蔻��
			m_VirusBeam->WithPlayer(this);

			//�w�ʂ̃e�N�X�`���������o��
			m_backTexture->Extrude(this);

			//�_���[�W�x�N�g����ݒ肷��
			Vector3 vel = -this->GetVelocity();

			//�x�N�g�����[���x�N�g���̏ꍇ�́A�x�N�g����␳����
			if (vel.x == 0.0f)vel = Vector3(SPEED * -hyene->DirectX(), 0.0f, 0.0f);

			//�_���[�W���󂯂�
			m_DamageState->SetDamage(vel);

			//���s�֐��̐ݒ�
			m_DamageState->SetExecute(true);
			m_WalkState->SetExecute(false);

			//�_�ł�����
			this->SetBlink(true);

			//�_���[�W����ݒ肷��
			staticData->Add("�v���C���[�_���[�W", hyene->GetName());

			//�_���[�W���󂯂�
			this->OnDamage(hyperenemy->GetAtack(), this->GetDefence());
		}
	}
}

void CoroKun::OnCollision_SuperEnemy(ITextureBase* super)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�G�̃C���X�^���X���擾����
	SuperEnemy* superenemy = dynamic_cast<SuperEnemy*>(super);

	//�G�ɓ��������Ƃ�
	if (!this->GetBlink() && !superenemy->GetBlink())
	{
		//�r�[�������s��
		if (m_VirusBeam->GetActive())
		{
			//�X�e�[�g��؂�ւ���
			m_VirusBeam->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//���s�֐��𗎂Ƃ�
			m_VirusBeam->SetActive(false);

			//�r�[���̏�����
			m_VirusBeam->InitBeam();
			m_VirusBeam->GetMainParticle()->Initialize();

			//�r�[���̕`���؂�
			for (Particle* particle : m_VirusBeam->GetAllParticleList())
			{
				particle->SetState(STATE::NONE);
			}
		}

		//�Z���g���Ă��Ȃ��Ƃ�
		if (!m_BodyBlow->GetActive())
		{
			//�摜�������o��
			TextureBase::Extrude(this);

			//���V��̍��W�����킹��
			m_warpPos += -this->GetVelocity();

			//�̓�����̓����蔻��
			m_BodyBlow->WithPlayer(this);

			//�E�C���X�r�[���̓����蔻��
			m_VirusBeam->WithPlayer(this);

			//�w�ʂ̃e�N�X�`���������o��
			m_backTexture->Extrude(this);

			//�_���[�W�x�N�g����ݒ肷��
			Vector3 vel = -this->GetVelocity();

			//�x�N�g�����[���x�N�g���̏ꍇ�́A�x�N�g����␳����
			if (vel.x == 0.0f)vel = Vector3(SPEED * -super->DirectX(), 0.0f, 0.0f);

			//�_���[�W���󂯂�
			m_DamageState->SetDamage(vel);

			//���s�֐��̐ݒ�
			m_DamageState->SetExecute(true);
			m_WalkState->SetExecute(false);

			//�_�ł�����
			this->SetBlink(true);

			//�_���[�W����ݒ肷��
			staticData->Add("�v���C���[�_���[�W", super->GetName());

			//�_���[�W���󂯂�
			this->OnDamage(superenemy->GetAtack(), this->GetDefence());
		}
	}
}

void CoroKun::OnCollision_NormalEnemy(ITextureBase* normal)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�G�̃C���X�^���X���擾����
	NormalEnemy* normalEnemy = dynamic_cast<NormalEnemy*>(normal);

	//�G�ɓ��������Ƃ�
	if (!this->GetBlink() && !normalEnemy->GetBlink())
	{
		//�r�[�������s��
		if (m_VirusBeam->GetActive())
		{
			//�X�e�[�g��؂�ւ���
			m_VirusBeam->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//���s�֐��𗎂Ƃ�
			m_VirusBeam->SetActive(false);

			//�r�[���̏�����
			m_VirusBeam->InitBeam();
			m_VirusBeam->GetMainParticle()->Initialize();

			//�r�[���̕`���؂�
			for (Particle* particle : m_VirusBeam->GetAllParticleList())
			{
				particle->SetState(STATE::NONE);
			}
		}

		//�Z���g���Ă��Ȃ��Ƃ�
		if (!m_BodyBlow->GetActive())
		{
			//�摜�������o��
			TextureBase::Extrude(this);

			//���V��̍��W�����킹��
			m_warpPos += -this->GetVelocity();

			//�̓�����̓����蔻��
			m_BodyBlow->WithPlayer(this);

			//�E�C���X�r�[���̓����蔻��
			m_VirusBeam->WithPlayer(this);

			//�w�ʂ̃e�N�X�`���������o��
			m_backTexture->Extrude(this);

			//�_���[�W�x�N�g����ݒ肷��
			Vector3 vel = -this->GetVelocity();

			//�x�N�g�����[���x�N�g���̏ꍇ�́A�x�N�g����␳����
			if (vel.x == 0.0f)vel = Vector3(SPEED * -normalEnemy->DirectX(), 0.0f, 0.0f);

			//�_���[�W���󂯂�
			m_DamageState->SetDamage(vel);

			//���s�֐��̐ݒ�
			m_DamageState->SetExecute(true);
			m_WalkState->SetExecute(false);

			//�_�ł�����
			this->SetBlink(true);

			//�_���[�W����ݒ肷��
			staticData->Add("�v���C���[�_���[�W", normalEnemy->GetName());

			//�_���[�W���󂯂�
			this->OnDamage(normalEnemy->GetAtack(), this->GetDefence());
		}
	}
}

void CoroKun::OnCollisionWhiteBack(ITextureBase* whiteback)
{
	bool flag = true;

	//�摜�������o��
	TextureBase::Extrude(this);

	//���V��̍��W�����킹��
	m_warpPos += -this->GetVelocity();

	//�̓�����̓����蔻��
	m_BodyBlow->WithPlayer(this);

	//�E�C���X�r�[���̓����蔻��
	m_VirusBeam->WithPlayer(this);

	//�w�ʂ̃e�N�X�`���������o��
	m_backTexture->Extrude(this);

	//���̃I�u�W�F�N�g�ɓ���������A
	//WalkState��true�ɂ��āAAccelState�𗎂Ƃ�
	m_WalkState->SetExecute(true);
	m_AccelState->SetExecute(false);

	//�̓����蒆�̎��́A���s�֐��𗎂Ƃ�
	if (m_BodyBlow->GetActive())
	{
		m_BodyBlow->SetActive(false);
	}
}

void CoroKun::OnCollisionHyperBabble(ITextureBase* highbabble)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�G�̃C���X�^���X���擾����
	SoapBabble* babble = dynamic_cast<SoapBabble*>(highbabble);
	HyperEnemy* enemy = dynamic_cast<HyperEnemy*>(babble->GetOwner());

	//�ړ������r�[����ł��Ă���Ƃ�(�_���[�W���󂯂Ă��Ȃ��Ƃ�)
	if (m_WalkState->GetExecute() && !this->GetBlink() ||
		m_VirusBeam->GetActive() && !this->GetBlink())
	{
		//�摜�������o��
		TextureBase::Extrude(this);

		//���V��̍��W�����킹��
		m_warpPos += -this->GetVelocity();

		//�̓�����̓����蔻��
		m_BodyBlow->WithPlayer(this);

		//�E�C���X�r�[���̓����蔻��
		m_VirusBeam->WithPlayer(this);

		//�w�ʂ̃e�N�X�`���������o��
		m_backTexture->Extrude(this);

		//�_���[�W�x�N�g����ݒ肷��
		Vector3 vel = -this->GetVelocity();

		//�G�̃_���[�W�x�N�g����ݒ肷��
		Vector3 babblevel = highbabble->GetVelocity();

		//�x�N�g���𐳋K������
		babblevel.Normalize();

		//�x�N�g�����[���x�N�g���̏ꍇ�́A�x�N�g����␳����
		if (vel.x == 0.0f)vel = Vector3(SPEED * babblevel.x, 0.0f, 0.0f);

		//�_���[�W���󂯂�
		m_DamageState->SetDamage(vel);

		//�_�ł�����
		this->SetBlink(true);

		//���s�֐��̐ݒ�
		m_DamageState->SetExecute(true);
		m_WalkState->SetExecute(false);

		//�r�[�������s��(�`���[�W���̎�)
		if (m_VirusBeam->GetActive())
		{
			//�X�e�[�g��؂�ւ���
			m_VirusBeam->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//���s�֐��𗎂Ƃ�
			m_VirusBeam->SetActive(false);

			//�r�[���̏�����
			m_VirusBeam->InitBeam();
			m_VirusBeam->GetMainParticle()->Initialize();

			//�r�[���̕`���؂�
			for (Particle* particle : m_VirusBeam->GetAllParticleList())
			{
				particle->SetState(STATE::NONE);
			}
		}

		//�_���[�W����ݒ肷��
		staticData->Add("�v���C���[�_���[�W", highbabble->GetName());

		//�_���[�W���󂯂�
		this->OnDamage(enemy->GetAtack(), this->GetDefence());
	}
}

void CoroKun::OnCollision_SuperBabble(ITextureBase* superbabble)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�G�̃C���X�^���X���擾����
	SoapBabble* babble = dynamic_cast<SoapBabble*>(superbabble);
	SuperEnemy* enemy = dynamic_cast<SuperEnemy*>(babble->GetOwner());

	//�ړ������r�[����ł��Ă���Ƃ�(�_���[�W���󂯂Ă��Ȃ��Ƃ�)
	if (m_WalkState->GetExecute() && !this->GetBlink() ||
		m_VirusBeam->GetActive() && !this->GetBlink())
	{
		//�摜�������o��
		TextureBase::Extrude(this);

		//���V��̍��W�����킹��
		m_warpPos += -this->GetVelocity();

		//�̓�����̓����蔻��
		m_BodyBlow->WithPlayer(this);

		//�E�C���X�r�[���̓����蔻��
		m_VirusBeam->WithPlayer(this);

		//�w�ʂ̃e�N�X�`���������o��
		m_backTexture->Extrude(this);

		//�_���[�W�x�N�g����ݒ肷��
		Vector3 vel = -this->GetVelocity();

		//�G�̃_���[�W�x�N�g����ݒ肷��
		Vector3 babblevel = superbabble->GetVelocity();

		//�x�N�g���𐳋K������
		babblevel.Normalize();

		//�x�N�g�����[���x�N�g���̏ꍇ�́A�x�N�g����␳����
		if (vel.x == 0.0f)vel = Vector3(SPEED * babblevel.x, 0.0f, 0.0f);

		//�_���[�W���󂯂�
		m_DamageState->SetDamage(vel);

		//�_�ł�����
		this->SetBlink(true);

		//���s�֐��̐ݒ�
		m_DamageState->SetExecute(true);
		m_WalkState->SetExecute(false);

		//�r�[�������s��(�`���[�W���̎�)
		if (m_VirusBeam->GetActive())
		{
			//�X�e�[�g��؂�ւ���
			m_VirusBeam->SetNowState(ENERGY_CHAGE::ENERGYNONE);

			//���s�֐��𗎂Ƃ�
			m_VirusBeam->SetActive(false);

			//�r�[���̏�����
			m_VirusBeam->InitBeam();
			m_VirusBeam->GetMainParticle()->Initialize();

			//�r�[���̕`���؂�
			for (Particle* particle : m_VirusBeam->GetAllParticleList())
			{
				particle->SetState(STATE::NONE);
			}
		}

		//�_���[�W����ݒ肷��
		staticData->Add("�v���C���[�_���[�W", superbabble->GetName());

		//�␳�l
		float correction = 1.0f;

		//���������̂��e�̖A�̎��̓_���[�W��2�{�ɂȂ�
		if (babble == enemy->GetTurretList()[enemy->GetNowData2()]->GetParentBabble())
		{
			correction = 2.0f;
		}

		//�_���[�W���󂯂�
		this->OnDamage(enemy->GetAtack(), this->GetDefence(), correction);
	}
}

void CoroKun::OnCollision_Goal(ITextureBase* goal)
{
	//�I���̏���
	this->Finish(PLAYERMOVE::CLEAR,CRI_CUESHEET_0__29_GOAL);
}


void CoroKun::InputAngle()
{
	//�L�[�̏�Ԃ��擾����
	Input* input = Input::GetInstance();

	//�L�[�{�[�h�̏�Ԃ��擾����
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//�̓����蒆�͉������Ȃ�
	if (m_AccelState->GetExecute())return;

	//�ړ����Ă��Ȃ��Ƃ��́A�������Ȃ�
	if (!m_WalkState->GetExecute())return;

	//�E����������
	if (keyState.Right && !this->ALLTechniqueExe())
	{
		this->SetAngle(0.0f);
		this->SetSpriteEffect(SpriteEffects::SpriteEffects_None);
		return;
	}

	//��������
	if (keyState.Left && !this->ALLTechniqueExe())
	{
		this->SetAngle(0.0f);
		this->SetSpriteEffect(SpriteEffects::SpriteEffects_FlipHorizontally);
		return;
	}
}

void CoroKun::CreateTechnique()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManager���擾����
	TextureManager* texMana = GameContext::Get<PlayScene>()->GetGameManager().get();

	//CollisionManager���擾����
	CollisionManager* colMana = GameContext::Get<CollisionManager>();

	//�g�p����e�N�X�`�����擾����
	ITextureBase* blow = resourceManager->GetTexture("�Փ�1");
	ITextureBase* beam1 = resourceManager->GetTexture("�r�[��1");
	ITextureBase* player1 = resourceManager->GetTexture("SickChar");
	ITextureBase* player2 = resourceManager->GetTexture("SickChar2"); 
	ITextureBase* circle = resourceManager->GetTexture("���q");

	//�Z�̐ݒ������

	//�Փ˃G�t�F�N�g�𐶐�����
	std::unique_ptr<BodyBlow>pBody = std::make_unique<BodyBlow>(blow);
	m_BodyBlow = pBody.get();
	m_BodyBlow->SetName("�Փ�");

	//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("�̓�����"))
	{
		//�A�j���[�V�����̎g�p����e�N�X�`�����擾����
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//�A�j���[�V��������ݒ肷��
		m_BodyBlow->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//���̃A�j���[�V�����̏���ݒ肷��
		m_BodyBlow->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		m_BodyBlow->SetEndTime(importer->GetAnimation(), importer->GetEnd());
	}

	//�A�j���[�V�����̏�����
	m_BodyBlow->SetAniFlag(true);
	m_BodyBlow->SetLoopFlag(true);

	m_BodyBlow->SetNowInfo("�Փ�1");
	m_BodyBlow->SetLoopInfo("�Փ�2");
	m_BodyBlow->SetNowAnimation(m_BodyBlow->GetRect());

	//GameContext�ɒǉ�����
	GameContext::Register<BodyBlow>(m_BodyBlow);

	//TextureManager�ɒǉ�����
	GameContext::Get<PlayScene>()->GetGameManager()->Add(std::move(pBody));

	//�r�[���G�t�F�N�g�𐶐�����
	std::unique_ptr<VirusBeam> pVirus = std::make_unique<VirusBeam>(beam1);
	m_VirusBeam = pVirus.get();
	m_VirusBeam->SetName("�E�C���X�r�[��");
	m_VirusBeam->SetDrawTimeLimit(180);
	m_VirusBeam->SetBeamSpeed(20);
	m_VirusBeam->SetDrawLimit(700);
	m_VirusBeam->SetColor(Color(1.0f, 1.0f, 1.0f, 0.5f));

	//CollisionManager�ɒǉ�����
	GameContext::Get<CollisionManager>()->Add(m_VirusBeam);

	GameContext::Get<PlayScene>()->GetGameManager()->Add(std::move(pVirus));

	//Needle�̒������͈͍U���𐶐�����
	m_NeedleOverAll = std::make_unique<NeedleOverall>();

	//Needle�̒Z�����͈͍U���𐶐�����
	m_Resurrection = std::make_unique<Resurrection>();

	//�A�j���[�V�����̏�����������

//--------------------------�L�͈͂̃A�j���[�V�����̐ݒ�--------------------------//

	//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Needle1"))
	{
		//�A�j���[�V�����̎g�p����e�N�X�`�����擾����
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//�A�j���[�V��������ݒ肷��
		this->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//���̃A�j���[�V�����̏���ݒ肷��
		this->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		this->SetEndTime(importer->GetAnimation(), importer->GetEnd());
	}

//--------------------------�L�͈͂̃A�j���[�V�����̐ݒ�----------------------//

//--------------------------���͈͗p�̃A�j���[�V�����̐ݒ�----------------------//

	//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Needle2"))
	{
		//�A�j���[�V�����̎g�p����e�N�X�`�����擾����
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//�A�j���[�V��������ݒ肷��
		this->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//���̃A�j���[�V�����̏���ݒ肷��
		this->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		this->SetEndTime(importer->GetAnimation(), importer->GetEnd());
	}

//--------------------------���͈͗p�̃A�j���[�V�����̐ݒ�----------------------//

//--------------------------���S�p�̃A�j���[�V�����̐ݒ�----------------------//

		//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("Dead"))
	{
		//�A�j���[�V�����̎g�p����e�N�X�`�����擾����
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//�A�j���[�V��������ݒ肷��
		this->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//���̃A�j���[�V�����̏���ݒ肷��
		this->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
		this->SetEndTime(importer->GetAnimation(), importer->GetEnd());
	}

//--------------------------���S�p�̃A�j���[�V�����̐ݒ�----------------------//
	
	//�v���C���[�̌��݂̃A�j���[�V�����̐ݒ�
	this->SetAniFlag(false);
	this->SetLoopFlag(false);

	this->SetNowInfo("�ʏ�1");
	this->SetLoopInfo("�ʏ��1");
	this->SetNowAnimation(player1->GetRect());

	//�ʏ�̋Z�𐶐�����
	m_Normal = std::make_unique<NormalTechnique>();
}

void CoroKun::OnDamage(const int& atack, const int& defence,const float correction)
{
	//HP���擾����
	int hp = this->GetHP();

	//rect���擾����
	RECT rect = m_HPGage->GetRect();

	//�_���[�W���v�Z����
	StatusCalc::CalcDamage(hp, atack, defence,
		m_HPGage->GetMaxTexLength(), this->GetMaxHP(), rect.right,correction);

	//HP��ݒ肷��
	this->SetHP(hp);

	//RECT��ݒ肷��
	m_HPGage->SetRect(rect);

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SE��炷
	soundManager->Play(CRI_CUESHEET_0__11_�_���[�W);

	//�_���[�W�p�̃t���O�𗧂Ă�
	m_damageCheck = true;

	//HP�Q�[�W�̗��ʂ̃e�N�X�`�����擾����
	ITextureBase* backHpTexture = m_HPGage->GetBackGageTexture();

	//HP�Q�[�W�̗��ʂ̐F��ݒ肷��
	backHpTexture->SetColor(Colors::Red);

	//�_���[�W�̃J�E���g������������
	m_HPGage->SetEasingTime(0.0f);

	//�ʏ�U��������������
	if (m_Normal->GetFrontBullet() != nullptr)
	{
		m_Normal->GetFrontBullet()->Initialize();
	}
	m_Normal->GetNormalGage()->Initialize();

	//SE���~�߂�
	soundManager->Stop(CRI_CUESHEET_0__14_�`���[�W);
}

void CoroKun::OnCommand(const int& cost)
{
	//MP���擾����
	int mp = this->GetMP();

	//rect���擾����
	RECT rect = m_mpGage->GetRect();

	//�_���[�W���v�Z����
	StatusCalc::CalcDamage(mp, cost, NULL,
		m_mpGage->GetMaxTexLength(), this->GetMaxMP(), rect.right, 1.0f);

	//MP��ݒ肷��
	this->SetMP(mp);

	//RECT��ݒ肷��
	m_mpGage->SetRect(rect);
}

void CoroKun::OnSpesial(const int& cost)
{
	//SP���擾����
	int sp = this->GetSpecial();

	//rect���擾����
	RECT rect = m_specialGage->GetRect();

	//�_���[�W���v�Z����
	StatusCalc::CalcDamage(sp, cost, NULL,
		m_specialGage->GetMaxTexLength(), this->GetMaxSpecialPoint(), rect.right, 1.0f);

	//SP��ݒ肷��
	this->SetSpecial(sp);

	//RECT��ݒ肷��
	m_specialGage->SetRect(rect);
}

void CoroKun::OnHealCommand(int& heal)
{
	//MP���擾����
	int mp = this->GetMP();

	//rect���擾����
	RECT rect = m_mpGage->GetRect();

	//�񕜗ʂ��v�Z����
	StatusCalc::CalcHeal(mp, heal, m_mpGage->GetMaxTexLength(), this->GetMaxMP(), rect.right);

	//MP��ݒ肷��
	this->SetMP(mp);

	//RECT��ݒ肷��
	m_mpGage->SetRect(rect);
}

void CoroKun::OnHealSpecial(int& heal)
{
	//SpecialPoint���擾����
	int sp = this->GetSpecial();

	//rect���擾����
	RECT rect = m_specialGage->GetRect();

	//�񕜗ʂ��v�Z����
	StatusCalc::CalcHeal(sp, heal, m_specialGage->GetMaxTexLength(), this->GetMaxSpecialPoint(), rect.right);

	//SP��ݒ肷��
	this->SetSpecial(sp);

	//RECT��ݒ肷��
	m_specialGage->SetRect(rect);
}

void CoroKun::OnHealHp(int& heal)
{
	//HP���擾����
	int hp = this->GetHP();

	//rect���擾����
	RECT rect = m_HPGage->GetRect();

	//�񕜗ʂ��v�Z����
	StatusCalc::CalcHeal(hp, heal, m_HPGage->GetMaxTexLength(), this->GetMaxHP(), rect.right);

	//HP��ݒ肷��
	this->SetHP(hp);

	//RECT��ݒ肷��
	m_HPGage->SetRect(rect);
}

void CoroKun::OnHealExp(int& heal)
{
	//�擾�o���l���擾����
	int next = this->GetNextLevelPoint();

	//rect���擾����
	RECT rect = m_expGage->GetRect();

	//�񕜗ʂ��v�Z����
	StatusCalc::CalcHeal(next, heal, m_expGage->GetMaxTexLength(), this->GetMaxNextLevelPoint(), rect.right);

	//HP��ݒ肷��
	this->SetNextLevelPoint(next);

	//RECT��ݒ肷��
	m_expGage->SetRect(rect);
}

void CoroKun::Finish(const PLAYERMOVE& state, const int& sound)
{
	//PlayScene���擾����
	PlayScene* play = GameContext::Get<PlayScene>();

	//�^�C�}�[���擾����
	TimerJudge* timer = GameContext::Get<TimerJudge>();

	//�S�[�����o���擾����
	TimeLag* finish = play->GetLagFinish().get();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�̓������~
	m_WalkState->SetExecute(false);

	//�Q�[�W���\���ɂ���
	m_Normal->GetNormalGage()->SetState(STATE::NONE);
	m_Normal->GetBackGage()->SetState(STATE::NONE);

	//������ʂ��̂́A��x����
	if (!finish->GetFrameFrag())
	{
		finish->SetFrameFlag(true);
		timer->SetTimerUp(false);
		play->SetNowState(BACKOUT::ADMISSON);
		this->AddData2(state);

		//SE���Đ�����
		soundManager->Play(sound);
	}
}

void CoroKun::OnExeDeathBlowHyperEnemy()
{
	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//�X�e�[�W���擾����
	Stage* stage = GameContext::Get<Stage>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//HyperEnemy���擾����
	std::vector<HyperEnemy*> hyperEnemies = stage->GetHyperEnemies();

	for (HyperEnemy* enemy : hyperEnemies)
	{
		//���łɂ���Ă���Ƃ��́A�J�E���g���Ȃ�
		if (enemy->GetState() == STATE::NONE)continue;

		//��ʓ��ɂ���G�ɂ̂݁A�_���[�W��^����
		if (cameraManager->ScreenCheck(enemy->GetPosition()))
		{
			//�_�ł�����
			enemy->SetBlink(true);

			//�A������
			for (SoapBabble* babble : enemy->GetBabbleList())
			{
				babble->SetState(STATE::NONE);
			}

			//�G�Ƀ_���[�W��^����(�K�E�Z�́A�{�̃_���[�W��^����)
			enemy->OnDamage(this->GetAtack(), enemy->GetDefence(), 2.0f);

			//�X�R�A��ǉ�����
			score->AddScore(enemy->GetLevel() * DirecTingScene::BASE_SCORE);
			score->SetFrameSave(enemy->GetLevel() * DirecTingScene::BASE_SCORE, 60.0f);

			//�G�t�F�N�g���Đ�����
			enemy->GetMasicEffectManager()->NowEffectPlay
			(DirecTingScene::TECHNIQUE_HEAL_COMMAND, DirecTingScene::TECHNIQUE_HEAL_SPECIAL);
		}
	}
}

void CoroKun::OnExeDeathBlowSuperEnemy()
{
	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//�X�e�[�W���擾����
	Stage* stage = GameContext::Get<Stage>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//SuperEnemy���擾����
	std::vector<SuperEnemy*> superEnemies = stage->GetSuperEnemies();

	for (SuperEnemy* enemy : superEnemies)
	{
		//���łɂ���Ă���Ƃ��́A�J�E���g���Ȃ�
		if (enemy->GetState() == STATE::NONE)continue;

		//��ʓ��ɂ���G�ɂ̂݁A�_���[�W��^����
		if (cameraManager->ScreenCheck(enemy->GetPosition()))
		{
			//�_�ł�����
			enemy->SetBlink(true);

			//�A������
			for (std::unique_ptr<FixedTurret>& turret : enemy->GetTurretList())
			{
				for (SoapBabble* babble : turret->GetBabbleList())
				{
					babble->SetState(STATE::NONE);
				}
			}

			//�G�Ƀ_���[�W��^����(�K�E�Z�́A�{�̃_���[�W��^����)
			enemy->OnDamage(this->GetAtack(), enemy->GetDefence(), 2.0f);

			//�X�R�A��ǉ�����
			score->AddScore(enemy->GetLevel() * DirecTingScene::BASE_SCORE);
			score->SetFrameSave(enemy->GetLevel() * DirecTingScene::BASE_SCORE, 60.0f);

			//�G�t�F�N�g���Đ�����
			enemy->GetMasicEffectManager()->NowEffectPlay
			(DirecTingScene::TECHNIQUE_HEAL_COMMAND, DirecTingScene::TECHNIQUE_HEAL_SPECIAL);
		}

	}
}

void CoroKun::OnExeDeathBlowNormalEnemy()
{
	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//�X�e�[�W���擾����
	Stage* stage = GameContext::Get<Stage>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//NormalEnemy���擾����
	std::vector<NormalEnemy*> normalEnemies = stage->GetNormalEnemies();

	for (NormalEnemy* enemy : normalEnemies)
	{
		//���łɂ���Ă���Ƃ��́A�J�E���g���Ȃ�
		if (enemy->GetState() == STATE::NONE)continue;

		//��ʓ��ɂ���G�ɂ̂݁A�_���[�W��^����
		if (cameraManager->ScreenCheck(enemy->GetPosition()))
		{
			//�_�ł�����
			enemy->SetBlink(true);

			//�G�Ƀ_���[�W��^����(�K�E�Z�́A�{�̃_���[�W��^����)
			enemy->OnDamage(this->GetAtack(), enemy->GetDefence(), 2.0f);

			//�X�R�A��ǉ�����
			score->AddScore(enemy->GetLevel() * DirecTingScene::BASE_SCORE);
			score->SetFrameSave(enemy->GetLevel() * DirecTingScene::BASE_SCORE, 60.0f);

			//�G�t�F�N�g���Đ�����
			enemy->GetMasicEffectManager()->NowEffectPlay
			(DirecTingScene::TECHNIQUE_HEAL_COMMAND, DirecTingScene::TECHNIQUE_HEAL_SPECIAL);
		}

	}
}

const bool CoroKun::ALLTechniqueExe()
{
	if (m_BodyBlow->GetActive())return true;

	if (m_VirusBeam->GetActive())return true;

	if (m_NeedleOverAll->GetActive())return true;

	if (m_Resurrection->GetActive())return true;

	return false;
}
