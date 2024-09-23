#include "pch.h"
#include "SuperEnemy.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"
#include "GameObject/Item/Drop/ItemDrop.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�v�l����
const float SuperEnemy::THINK_TIME = 5.0f;

//�G�̋���(�o���l�ɉe������)
const float SuperEnemy::STRENGTH = 200.0f;


SuperEnemy::SuperEnemy(ITextureBase* texture):
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_HpGage(nullptr),//Hp�Q�[�W
	m_HpBack(nullptr),//Hp�Q�[�W�̗���
	m_ThinkTime(0.0f),//�v�l����
	m_experience(nullptr),//�o���l
	m_masicEffectManager(nullptr),//�Z�̃G�t�F�N�g�̊Ǘ��N���X
	m_frameCount(NULL),//�t���[�����̃J�E���g
	m_damageCheck(false)//�_���[�W�̃`�F�b�N�t���O
{
	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//GameManager���擾����
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//�o���l�̉摜���擾����
	ITextureBase* ex = resourceManager->GetTexture("�o���l");

	//�`�揇��ύX
	this->SetDrawOrder(5);

	//�_�ł̊Ԋu��ݒ肷��
	TextureBase::SetTarget(0.1f);

	//�_�ł̏I���̐ݒ������
	TextureBase::SetFinal(1);
	TextureBase::SetFinalOption(true);

	//�Œ�C��̃C���f�b�N�X������������
	this->AddData2(0);

	//���s�֐����N����
	this->SetActive(true);


	//�A�j���[�V�����̐ݒ�
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList("SuperEnemy"))
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
	this->SetAniFlag(false);
	this->SetLoopFlag(false);

	//���݂̃A�j���[�V��������ݒ肷��
	this->SetNowInfo("�X�[�p�[1");
	this->SetLoopInfo("�X�[�p�[5");
	this->SetNowAnimation(this->GetNowAnimation());

	ITextureBase* Gage = resourceManager->GetTexture("WhiteBack");

	//�񕜃A�C�e���̉摜���擾����
	ITextureBase* healTex = resourceManager->GetTexture("��2");

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
	this->SetRadius(Vector3(this->AskOrigin()*2));

	//HealManager���擾����
	HealManager* healManager = GameContext::Get<PlayScene>()->GetHealManager();

	//�񕜃A�C�e���𐶐�����
	healManager->CreateItem(healTex, [&](Item* item)
		{	Heal* healItem = dynamic_cast<Heal*>(item);
	m_healItem = healItem; });
}

void SuperEnemy::Initialize()
{
	//PlayScene���擾����
	PlayScene* playScene = GameContext::Get<PlayScene>();

	//TextureManager���擾����
	TextureManager* gameManager = playScene->GetGameManager().get();

	//�ÓI�f�[�^���擾����
	StaticData* staticData = StaticData::GetInstance();

	//Player���擾����
	CoroKun* player = GameContext::Get<CoroKun>();

	//�Œ�C��̐������s��
	for (int i = 0; i < TURRET_NUM; i++)
	{
		//�C���X�^���X�𐶐�����
		std::unique_ptr<FixedTurret> pturret = std::make_unique<FixedTurret>();
		//���L�҂̃|�C���^��ݒ肷��
		pturret->SetOwner(this);
		//����������
		pturret->Initialize();
		//�`���؂�
		for (SoapBabble* babble : pturret->GetBabbleList())
		{
			babble->SetState(STATE::NONE);
		}
		//���L�������X�g�ɒǉ�����
		m_TurrentList.push_back(std::move(pturret));
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

void SuperEnemy::Update()
{
	//UI�̕\�����I���܂ŉ������Ȃ�
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//���s�֐���Active�łȂ���΁A�������Ȃ�
	if (!this->GetActive())return;

	//CameraManager���擾����
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//���݉ғ����Ă���J�������擾����
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//�J�������X�N���[���J�����̎��́A�X�V���~�߂�
	if (!nowCamera->GetObjectMove())return;

	//�A�j���[�V�����̍X�V����
	Animation::Update();

	//�摜�̐؂�o���ʒu���X�V����
	this->SetRect(this->GetNowAnimation());

	//�Q�[�W�̍X�V����
	this->UpdateGage();

	//�X�e�[�g�̍X�V����
	this->UpdateState();

	//�Œ�C��̍X�V
	for (std::unique_ptr<FixedTurret>& turret : m_TurrentList)
	{
		turret->Update();
	}

	//�摜�̍X�V����
	TextureBase::Update();

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

	//�����蔻��̍��W��ݒ肷��
	this->SetCenter(this->GetPosition());

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
				GameContext::Get<CollisionManager>()->AddUseOther(needle, this, 400.0f);
			}

			//�������ɔ�Ԑj�Ƃ̓����蔻���ݒ肷��
			std::vector<Needle*> needleOver = GameContext::Get<NeedleOverall>()->GetNeedleList();

			for (Needle* needle : needleOver)
			{
				//�����蔻���ݒ肷��
				GameContext::Get<CollisionManager>()->AddUseOther(needle, this, 300.0f);
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

void SuperEnemy::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void SuperEnemy::Finalize()
{
}

void SuperEnemy::OnCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::OnCollisionEnter(obj);

	//�v���C���[�ɓ��������Ƃ��̏���
	if (obj->GetName() == "SickChar")this->OnCollision_Player(obj);

	//�L�͈͍U���ɓ��������Ƃ��̏���
	if (obj->GetName() == "shortNeedle")this->OnCollision_NeedleOverAll(obj);

	//���͈͍U���ɓ��������Ƃ��̏���
	if (obj->GetName() == "longNeedle")this->OnCollision_Resurrection(obj);

	//�E�C���X�r�[���ɓ��������Ƃ��̏���
	if (obj->GetName() == "�E�C���X�r�[��")this->OnCollision_VirusBeam(obj);

	//�ʏ�e�ɓ��������Ƃ��̏���
	if (obj->GetName() == "�ʏ�e")this->OnCollision_NormalBullet(obj);

}

void SuperEnemy::NonCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::NonCollisionEnter(obj);
}

void SuperEnemy::OnDamage(const int& atack, const int& defence, const float correction)
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

void SuperEnemy::UpdateGage()
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

void SuperEnemy::UpdateState()
{
	//�X�e�[�g�̍X�V����
	//�X�e�[�g�ɂ���āA�����𕪂���

	//�_���[�W���󂯂Ă��Ȃ��Ƃ��ŁAHP��0�ɂȂ�����
	if (!this->GetBlink() && this->GetHP() <= NULL && this->GetNowData1() < ENEMYSTATE::DEADSTART)
	{
		//�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::DEADSTART);
	}

	switch (this->GetNowData1())
	{
	case ENEMYSTATE::ENONE:
		this->State_None();
		break;
	case ENEMYSTATE::ATACKSTART:
		this->State_Start();
		break;
	case ENEMYSTATE::ATACKING:
		this->State_Atacking();
		break;
	case ENEMYSTATE::ATACKRESUTL:
		this->State_AtackResult();
		break;
	case ENEMYSTATE::NEXTSTATE:
		this->State_Next();
		break;
	case ENEMYSTATE::DEADSTART:
		this->State_DeadStart();
		break;
	case ENEMYSTATE::DEADTING:
		this->State_Deadting();
		break;
	case ENEMYSTATE::DEAD:
		this->State_Dead();
		break;
	}
}

void SuperEnemy::State_None()
{
	//State_None�̏���

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�A�j���[�V�������N��������
	this->SetAniFlag(true);

	//���W��ݒ肷��
	for (SoapBabble* babble : m_TurrentList[this->GetNowData2()]->GetBabbleList())
	{
		babble->SetPosition(this->GetPosition());
	}

	//���˗p�̌x�����o��
	soundManager->Play(CRI_CUESHEET_0__43_�U���OSE);

	//���̃X�e�[�g�ֈڍs����
	this->AddData1(ENEMYSTATE::ATACKSTART);
}

void SuperEnemy::State_Start()
{
	//State_Start�̏���

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�A�j���[�V�������I���܂ő҂�
	if (!this->GetAniFlag())
	{
		//���˗p�̉����o��
		soundManager->Play(CRI_CUESHEET_0__44_����SE);

		//�����ړ��ƃW�����v�X�e�[�g���N��������
		m_TurrentList[this->GetNowData2()]->GetAutoMoveState()->SetExecute(true);
		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetExecute(true);

		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetNowState(JUMP::START);

		//�S�Ă̖A��`�悷��
		for (SoapBabble* babble : m_TurrentList[this->GetNowData2()]->GetBabbleList())
		{
			babble->SetState(STATE::DRAW);
		}

		//�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::ATACKING);
	}
}

void SuperEnemy::State_Atacking()
{
	//State_Atacking�̏���

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�W�����v���I��������A�`�������
	if (m_TurrentList[this->GetNowData2()]->GetJumpState()->GetJumpmDesignation())
	{
		//���e�p�̉����o��
		soundManager->Play(CRI_CUESHEET_0__42_���eSE);

		m_TurrentList[this->GetNowData2()]->GetParentBabble()->SetState(ITextureBase::STATE::NONE);

		//�����ړ��ƃW�����v�X�e�[�g���~������
		m_TurrentList[this->GetNowData2()]->GetAutoMoveState()->SetExecute(false);
		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetExecute(false);

		//�I���t���O���Đݒ肷��
		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetJumpmDesignation(false);

		//������U��
		for (SoapBabble* child : m_TurrentList[this->GetNowData2()]->GetChildBabble())
		{
			m_TurrentList[this->GetNowData2()]->RandShake(child);
		}

		//�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::ATACKRESUTL);
	}
}

void SuperEnemy::State_AtackResult()
{
	//State_AtackResult�̏���

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�v�l���Ԃ��o�߂�����A���ɖ߂�
	if (m_ThinkTime > THINK_TIME)
	{
		m_ThinkTime = 0.0f;

		//���݂̒e���擾����
		int bullet = this->GetNowData2();

		//�Ō�̒e�̏ꍇ�́A�ŏ��̋��ɖ߂�
		if (bullet == (int)m_TurrentList.size() - 1)bullet = NULL;
		
		else bullet++;

		//�e�𑕓U����
		this->AddData2(bullet);

		//�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::ENONE);
	}

	//�v�l���Ԃ��X�V����
	m_ThinkTime += staticData->Get_FloatData("elapsedTime");
}

void SuperEnemy::State_Next()
{
	//State_Next�̏���

	//�_�ł��I������܂ő҂�
	if (!this->GetBlink())
	{
		//�_�ł��I��������A�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::ENONE);

		//�v�l���Ԃ�����������
		m_ThinkTime = 0.0f;

		//�����ړ��ƃW�����v�X�e�[�g���~������
		m_TurrentList[this->GetNowData2()]->GetAutoMoveState()->SetExecute(false);
		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetExecute(false);

		//�I���t���O���Đݒ肷��
		m_TurrentList[this->GetNowData2()]->GetJumpState()->SetJumpmDesignation(false);
	}
}

void SuperEnemy::State_DeadStart()
{
	//State_DeadStart�̏���

	//TurretList�̏���
	for (std::unique_ptr<FixedTurret>& turret : m_TurrentList)
	{
		//���s�֐��𗎂Ƃ�
		turret->GetAutoMoveState()->SetExecute(false);
		turret->GetJumpState()->SetExecute(false);
		//�I���t���O���Đݒ肷��
		turret->GetJumpState()->SetJumpmDesignation(false);
		for (SoapBabble* babble : turret->GetBabbleList())
		{
			//�A�̕`���؂�
			babble->SetState(STATE::NONE);
		}
	}

	//�z��̎Q�ƃC���f�b�N�X������������
	this->AddData2(NULL);

	//�A�j���[�V������؂�ւ���
	this->SetNowInfo("�X�[�p�[6");

	//�A�j���[�V�����̃��[�v����ݒ肷��
	this->SetLoopInfo("�X�[�p�[10");

	//�A�j���[�V�������N��������
	this->SetAniFlag(true);

	//�X�e�[�g��؂�ւ���
	this->AddData1(ENEMYSTATE::DEADTING);
}

void SuperEnemy::State_Deadting()
{
	//State_Deadting�̏���

	//�A�j���[�V�������I������܂ő҂�
	if (this->GetNowInfo() == "�X�[�p�[10")
	{
		//�A�j���[�V�������I��������A�P�b�ԓ_�ł�����
		this->SetBlink(true);

		//�X�e�[�g��؂�ւ���
		this->AddData1(ENEMYSTATE::DEAD);
	}
}

void SuperEnemy::State_Dead()
{
	//State_Dead�̏���

	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�_�ł��I�����
	if (!this->GetBlink())
	{
		//�`���؂�
		this->SetState(STATE::NONE);

		//���s�֐��𗎂Ƃ�
		this->SetActive(false);

		//HP�o�[�̕`���؂�
		m_HpGage->SetState(STATE::NONE);
		m_HpBack->SetState(STATE::NONE);

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

void SuperEnemy::HealItemDrop()
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

void SuperEnemy::OnCollision_Player(ITextureBase* obj)
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

			//�A�j���[�V������؂�ւ���
			this->SetNowInfo("�X�[�p�[1");

			//�A�j���[�V�����̍X�V���~�߂�
			this->SetAniFlag(false);

			//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
			this->AddData1(ENEMYSTATE::NEXTSTATE);

			//�t���O�������I�ɔ��ˍς݂ɂ���
			m_TurrentList[this->GetNowData2()]->InitTurret();

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

void SuperEnemy::OnCollision_NeedleOverAll(ITextureBase* obj)
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

		//�A�j���[�V������؂�ւ���
		this->SetNowInfo("�X�[�p�[1");

		//�A�j���[�V�����̍X�V���~�߂�
		this->SetAniFlag(false);

		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�t���O�������I�ɔ��ˍς݂ɂ���
		m_TurrentList[this->GetNowData2()]->InitTurret();

		//�␳�l��ݒ肷��
		float correction = 1.0f;

		//�j���������Ă���^�C�~���O�œ���������A�󂯂�_���[�W�͔���
		if (needleOver->GetNowState() == NEEDLEACTION::MOPE)correction = 0.5f;

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence(), correction);

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(NeedleOverall::TECHNIQUE_HEAL_COMMAND, NeedleOverall::TECHNIQUE_HEAL_SPECIAL);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * NeedleOverall::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * NeedleOverall::BASE_SCORE, 60.0f);
	}
}

void SuperEnemy::OnCollision_Resurrection(ITextureBase* obj)
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

		//�A�j���[�V������؂�ւ���
		this->SetNowInfo("�X�[�p�[1");

		//�A�j���[�V�����̍X�V���~�߂�
		this->SetAniFlag(false);

		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�t���O�������I�ɔ��ˍς݂ɂ���
		m_TurrentList[this->GetNowData2()]->InitTurret();

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

void SuperEnemy::OnCollision_VirusBeam(ITextureBase* obj)
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

		//�A�j���[�V������؂�ւ���
		this->SetNowInfo("�X�[�p�[1");

		//�A�j���[�V�����̍X�V���~�߂�
		this->SetAniFlag(false);

		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�t���O�������I�ɔ��ˍς݂ɂ���
		m_TurrentList[this->GetNowData2()]->InitTurret();

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence());

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(VirusBeam::TECHNIQUE_HEAL_COMMAND, VirusBeam::TECHNIQUE_HEAL_SPECIAL);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * VirusBeam::BASE_SCORE);
		score->SetFrameSave(this->GetLevel() * VirusBeam::BASE_SCORE, 60.0f);
	}
}

void SuperEnemy::OnCollision_NormalBullet(ITextureBase* obj)
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

		//�A�j���[�V������؂�ւ���
		this->SetNowInfo("�X�[�p�[1");

		//�A�j���[�V�����̍X�V���~�߂�
		this->SetAniFlag(false);

		//�X�e�[�g��؂�ւ���(�_���[�W���󂯂���A�����I�Ɏv�l���ԂɈڂ�)
		this->AddData1(ENEMYSTATE::NEXTSTATE);

		//�t���O�������I�ɔ��ˍς݂ɂ���
		m_TurrentList[this->GetNowData2()]->InitTurret();

		//�␳�l��ݒ肷��
		float correction = 1.0f;

		correction = obj->GetScale().x / 1.6f;

		//�_���[�W���󂯂�
		this->OnDamage(player->GetAtack(), this->GetDefence(), correction);

		int core1 = NormalTechnique::TECHNIQUE_HEAL_COMMAND * correction;
		int core2 = NormalTechnique::TECHNIQUE_HEAL_SPECIAL * correction;

		//�G�t�F�N�g���Đ�����
		m_masicEffectManager->NowEffectPlay(core1, core2);

		//�X�R�A��ǉ�����
		score->AddScore(this->GetLevel() * (NormalBullet::BASE_SCORE * correction));
		score->SetFrameSave(this->GetLevel() * (NormalBullet::BASE_SCORE * correction), 60.0f);
	}
}
