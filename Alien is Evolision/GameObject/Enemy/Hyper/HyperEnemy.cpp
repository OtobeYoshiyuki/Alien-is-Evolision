#include "HyperEnemy.h"
#include "pch.h"
#include "HyperEnemy.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Stage/Stage.h"
#include "GameObject/Stage/Maptip/Floor/Floor.h"
#include "GameObject/Stage/Maptip/MapTip.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameContext.h"	
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "OtobeLib/FrameWork/Score/Score.h"
#include "GameObject/Item/Drop/ItemDrop.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�G�̎v�l����
const float HyperEnemy::THINKTIME = 5.0f;

//�G�̋���(�o���l�ɉe������)
const float HyperEnemy::STRENGTH = 300.0f;

HyperEnemy::HyperEnemy(ITextureBase* texture)
	:TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_JumpState(nullptr),//JumpState
	m_AutoMoveState(nullptr),//AutoMoveState
	m_AtackTime(NULL),//�U������
	m_ArrayBabble(-1),//BabulList�̎Q�Ɨp�̓Y����
	m_ThinkingTime(0.0f),//�G�̎v�l����
	m_HpGage(nullptr),//HP�Q�[�W
	m_HpBack(nullptr),//HP�Q�[�W�̗���
	m_Result(false),//�I���t���O
	m_experience(nullptr),//�o���l�̉摜
	m_masicEffectManager(nullptr),//�Z�̃G�t�F�N�g�̊Ǘ��N���X
	m_frameCount(NULL),//�t���[�����̃J�E���g
	m_damageCheck(false)//�_���[�W�̃`�F�b�N�t���O
{
	//ResourcesManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	
	//GameManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//�`�揇��ύX
	this->SetDrawOrder(5);

	//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("HyperEnemy"))
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

	//�A�j���[�V�����̃t���O��ݒ肷��
	this->SetAniFlag(true);
	this->SetLoopFlag(true);

	//���݂̃A�j���[�V��������ݒ肷��
	this->SetNowInfo("�n�C�p�[2");
	this->SetLoopInfo("�n�C�p�[4");
	this->SetNowAnimation(this->GetNowAnimation());

	//�X�e�[�g�̐���
	m_AutoMoveState = std::make_unique<AutoMoveState>();
	m_JumpState = std::make_unique<JumpState>();

	//�X�e�[�g�����X�g�ɒǉ�����
	m_MoveStateList.push_back(m_AutoMoveState.get());
	m_MoveStateList.push_back(m_JumpState.get());

	//�e�X�e�[�g�̐ݒ���s��

	//�����ړ��̃X�e�[�g
	m_AutoMoveState->SetExecute(true);
	m_AutoMoveState->SetNowState(DIRECT::LEFT);
	m_AutoMoveState->SetSpeed(Vector3(2.0f, 0.0f, 0.0f));

	//�W�����v�̃X�e�[�g
	m_JumpState->SetExecute(false);
	m_JumpState->SetJumpOption(false);
	m_JumpState->SetJumpLimit(200.0f);
	m_JumpState->SetJumpSpeed(10.0f);
	m_JumpState->SetCorrection(0.5f);
	m_JumpState->GetOM()->SetOwner(this);

	//�_�ł̊Ԋu��ݒ肷��
	TextureBase::SetTarget(0.1f);

	//�_�ł̏I���̐ݒ������
	TextureBase::SetFinal(1);
	TextureBase::SetFinalOption(true);

	//���s�֐����N����
	this->SetActive(true);

	ITextureBase* Gage = resourceManager->GetTexture("WhiteBack");
	ITextureBase* ex = resourceManager->GetTexture("�o���l");

	//�񕜃A�C�e���̉摜���擾����
	ITextureBase* healTex = resourceManager->GetTexture("��3");

	//HP�Q�[�W�𐶐�����
	std::unique_ptr<HPGage>pHpGage = std::make_unique<HPGage>(Gage);
	pHpGage->SetColor(Colors::GreenYellow);
	pHpGage->SetRect(RECT{ 0,0,80,10 });
	pHpGage->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	pHpGage->SetMaxTexLength(80);
	pHpGage->SetDrawOrder(4);
	pHpGage->SetState(STATE::DRAW);
	m_HpGage = pHpGage.get();

	//HP�Q�[�W�̗����𐶐�����
	std::unique_ptr<ITextureBase>pHpBack = std::make_unique<TextureBase>(Gage);
	pHpBack->SetColor(Colors::White);
	pHpBack->SetRect(RECT{ 0,0,80,10 });
	pHpBack->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	pHpBack->SetDrawOrder(6);
	m_HpBack = pHpBack.get();

	//�o���l�𐶐�����
	std::unique_ptr<Experience> pex = std::make_unique<Experience>(ex);
	pex->SetDrawOrder(4);
	pex->SetState(STATE::NONE);
	m_experience = pex.get();

	//TextureManager�ɒǉ�����
	gameManager->Add(std::move(pHpGage));
	gameManager->Add(std::move(pHpBack));
	gameManager->Add(std::move(pex));

	//�摜�̑傫���𓖂��蔻��Ɏg�p����
	this->SetRadius(Vector3(this->AskOrigin() * 2));

	//HealManager���擾����
	HealManager* healManager = GameContext::Get<PlayScene>()->GetHealManager();

	//�񕜃A�C�e���𐶐�����
	healManager->CreateItem(healTex, [&](Item* item)
		{	Heal* healItem = dynamic_cast<Heal*>(item);
	m_healItem = healItem; });
}

void HyperEnemy::Initialize()
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	ITextureBase* shabondama1 = resourceManager->GetTexture("�V���{����1");

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//GameManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//Stage���擾����
	Stage* stage = GameContext::Get<Stage>();

	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�}�b�v�����擾����
	std::vector<std::vector<ITextureBase*>>& mapAll = stage->GetMap();

	//�V���{���ʂ̐������s��
	for (int i = 0; i < SOAPBABBLENUM; i++)
	{
		//�V���{���ʂ𐶐�����
		std::unique_ptr<SoapBabble> babble = std::make_unique<SoapBabble>(shabondama1);

		//���W��ݒ肷��
		babble->SetPosition(this->GetPosition());

		//�`���؂�
		babble->SetState(STATE::NONE);

		//���_��ύX����
		babble->SetOrigin(this->AskOrigin());

		//���O��ύX����
		babble->SetName("Hypersyabondama");

		//���g�̃|�C���^��ݒ肷��
		babble->SetOwner(this);

		//���|�C���^�����X�g�ɒǉ�����
		m_SoapBubbleList.push_back(babble.get());

		//�}�b�v����ǉ�����
		mapAll[mapAll.size() - 1].push_back(babble.get());

		//Manager�N���X�ɓo�^����
		gameManager->Add(std::move(babble));
	}

	//�o���l�̒ǔ��p�̃X�e�[�g�̐ݒ������

	//�x�N�g���̕␳�l��ݒ肷��
	m_experience->GetTrackingState()->SetCorrection(Vector3(CoroKun::SPEED));

	//�ǐՖڕW��ݒ肷��
	m_experience->GetTrackingState()->SetTarget(&player->GetPosition());

	//���g�̍��W��ݒ肷��
	m_experience->GetTrackingState()->SetThis(&m_experience->GetPosition());

	//���L�҂̃|�C���^��ݒ肷��
	m_experience->SetOwner(this);

	//MasicEffectManager�𐶐�����
	m_masicEffectManager = std::make_unique<MasicEffectManager>();
	m_masicEffectManager->Initialize(this);

	//�G�̃��x�������擾����
	int level = staticData->Get_IntData(this->GetName());

	//�X�e�[�^�X��ݒ肷��
	StatusImporter* importer = StatusBase::FindStatus(this->GetName(), level);
	this->SetStatus(importer->GetHp(), importer->GetMp(), importer->GetAtack(), importer->GetDefence(),
		importer->GetSpecial(), importer->GetLevel(), importer->GetNextLevelPoint());
	this->SetMaxStatus(importer->GetHp(), importer->GetMp(), importer->GetAtack(), importer->GetDefence(),
		importer->GetSpecial(), MAX_LEVEL, importer->GetNextLevelPoint());

	//�o���l�̕␳�l��ݒ肷��
	this->SetCompensation(STRENGTH);

	//��ɓG�̏㑤��HP�Q�[�W������悤�ɂ���
	m_HpGage->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	m_HpBack->SetPosition(this->GetPosition() - Vector3(0, 20, 0));

	//HP�Q�[�W�̏�����
	m_HpGage->CreateGage(gameManager);
}

void HyperEnemy::Update()
{
	//UI�̕\�����I���܂ŉ������Ȃ�
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//���s�֐��������Ă���Ƃ��́A�������Ȃ�
	if (!this->GetActive())return;

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//�A�j���[�V�����̍X�V����
	Animation::Update();

	//�U���̍X�V����
	this->UpdateAtack();

	//�Q�[�W�̍X�V����
	this->UpdateGage();

	//�X�e�[�g�̍X�V����
	this->UpdateState();

	//�摜�̍X�V����
	TextureBase::Update();

	//�摜�̐؂�o���ʒu���X�V����
	this->SetRect(this->GetNowAnimation());

	//�����蔻��̍��W��ݒ肷��
	this->SetCenter(this->GetPosition());

	//�_�Œ��̎�
	if (this->GetBlink())
	{
		switch (this->GetNowData2())
		{
		case COLOR_CHANGE::NONE:
			//�F�𔒂ɂ���
			this->SetColor(Colors::White);

			//5�t���[�����ƂɐF��؂�ւ���
			if (m_frameCount % 5 == NULL && m_frameCount != NULL)
			{
				//�X�e�[�g��؂�ւ���
				this->AddData2(COLOR_CHANGE::CHANGE);
			}

			break;
		case COLOR_CHANGE::CHANGE:
			//�F��Ԃɂ���
			this->SetColor(Colors::Red);

			//5�t���[�����ƂɐF��؂�ւ���
			if (m_frameCount % 5 == NULL && m_frameCount != NULL)
			{
				//�X�e�[�g��؂�ւ���
				this->AddData2(COLOR_CHANGE::NONE);
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


	//�����蔻���ǉ�����
	GameContext::Get<CollisionManager>()->AddUseHero(this);

	//�v���C���[���擾����
	CoroKun* player = GameContext::Get<CoroKun>();
	
	//���S���ȊO�͓����蔻���ݒ肷��
	if (this->GetHP() > NULL)
	{
		//�v���C���[���������Ă���Ƃ�
		if (player->GetHP() > NULL)
		{
			//�v���C���[�Ƃ̓����蔻���ݒ肷��
			GameContext::Get<CollisionManager>()->AddUseOther(player, this, 100.0f);

			//���͂ɐ�����j�Ƃ̓����蔻���ݒ肷��
			std::vector<Needle*> needleRe = GameContext::Get<Resurrection>()->GetNeedleList();

			for (Needle* needle : needleRe)
			{
				//�����蔻���ݒ肷��
				GameContext::Get<CollisionManager>()->AddUseOther(needle, this, 200.0f);
			}

			//�������ɔ�Ԑj�Ƃ̓����蔻���ݒ肷��
			std::vector<Needle*> needleOver = GameContext::Get<NeedleOverall>()->GetNeedleList();

			for (Needle* needle : needleOver)
			{
				//�����蔻���ݒ肷��
				GameContext::Get<CollisionManager>()->AddUseOther(needle, this, 100.0f);
			}

			//�ʏ�e���擾����
			std::vector<NormalBullet*> normalBulletList = GameContext::Get<NormalTechnique>()->GetBulletsList();

			for (NormalBullet* bullet : normalBulletList)
			{
				//�����蔻���ݒ肷��
				GameContext::Get<CollisionManager>()->AddUseOther(bullet, this, 200.0f);
			}
		}
	}
}

void HyperEnemy::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void HyperEnemy::Finalize()
{
}

void HyperEnemy::OnCollisionEnter(ITextureBase* obj)
{
	CollisionBase::OnCollisionEnter(obj);

	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);

	if (obj->GetName() == "floor")this->OnCollision_Floor(obj);

	if (obj->GetName() == "wall")this->OnCollision_Wall(obj);

	if (obj->GetName() == "WhiteBack")this->OnCollision_WhiteBack(obj);

	//�p�l���ɂ���āA�ĂԊ֐���ς���
	if (obj->GetName() == "LeftPanel")this->OnCollision_LeftPanel(obj);

	if (obj->GetName() == "RightPanel")this->OnCollision_RightPanel(obj);

	if (obj->GetName() == "LeftJumpPanel")this->OnCollision_LeftJumpPanel(obj);

	if (obj->GetName() == "RightJumpPanel")this->OnCollision_RightJumpPanel(obj);

	if (obj->GetName() == "shortNeedle")this->OnCollision_NeedleOverAll(obj);

	if (obj->GetName() == "longNeedle")this->OnCollision_Resurrection(obj);

	if (obj->GetName() == "�E�C���X�r�[��")this->OnCollision_VirusBeam(obj);

	if (obj->GetName() == "�ʏ�e")this->OnCollision_NormalBullet(obj);
}

void HyperEnemy::NonCollisionEnter(ITextureBase* obj)
{
	CollisionBase::NonCollisionEnter(obj);
}

void HyperEnemy::OnDamage(const int& atack, const int& defence, const float correction)
{
	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//HP���擾����
	int hp = this->GetHP();

	//rect���擾����
	RECT rect = m_HpGage->GetRect();

	//�_���[�W���v�Z����
	StatusCalc::CalcDamage(hp, atack, defence,
		m_HpGage->GetMaxTexLength(), this->GetMaxHP(), rect.right, correction);

	//HP��ݒ肷��
	this->SetHP(hp);

	//RECT��ݒ肷��
	m_HpGage->SetRect(rect);

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SE��炷
	soundManager->Play(CRI_CUESHEET_0__11_�_���[�W);

	if (score->GetSaving() == NULL)
	{
		//SE��炷
		soundManager->Play(CRI_CUESHEET_0__19_�X�R�A���Z);
	}

	//�_���[�W�p�̃t���O�𗧂Ă�
	m_damageCheck = true;

	//HP�Q�[�W�̗��ʂ̃e�N�X�`�����擾����
	ITextureBase* backHpTexture = m_HpGage->GetBackGageTexture();

	//HP�Q�[�W�̗��ʂ̐F��ݒ肷��
	backHpTexture->SetColor(Colors::Red);

	//�_���[�W�̃J�E���g������������
	m_HpGage->SetEasingTime(0.0f);
}

void HyperEnemy::UpdateState()
{
	//�x�N�g���̏�����
	Vector3 vel = Vector3::Zero;

	//Stage���擾
	Stage* stage = GameContext::Get<Stage>();

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�Ώۂ̉��ɂ�����̂𐔂���
	int botomNum = NULL;

	for (Floor* pfloor : stage->GetFloors())
	{
		if (stage->DirectlyBlowMap(this, pfloor) == nullptr)
		{
			//���ɉ����Ȃ��Ƃ��A�J�E���g���X�V����
			botomNum++;
		}
	}

	//���n���Ă���A���ɉ����Ȃ��Ƃ��A��������
	if (botomNum == (int)stage->GetFloors().size())
	{
		if (m_JumpState->GetPreState() == JUMP::LANDING)
		{
			m_JumpState->SetNowState(JUMP::FALL);
			m_JumpState->SetJumpSpeed(3.0f);
			m_JumpState->AddFall();
			//�I���������s���Ă��Ȃ��Ƃ�
			if (!m_Result)
			{
				this->SetNowInfo("�n�C�p�[5");
				this->SetLoopInfo("�n�C�p�[5");
			}
		}
	}

	//���s���̃X�e�[�g����x�N�g�����擾����
	for (MoveState*& state : m_MoveStateList)
	{
		vel += state->Update();
	}

	//�x�N�g����ݒ肷��
	this->SetVelocity(vel);

	//�f�[�^�����݂��邩�ǂ����A�m�F����
	if (!staticData->Request_Data("�n�C�p�[�G�l�~�[�_���[�W"))return;

	//�X�e�[�g�̍X�V����
	if (!m_Result)
	{
		if (this->GetFrameCount() > this->GetFinal())
		{
			m_AutoMoveState->SetExecute(true);
			this->SetNowInfo("�n�C�p�[2");
			this->SetLoopInfo("�n�C�p�[4");
			this->SetInitTimer(0);
		}
	}
}

void HyperEnemy::UpdateAtack()
{
	//�v���C���[�̃C���X�^���X���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�_���[�W���󂯂Ă���Ƃ��́A�������Ȃ�
	if (this->GetBlink())return;

	//�v���C���[���U�����̎��́A�������Ȃ�
	if (player->ALLTechniqueExe())return;

	//�̗͂��s������A�����
	//�����I�Ɏ��S�X�e�[�g�ɐ؂�ւ���
	if (this->GetHP() <= NULL && !m_Result)
	{
		this->AddData1(ENEMYSTATE::DEADSTART);
	}

	//�X�e�[�g�ɂ���āA�����𕪂���
	switch (this->GetNowData1())
	{
	case ENEMYSTATE::ENONE:
		this->State_AtackNone();
		break;
	case ENEMYSTATE::ATACKSTART:
		this->State_AtackStart();
		break;
	case ENEMYSTATE::ATACKING:
		this->State_Atacking();
		break;
	case ENEMYSTATE::ATACKRESUTL:
		this->State_AtackResult();
		break;
	case ENEMYSTATE::NEXTSTATE:
		this->State_AtackNext();
		break;
	case ENEMYSTATE::DEADSTART:
		this->State_DeadStart();
		break;
	case ENEMYSTATE::DEADTING:
		this->State_DeadTing();
		break;
	case ENEMYSTATE::DEAD:
		this->State_Dead();
		break;
	}
}

void HyperEnemy::UpdateGage()
{
	//��ɓG�̏㑤��HP�Q�[�W������悤�ɂ���
	m_HpGage->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	m_HpBack->SetPosition(this->GetPosition() - Vector3(0, 20, 0));
	m_HpGage->GetBackGageTexture()->SetPosition(m_HpGage->GetPosition());

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//�O��̃t���[������̌o�ߎ��Ԃ��擾����
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//�_���[�W���󂯂Ă���Ƃ�
	if (m_damageCheck)
	{
		//�w�ʂ̃e�N�X�`�����擾����
		ITextureBase* backTexture = m_HpGage->GetBackGageTexture();

		//HP�Q�[�W�̗��ʂ̐؂�o���ʒu���擾����
		RECT back = m_HpGage->GetBackGageTexture()->GetRect();

		//HP�Q�[�W�̐؂�o���ʒu���擾����
		RECT front = m_HpGage->GetRect();

		//�_���[�W�̍��������߂�
		int diffrent = (int)(back.right - front.right);

		//���݂̎��Ԃ��擾����
		float time = m_HpGage->GetEasingTime();

		//���Ԃ��X�V����
		time += elapsedTime;

		//�_���[�W���X�J���␳����
		diffrent *= time;

		//�Q�[�W�ɓ���������
		back.right -= diffrent;

		//�؂�o���ʒu���Đݒ肷��
		m_HpGage->GetBackGageTexture()->SetRect(back);

		//���Ԃ��Đݒ肷��
		m_HpGage->SetEasingTime(time);

		if (m_HpGage->GetEasingTime() >= 1.0f)
		{
			//1�b�o�߂�����A�t���O��؂�
			m_damageCheck = false;

			//�؂�o���ʒu��ݒ肷��
			m_HpGage->GetBackGageTexture()->SetRect(m_HpGage->GetRect());
		}
	}
}

void HyperEnemy::HealItemDrop()
{
	//�������擾����
	Rand* rand = Rand::GetInstance();

	//�h���b�v����m�������߂�
	int pro = ItemDrop::GetProbability(this->GetName(), this->GetLevel());

	//0����100�ŗ�����U��
	int result = rand->RandomMachine(0, 100);

	//�����̌��ʂ��h���b�v����m���ȉ��̏ꍇ�ɁA�h���b�v������
	if (pro >= result)
	{
		m_healItem->OnFall(this->GetPosition());
	}
}

void HyperEnemy::OnCollision_LeftPanel(ITextureBase* obj)
{
	//�X�e�[�g��ύX����
	m_AutoMoveState->SetNowState(DIRECT::LEFT);

	//�`��𔽓]������
	this->SetSpriteEffect(SpriteEffects::SpriteEffects_None);
}

void HyperEnemy::OnCollision_Player(ITextureBase* obj)
{
	//�v���C���[�Ɠ����������̏���
	CoroKun* player = dynamic_cast<CoroKun*>(obj);

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�v���C���[���̓����蒆�̂Ƃ�
		if (player->GetBodyBlow()->GetActive())
		{
			//�_�ł�����
			this->SetBlink(true);

			//�V���{���ʂ�����
			for (SoapBabble* babble : m_SoapBubbleList)
			{
				babble->SetState(STATE::NONE);
			}

			//�����ړ��̃X�e�[�g��ύX����
			m_AutoMoveState->SetExecute(false);

			//�A�j���[�V������؂�ւ���
			this->SetNowInfo("�n�C�p�[1");

			//�_���[�W�X�e�[�g�̏��𑗐M����
			staticData->Add("�n�C�p�[�G�l�~�[�_���[�W", obj->GetName());

			//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
			this->AddData1(ENEMYSTATE::NEXTSTATE);

			//�_���[�W���󂯂�
			this->OnDamage(player->GetAtack(), this->GetDefence());

			//�G�t�F�N�g���Đ�����
			m_masicEffectManager->NowEffectPlay(BodyBlow::TECHNIQUE_HEAL_COMMAND, BodyBlow::TECHNIQUE_HEAL_SPECIAL);

			//�X�R�A��ǉ�����
			score->AddScore(this->GetLevel() * BodyBlow::BASE_SCORE);
			score->SetFrameSave(this->GetLevel() * BodyBlow::BASE_SCORE, 60.0f);
		}
	}
}

void HyperEnemy::OnCollision_RightPanel(ITextureBase* obj)
{
	//�X�e�[�g��ύX����
	m_AutoMoveState->SetNowState(DIRECT::RIGHT);

	//�`��𔽓]������
	this->SetSpriteEffect(SpriteEffects::SpriteEffects_FlipHorizontally);
}

void HyperEnemy::OnCollision_LeftJumpPanel(ITextureBase* obj)
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	if (m_AutoMoveState->GetNowState() == DIRECT::LEFT)
	{
		//���s�֐����N����
		m_JumpState->SetExecute(true);

		//�W�����v�ҋ@���̎��̂݁A�W�����v����
		if (m_JumpState->GetNowState() == JUMP::JNONE)
		{
			//�U�����󂯂Ă��Ȃ��Ƃ�
			if (!this->GetBlink())
			{
				m_JumpState->SetNowState(JUMP::START);
				m_JumpState->SetJumpSpeed(10.0f);
				m_AutoMoveState->SetSpeed(Vector3(3.0f, 0.0f, 0.0f));
				this->SetNowInfo("�n�C�p�[5");
				this->SetLoopInfo("�n�C�p�[5");

				//�W�����v�����o��
				soundManager->Play(CRI_CUESHEET_0__45_�W�����v);
			}
		}
	}
}

void HyperEnemy::OnCollision_RightJumpPanel(ITextureBase* obj)
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	if (m_AutoMoveState->GetNowState() == DIRECT::RIGHT)
	{
		//���s�֐����N����
		m_JumpState->SetExecute(true);

		//�W�����v�ҋ@���̎��̂݁A�W�����v����
		if (m_JumpState->GetNowState() == JUMP::JNONE)
		{
			//�U�����󂯂Ă��Ȃ��Ƃ�
			if (!this->GetBlink())
			{
				m_JumpState->SetNowState(JUMP::START);
				m_JumpState->SetJumpSpeed(10.0f);
				m_AutoMoveState->SetSpeed(Vector3(3.0f, 0.0f, 0.0f));
				this->SetNowInfo("�n�C�p�[5");
				this->SetLoopInfo("�n�C�p�[5");

				//�W�����v�����o��
				soundManager->Play(CRI_CUESHEET_0__45_�W�����v);
			}
		}
	}
}

void HyperEnemy::OnCollision_Floor(ITextureBase* obj)
{
	//���W�̏C��
	Vector3 pos = this->GetPosition();
	this->SetPosition(Vector3(pos.x, obj->GetPosition().y - (this->AskOrigin().y * 2), pos.z));

	//�������̎��́A�x�N�g�����Đݒ肷��
	if (m_JumpState->GetNowState() == JUMP::FALL)
	{
		m_JumpState->SetNowState(JUMP::LANDING);
		m_AutoMoveState->SetSpeed(Vector3(2.0f, 0.0f, 0.0f));

		//�_���[�W���󂯂Ă��Ȃ���΁A�A�j���[�V������ύX����
		if (this->GetNowInfo() == "�n�C�p�[5")
		{
			this->SetNowInfo("�n�C�p�[2");
			this->SetLoopInfo("�n�C�p�[4");
			this->SetInitTimer(0);
		}
	}
}

void HyperEnemy::OnCollision_Wall(ITextureBase* obj)
{
	//X�����Ɉړ��ʂ��������o��
	this->ExtrudeX(obj);

	//�ړ��𔽑Ε����ɐ؂�ւ���
	m_AutoMoveState->ChangeState();

	//�`��𔽑΂ɂ���
	this->ChangeSpriteEffect();
}

void HyperEnemy::OnCollision_WhiteBack(ITextureBase* obj)
{
	//this->ExtrudeX(obj);

	//m_AutoMoveState->ChangeState();

	//this->ChangeSpriteEffect();
}

void HyperEnemy::OnCollision_NeedleOverAll(ITextureBase* obj)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//NeedleOverAll���擾����
	NeedleOverall* needleOver = GameContext::Get<NeedleOverall>();

	//�v���C���[�Ɠ����������̏���
	CoroKun* player = GameContext::Get<CoroKun>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�_�ł�����
		this->SetBlink(true);
		//�V���{���ʂ�����
		for (SoapBabble* babble : m_SoapBubbleList)
		{
			babble->SetState(STATE::NONE);
		}
		//�����ړ��̃X�e�[�g��ύX����
		m_AutoMoveState->SetExecute(false);
		//�ړ�������ύX����
		m_AutoMoveState->ChangeState();
		//������Ԃ�
		this->ChangeSpriteEffect();
		//�A�j���[�V������؂�ւ���
		this->SetNowInfo("�n�C�p�[1");
		//�_���[�W�X�e�[�g�̏��𑗐M����
		staticData->Add("�n�C�p�[�G�l�~�[�_���[�W", obj->GetName());
		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�␳�l��ݒ肷��
		float correction = 1.0f;

		//�j���������Ă���^�C�~���O�œ���������A�󂯂�_���[�W�͔���
		if (needleOver->GetNowState() == NEEDLEACTION::MOPE)correction = 0.5f;

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence(),correction);

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(NeedleOverall::TECHNIQUE_HEAL_COMMAND, NeedleOverall::TECHNIQUE_HEAL_SPECIAL);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * NeedleOverall::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * NeedleOverall::BASE_SCORE, 60.0f);
	}
}

void HyperEnemy::OnCollision_Resurrection(ITextureBase* obj)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//Resurrection���擾����
	Resurrection* resurrection = GameContext::Get<Resurrection>();

	//�v���C���[�Ɠ����������̏���
	CoroKun* player = GameContext::Get<CoroKun>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�_�ł�����
		this->SetBlink(true);
		//�V���{���ʂ�����
		for (SoapBabble* babble : m_SoapBubbleList)
		{
			babble->SetState(STATE::NONE);
		}
		//�����ړ��̃X�e�[�g��ύX����
		m_AutoMoveState->SetExecute(false);
		//�ړ�������ύX����
		m_AutoMoveState->ChangeState();
		//������Ԃ�
		this->ChangeSpriteEffect();
		//�A�j���[�V������؂�ւ���
		this->SetNowInfo("�n�C�p�[1");
		//�_���[�W�X�e�[�g�̏��𑗐M����
		staticData->Add("�n�C�p�[�G�l�~�[�_���[�W", obj->GetName());
		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�␳�l��ݒ肷��
		float correction = 1.0f;

		//�j���������Ă���^�C�~���O�œ���������A�󂯂�_���[�W�͔���
		if (resurrection->GetNowState() == RENEEDLE::LITTLE)correction = 0.5f;

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence(), correction);

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(Resurrection::TECHNIQUE_HEAL_COMMAND, Resurrection::TECHNIQUE_HEAL_SPECIAL);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * Resurrection::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * Resurrection::BASE_SCORE, 60.0f);
	}
}

void HyperEnemy::OnCollision_VirusBeam(ITextureBase* obj)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�v���C���[�Ɠ����������̏���
	CoroKun* player = GameContext::Get<CoroKun>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�_�ł�����
		this->SetBlink(true);
		//�V���{���ʂ�����
		for (SoapBabble* babble : m_SoapBubbleList)
		{
			babble->SetState(STATE::NONE);
		}
		//�����ړ��̃X�e�[�g��ύX����
		m_AutoMoveState->SetExecute(false);
		//�A�j���[�V������؂�ւ���
		this->SetNowInfo("�n�C�p�[1");
		//�_���[�W�X�e�[�g�̏��𑗐M����
		staticData->Add("�n�C�p�[�G�l�~�[�_���[�W", obj->GetName());
		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence());

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(VirusBeam::TECHNIQUE_HEAL_COMMAND, VirusBeam::TECHNIQUE_HEAL_SPECIAL);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * VirusBeam::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * VirusBeam::BASE_SCORE, 60.0f);
	}
}

void HyperEnemy::OnCollision_NormalBullet(ITextureBase* obj)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�v���C���[�Ɠ����������̏���
	CoroKun* player = GameContext::Get<CoroKun>();

	//�X�R�A���擾����
	Score* score = GameContext::Get<PlayScene>()->GetScore();

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (!TextureBase::GetBlink())
	{
		//�_�ł�����
		this->SetBlink(true);
		//�����ړ��̃X�e�[�g��ύX����
		m_AutoMoveState->SetExecute(false);
		//�A�j���[�V������؂�ւ���
		this->SetNowInfo("�n�C�p�[1");
		//�_���[�W�X�e�[�g�̏��𑗐M����
		staticData->Add("�n�C�p�[�G�l�~�[�_���[�W", obj->GetName());
		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�␳�l��ݒ肷��
		float correction = 1.0f;

		correction = obj->GetScale().x / 1.6f;

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence(),correction);

		int core1 = NormalTechnique::TECHNIQUE_HEAL_COMMAND * correction;
		int core2 = NormalTechnique::TECHNIQUE_HEAL_SPECIAL * correction;

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(core1,core2);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * (NormalBullet::BASE_SCORE * correction));
		score->SetFrameSave(this->GetLevel() * (NormalBullet::BASE_SCORE * correction), 60.0f);
	}
}

void HyperEnemy::State_AtackNone()
{
	//�v���C���[�̃|�C���^���擾
	CoroKun* player = GameContext::Get<CoroKun>();

	//�v���C���[�ƈ��͈͓��ɂ���Ƃ��ɍU������

	Vector3 len = Length(player->GetPosition(), this->GetPosition());

	//�W�����v���Ă��Ȃ��Ƃ�
	if (m_JumpState->GetNowState() == JUMP::JNONE)
	{
		//���͈͓��ɂ���Ƃ�
		if (len.x <= 200.0f && len.y <= 50.0f)
		{
			//�ړ����~�߂�
			m_AutoMoveState->SetExecute(false);
			//�A�j���[�V������؂�ւ���
			this->SetNowInfo("�n�C�p�[1");
			//�U���J�n
			m_ArrayBabble = NULL;
			//�X�e�[�g��؂�ւ���
			this->AddData1(ENEMYSTATE::ATACKSTART);
		}
	}
}

void HyperEnemy::State_AtackStart()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�A�^�b�N�^�C�����X�V����
	m_AtackTime++;

	//�U���p��SE���Đ�����
	soundManager->Play(CRI_CUESHEET_0__46_�A�U��);

	//�X�e�[�g��؂�ւ���
	this->AddData1(ENEMYSTATE::ATACKING);
}

void HyperEnemy::State_Atacking()
{
	//RandMachine���擾����
	Rand* Ramdom = Rand::GetInstance();

	//�A�^�b�N�^�C�����X�V����
	m_AtackTime++;

	//0.1�b��1��U������
	if (m_AtackTime % ATACKINTERVAL == 0)
	{
		//�Q�Ƃł��Ȃ��Ƃ��́A���s���Ȃ�
		if (m_ArrayBabble != -1)
		{
			//���_���摜�̒��S�Ƃ��čl����
			Vector3 center = this->GetPosition() + this->AskOrigin();

			//�G�͂��Ƃ��獶���������ʂȂ̂ŁA�������Ƃ邽�߂ɂ́A�����𔽓]������
			int dir = this->DirectX() * -1;

			//�����ɂ���āA���W���ς��
			center += Vector3(50.0f, 0.0f, 0.0f) * dir;

			//�x�N�g��������������
			Vector3 vel = Vector3::Zero;

			//X�x�N�g���̗���
			double x = Ramdom->RandomMachine(0.5, 2.0);

			//Y�x�N�g���̗���
			double y = Ramdom->RandomMachine(-1.0, 1.0);

			//�g��{���̗���
			double scale = Ramdom->RandomMachine(0.2, 1.0);

			//�擪�̖A���擾����
			SoapBabble* babble = m_SoapBubbleList[m_ArrayBabble];

			//���W��ݒ肷��
			babble->SetPosition(center);

			//�x�N�g����ݒ肷��
			babble->SetVelocity(Vector3((float)x * dir, (float)y, 0.0f));

			//�g��{����ݒ肷��
			babble->SetScale(Vector3((float)scale));

			//�`����N����
			babble->SetState(STATE::DRAW);

			if (m_ArrayBabble >= SOAPBABBLENUM - 1)m_ArrayBabble = -1;
			else m_ArrayBabble++;
		}
		//�z��̗v�f���Q�Ƃł��Ȃ��Ƃ��́A�X�e�[�g��؂�ւ���
		else this->AddData1(ENEMYSTATE::ATACKRESUTL);
	}
}

void HyperEnemy::State_AtackResult()
{
	//�ړ����ĊJ
	m_AutoMoveState->SetExecute(true);
	//�A�j���[�V������؂�ւ���
	this->SetNowInfo("�n�C�p�[2");
	//�X�e�[�g��؂�ւ���
	this->AddData1(ENEMYSTATE::NEXTSTATE);
}

void HyperEnemy::State_AtackNext()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�O�̃t���[������̌o�ߎ��Ԃ��擾����
	float elapsetTime = staticData->Get_FloatData("elapsedTime");

	//�v�l���Ԃ����Z���Ă���
	m_ThinkingTime += elapsetTime;

	//�Đ����̉����~�߂�
	soundManager->Stop(CRI_CUESHEET_0__46_�A�U��);

	//�v�l������������A�X�e�[�g��؂�ւ���
	if (m_ThinkingTime > THINKTIME)
	{
		//�v�l���Ԃ͏���������
		m_ThinkingTime = 0.0f;
		//�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::ENONE);
	}
}

void HyperEnemy::State_DeadStart()
{
	//�I���t���O���I���ɂ���
	m_Result = true;

	//�A�j���[�V�������Ԃ�����������
	this->SetInitTimer(NULL);

	//���S�A�j���[�V�������N������
	this->SetNowInfo("�n�C�p�[6");

	//���[�v����ݒ肷��
	this->SetLoopInfo("�n�C�p�[10");

	//�ړ��̎��s�֐��𗎂Ƃ�
	m_AutoMoveState->SetExecute(false);

	//�X�e�[�g��؂�ւ���
	this->AddData1(ENEMYSTATE::DEADTING);
}

void HyperEnemy::State_DeadTing()
{
	//�A�j���[�V�������I��
	if (this->GetNowInfo() == "�n�C�p�[10")
	{
		//�_�ł��J�n����
		if(!this->GetBlink())this->SetBlink(true);

		//�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::DEAD);
	}
}

void HyperEnemy::State_Dead()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�P�b�_�ł�����A�`���؂�
	if (!this->GetBlink())
	{
		//�`���؂�
		this->SetState(STATE::NONE);

		//���s�֐��𗎂Ƃ�
		this->SetActive(false);

		//HP�o�[�̕`���؂�
		m_HpGage->SetState(STATE::NONE);
		m_HpBack->SetState(STATE::NONE);

		//�o���l�̍��W��ݒ肷��
		m_experience->SetPosition(this->GetPosition());

		//�o���l�̒ǔ��p�̃X�e�[�g���N����
		m_experience->GetTrackingState()->SetExecute(true);

		//�z�[�~���O�p�̃X�e�[�g���N����
		m_experience->GetHomingState()->SetExecute(true);
		m_experience->GetHomingState()->SetStartFlag(true);

		//�o���l�̃X�e�[�g��؂�ւ���
		m_experience->SetNowState(TARGET_STATE::WAIT);

		//�o���l��`�悷��
		m_experience->SetState(STATE::DRAW);

		//�񕜃A�C�e�����h���b�v����
		this->HealItemDrop();

		//SE���Đ�����
		soundManager->Play(CRI_CUESHEET_0__31_�A�C�e���o��);
	}
}
